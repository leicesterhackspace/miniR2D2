
#include "webHTML.h"




#define PART_BOUNDARY "123456789000000000000987654321"


static const char* _STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char* _STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char* _STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";


 /*
 * code for OTA
 */
typedef struct {
  esp_ota_handle_t handle;
  const esp_partition_t *update_partition;
  size_t received;
  bool started;
} ota_upload_ctx_t;

static ota_upload_ctx_t ota_ctx;


static esp_err_t index_handler(httpd_req_t *req){
  httpd_resp_set_type(req, "text/html");
  return httpd_resp_send(req, (const char *)INDEX_HTML, strlen(INDEX_HTML));
}




static esp_err_t update_page_handler(httpd_req_t *req){
  httpd_resp_set_type(req, "text/html");
  return httpd_resp_send(req, (const char *)UPDATE_HTML, strlen(UPDATE_HTML));
}

static esp_err_t ota_post_handler(httpd_req_t *req) {
  esp_err_t err;
  int remaining = req->content_len;

  if (!ota_ctx.started) {
    ota_ctx.update_partition = esp_ota_get_next_update_partition(NULL);
    if (ota_ctx.update_partition == NULL) {
      httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "No OTA partition");
      return ESP_FAIL;
    }
    err = esp_ota_begin(ota_ctx.update_partition, OTA_SIZE_UNKNOWN, &ota_ctx.handle);
    if (err != ESP_OK) {
      httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "esp_ota_begin failed");
      return ESP_FAIL;
    }
    ota_ctx.started = true;
    ota_ctx.received = 0;
  }

  char buf[1024];
  while (remaining > 0) {
    int to_read = remaining > sizeof(buf) ? sizeof(buf) : remaining;
    int read_len = httpd_req_recv(req, buf, to_read);
    if (read_len <= 0) {
      if (read_len == HTTPD_SOCK_ERR_TIMEOUT) {
        continue; // retry
      }
      httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Receive error");
      return ESP_FAIL;
    }

    err = esp_ota_write(ota_ctx.handle, buf, read_len);
    if (err != ESP_OK) {
      httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "esp_ota_write failed");
      return ESP_FAIL;
    }

    ota_ctx.received += read_len;
    remaining -= read_len;
  }

  err = esp_ota_end(ota_ctx.handle);
  if (err != ESP_OK) {
    httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "esp_ota_end failed");
    return ESP_FAIL;
  }

  err = esp_ota_set_boot_partition(ota_ctx.update_partition);
  if (err != ESP_OK) {
    httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "set_boot_partition failed");
    return ESP_FAIL;
  }

  httpd_resp_set_type(req, "text/plain");
  httpd_resp_sendstr(req, "OK, rebooting");

  // Give the response time to go out
  delay(100);
  esp_restart();
  return ESP_OK; // not reached
}
//end of ota code
 


void startr2d2Server(){
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  config.server_port = 80;
  httpd_uri_t index_uri = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = index_handler,
    .user_ctx  = NULL
  };

      
   httpd_uri_t update_page_uri = {
    .uri       = "/update",
    .method    = HTTP_GET,
    .handler   = update_page_handler,
    .user_ctx  = NULL
  };
  
   httpd_uri_t ota_uri = {
    .uri       = "/update",
    .method    = HTTP_POST,
    .handler   = ota_post_handler,
    .user_ctx  = NULL
  };
  


  
}
