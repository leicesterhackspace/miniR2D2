#include <Bluepad32.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <ESP32MX1508.h>
// called this way, it uses the default address 0x40

Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x41);


//
// Depending on your servo make, the pulse width min and max may vary, you 
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

// motor definitions for body
#define IN1 19 //Left Motor Driver IN1 5
#define IN2 18 //Left Motor Driver IN2 3
#define IN3 16 //Right Motor Driver IN1 6
#define IN4 17 //Right Motor Driver IN2 9
#define IN5 12 //Dome Motor Driver IN1    //uncomment if using dc motor for dome
#define IN6 13 //Dome Motor Driver IN2    //uncomment if using dc motor for dome
#define PWM_ch0 0
#define PWM_ch1 1
#define PWM_ch2 2
#define PWM_ch3 3
#define PWM_ch4 4
#define PWM_ch5 5

//Define the motor driver details on the MX1508
#define RES 12  // 4095 rewsolution
int pwmResolution =pow(2, RES) - 1;

MX1508 motorA(IN1, IN2, PWM_ch0, PWM_ch1, RES);
MX1508 motorB(IN3, IN4, PWM_ch2, PWM_ch3, RES);
MX1508 motorC(IN5, IN6, PWM_ch4, PWM_ch5, RES);
#define joystickMax 350

// numchukdefinitions
#define numchukC 1
#define numchukZ 2
#define trigger 8

GamepadPtr myGamepads[BP32_MAX_GAMEPADS];
bool Connected = false;

float elapsedTime, currentTime, timePrev, soundStartTime;

// Variables to adust for front arm open and close limits for servo movement
int frontarm1min = 30;
int frontarm1max = 170;

int frontarm2min = 30;
int frontarm2max = 170;

// Variables to adust for Centre leg lift and Tilt servo mechanism
int tiltmin = 133;
int tiltmax = 40;

int liftup = 125;
int liftdown = 28;
// BODY variables
int oldDpad = -1;
int centreliftstate = 0;
int liftPos = 0;
int tiltState = 0;
int tiltPos = 0;


//DOME  variables
int periscopestate =0;
int periscopeposition = 0;

// Variables to adust for periscope lift servo
int periup = 90;
int peridown = 155;

// Variables for dome servo
int domeservomin = 30;
int domeservomax = 150;
int domeservocentre = 90;

// values to adust for the LED random number blink, min and max delays
int ledonmin = 200; // value in milliseconds
int ledonmax = 500;
int ledoffmin = 50; // value in milliseconds
int ledoffmax = 150;

// Holo position values
int holoCentreVal = 300;      // change this value if the servos for the holo proectors keep rotating

int pos1 = 0;    // variable to store the servo1 position
int pos2 = 0;    // variable to store the servo2 position
int pos3 = 0;    // variable to store the servo3 position

int randompos1 = 0; //random number for servo 1
int randompos2 = 0; //random number for servo 2
int randompos3 = 0; //random number for servo 3

int randomtime1 = 0; // random time delay for movement 1
int randomtime2 = 0; // random time delay for movement 2
int randomtime3 = 0; // random time delay for movement 3
int randomtime4 = 0; // random time delay for movement 4

// Dome lights
int hololedinterval1 = 200;    // delay to blink the LED lights in the dome
int hololedinterval2 = 200;    // delay to blink the LED lights in the dome
int hololedinterval3 = 200;    // delay to blink the LED lights in the dome
int periledinterval = 500;    // delay to blink the LED lights in the Periscope
int hololedState1 = LOW;       // ledState used to set the LED
int hololedState2 = LOW;       // ledState used to set the LED
int hololedState3 = LOW;       // ledState used to set the LED
int periledState = LOW;       // ledState used to set the LED
int hololedonmin = 300;
int hololedonmax = 1000;
int hololedoffmin = 100; // value in milliseconds
int hololedoffmax = 500;
int periledonmin = 300; //periscope LED on off values
int periledonmax = 600;
int periledoffmin = 300; //
int periledoffmax = 600;

int soundbutton = 0;

int ledinterval = 200;    // delay to blink the LED lights in the dome
int ledinterval1 = 200;
int ledinterval2 = 200;
int ledinterval3 = 200;
int ledinterval4 = 200;
int ledState = LOW;             // ledState used to set the LED
int ledState1 = LOW;
int ledState2 = LOW;
int ledState3 = LOW;
int ledState4 = LOW;


static unsigned long lastMilli = 0;
unsigned long currentMillis; // time current
unsigned long previousMillis = 0;        // will store last time LED was updated
unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;
unsigned long previousMillis3 = 0;
unsigned long previousMillis4 = 0;
unsigned long previousMillis5 = 0;
unsigned long previousMillis6 = 0;
unsigned long previousMillis7 = 0;
unsigned long previousMillis8 = 0;
unsigned long previousMillis9 = 0;
unsigned long previousMillis10 = 0;
unsigned long previousMillis11 = 0;
unsigned long previousMillis12 = 0;
unsigned long previousMillis13 = 0; // used for testing only
unsigned long previousMillis14 = 0; // used for testing only
unsigned long lastservotime = 0;
unsigned long lastDebounceTime = 0;  // place holder for debounce time
unsigned long debounceDelay = 100;    // the debounce time; increase if the output flickers


// This callback gets called any time a new gamepad is connected.
// Up to 4 gamepads can be connected at the same time.
void onConnectedGamepad(GamepadPtr gp) {
  bool foundEmptySlot = false;
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myGamepads[i] == nullptr) {
      Serial.printf("CALLBACK: Gamepad is connected, index=%d\n", i);
      // Additionally, you can get certain gamepad properties like:
      // Model, VID, PID, BTAddr, flags, etc.
      GamepadProperties properties = gp->getProperties();
      Serial.printf("Gamepad model: %s, VID=0x%04x, PID=0x%04x\n",
                    gp->getModelName().c_str(), properties.vendor_id,
                    properties.product_id);
      myGamepads[i] = gp;
      foundEmptySlot = true;
      break;
    }
  }
  if (!foundEmptySlot) {
    Serial.println(
        "CALLBACK: Gamepad connected, but could not found empty slot");
  }
}

void onDisconnectedGamepad(GamepadPtr gp) {
  bool foundGamepad = false;

  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myGamepads[i] == gp) {
      Serial.printf("CALLBACK: Gamepad is disconnected from index=%d\n", i);
      myGamepads[i] = nullptr;
      foundGamepad = true;
      break;
    }
  }

  if (!foundGamepad) {
    Serial.println(
        "CALLBACK: Gamepad disconnected, but not found in myGamepads");
  }
}

// Arduino setup function. Runs in CPU 1
void setup() {
  Serial.begin(115200);
  Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
  const uint8_t *addr = BP32.localBdAddress();
  Serial.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n", addr[0], addr[1], addr[2],
                addr[3], addr[4], addr[5]);

  // Setup the Bluepad32 callbacks
  BP32.setup(&onConnectedGamepad, &onDisconnectedGamepad);

  // "forgetBluetoothKeys()" should be called when the user performs
  // a "device factory reset", or similar.
  // Calling "forgetBluetoothKeys" in setup() just as an example.
  // Forgetting Bluetooth keys prevents "paired" gamepads to reconnect.
  // But might also fix some connection / re-connection issues.
  BP32.forgetBluetoothKeys();
 

 
  /*
   * In theory the internal oscillator (clock) is 25MHz but it really isn't
   * that precise. You can 'calibrate' this by tweaking this number until
   * you get the PWM update frequency you're expecting!
   * The int.osc. for the PCA9685 chip is a range between about 23-27MHz and
   * is used for calculating things like writeMicroseconds()
   * Analog servos run at ~50 Hz updates, It is importaint to use an
   * oscilloscope in setting the int.osc frequency for the I2C PCA9685 chip.
   * 1) Attach the oscilloscope to one of the PWM signal pins and ground on
   *    the I2C PCA9685 chip you are setting the value for.
   * 2) Adjust setOscillatorFrequency() until the PWM update frequency is the
   *    expected value (50Hz for most ESCs)
   * Setting the value here is specific to each individual I2C PCA9685 chip and
   * affects the calculations for the PWM update frequency. 
   * Failure to correctly set the int.osc value will cause unexpected PWM results
   */
  pwm1.begin();
  pwm1.setPWMFreq(50);  // Analog servos run at ~50 Hz updates  
  pwm2.begin();
  pwm2.setPWMFreq(50);  // standard for analog servos

  
// Convert values from degrees to Pulse ranges for the servo positions to the PCA9685 board 0-180 to 150-600
  frontarm1min = (((450 / 180) * frontarm1min) + 150);
  frontarm1max = (((450 / 180) * frontarm1max) + 150);
  frontarm2min = (((450 / 180) * frontarm2min) + 150);
  frontarm2max = (((450 / 180) * frontarm2max) + 150);
  tiltmin = (((450 / 180) * tiltmin) + 150);
  tiltmax = (((450 / 180) * tiltmax) + 150);
  liftup = (((450 / 180) * liftup) + 150);
  liftdown = (((450 / 180) * liftdown) + 150);
  periup = (((450 / 180) * periup) + 150);
  peridown = (((450 / 180) * peridown) + 150);
  domeservomin = (((450 / 180) * domeservomin) + 150);
  domeservomax = (((450 / 180) * domeservomax) + 150);
  domeservocentre = (((450 / 180) * domeservocentre) + 150);

  //Set the servos to their start positions

  servoSetup(); // view function and end of code  
  servoSetup2(); // view function and end of code  

 
}

///////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////

// Arduino loop function. Runs in CPU 1
void loop() {
  
  int newDpad;
  int buttonPressed;
  int armDemand;
  
  // This call fetches all the gamepad info from the NINA (ESP32) module.
  // Just call this function in your main loop.
  // The gamepads pointer (the ones received in the callbacks) gets updated
  // automatically.
  BP32.update();

  // It is safe to always do this before using the gamepad API.
  // This guarantees that the gamepad is valid and connected.
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    pwm1.setPWM(14, 4096, 0); // led on
    pwm1.setPWM(15, 0, 4096);
    GamepadPtr myGamepad = myGamepads[i];
    if (myGamepad && myGamepad->isConnected()) {
      // Another way to query the buttons, is by calling buttons(), or
      // miscButtons() which return a bitmask.
      // Some gamepads also have DPAD, axis and more.
      // ought to check that is a wii controller
      pwm1.setPWM(15, 4096, 0); // led on
      pwm1.setPWM(14, 0, 4096);
      currentMillis = millis();
      timePrev = currentTime;                                                      // the previous time is stored before the actual time read
      currentTime = millis();                                                      // actual time read
      elapsedTime = (currentTime - timePrev);                                      //elapsedTime = (time - timePrev) / 1000;
      lastDebounceTime = millis();
      
      newDpad = myGamepad->dpad();
      if (oldDpad != newDpad){
        oldDpad = newDpad;
        switch(newDpad){
          case 1:
               centreliftstate = !centreliftstate;
               liftPos = centreliftstate==0? liftup : liftdown;
               pwm1.setPWM(3, 0, liftPos) ; // will need to check centrelift position
               break;
          case 2:
               if (centreliftstate == 1){ // centre lift must be down before tilting
                tiltState = !tiltState;
                tiltPos = tiltState==0? tiltmin : tiltmax;
                pwm1.setPWM(4, 0, tiltPos) ; 
               }
               break;
         case 4: // periscope left?
               updatePeriscopePosition();
               
               break;
        }
 

      }; // end of dpad changed
     // motor control
     buttonPressed = myGamepad->buttons() ;
    
     switch( buttonPressed & 0xb){
      case numchukZ:
         dome(myGamepad->axisX());
         motorA.motorStop();
         motorB.motorStop();
         break;

      case numchukC:
        frontArm(myGamepad->axisY());
        motorA.motorStop();
        motorB.motorStop();
        motorC.motorStop();
        break;
      case trigger:
        if ( soundbutton == 0){
          soundbutton = 1; // only set here and re-set out of loop
          soundStartTime = currentTime;
        
        }
        break;
      default:
        motordrivers(myGamepad->axisX(), myGamepad->axisY());
        motorC.motorStop();
        break;
     }
     
      // soundbutton will be reset when sound ahs finished
      if ( soundbutton ==1 && ((currentTime - soundStartTime )) > 5000.0){
        soundbutton = 0;
      }
      LED1();
      LED2();
      LED3();
      LED4();
      LED5();
      holoLED1();                                                       // holo lights
      holoLED2();
      holoLED3();
      holoServo1();                                                       // randome holo servo timing
      holoServo2();
      holoServo3();
      blinkPeriscopeLed();

    }; // end of id connected
      // You can query the axis and other properties as well. See Gamepad.h
      // For all the available functions.
    } // end of for each game pad loop
 

  // The main loop must have some kind of "yield to lower priority task" event.
  // Otherwise the watchdog will get triggered.
  // If your main loop doesn't have one, just add a simple `vTaskDelay(1)`.
  // Detailed info here:
  // https://stackoverflow.com/questions/66278271/task-watchdog-got-triggered-the-tasks-did-not-reset-the-watchdog-in-time

  //vTaskDelay(1);
  delay(10);
  
} // end of loop()
