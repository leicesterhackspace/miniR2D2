



void motordrivers(int forward, int left) {     // for the main drive wheels
  int output1, output2;
  int rawLeft, rawRight;
  int diff;
 
  //////////////////////////read in the values from bluetooth///////////////////
  output1 = constrain(left, -joystickMax, joystickMax);
  output1 = map(output1, -joystickMax, joystickMax, -pwmResolution, pwmResolution);

  output2 = constrain(forward, -joystickMax, joystickMax);
  output2 = map(output2, -joystickMax, joystickMax, -pwmResolution, pwmResolution); // swap the -pwmResolution and pwmResolution if the left and rigth turn the wrong way
  
  rawLeft = output2 + output1;
  rawRight = output2 - output1;

  rawLeft = constrain(rawLeft, -pwmResolution, pwmResolution) ; // constrain the PWM to max pwmResolution
  rawRight = constrain(rawRight, -pwmResolution, pwmResolution) ;
  Serial.print("rawleft ");
  Serial.println(rawLeft);
  if (rawLeft > 0){
    motorA.motorGo(rawLeft );
    //Serial.printf("rawleft %d   \n", rawLeft);
  }
  else if (rawLeft < 0){
    motorA.motorRev(-rawLeft );
    //Serial.printf("rawleft %d   \n", rawLeft);
  }
  else {
    motorA.motorStop();
    //Serial.println("rawleft stop ");
  }
Serial.print("rawright ");
Serial.println(rawRight);
 
  if (rawRight > 0){
    motorB.motorGo(rawRight);
    //Serial.printf("          rawright %d \n",  rawRight);
  }
  else if (rawRight < 0){
    motorB.motorRev(-rawRight );
    //Serial.printf("            rawright %d \n", rawRight);
  }
  else {
    motorB.motorStop();
    //Serial.println("             rawright stop");
  }
  
}

////////////////////////////////////////////////////////////////////////////////////////
void dome(int speed) {
  int output1 ;
  
  //////////////////////////read in the values from bluetooth///////////////////
  output1 = constrain(speed, -joystickMax, joystickMax);
  output1 = map(output1, -joystickMax, joystickMax, -pwmResolution, pwmResolution);
  // if (output1 < 0)output1 = 0;  // looks like dome spins one way only change if wrong direction commented out ooo-xxx
  // Serial.println(output1);
  motorC.motorGo(output1);


}


////////////////////////////////////////////////////////////////////////////////////////
void frontArm(int demand){
  // first map demand, joystick position to range 0 to 90
  int mappedDemand;
  int servo0, servo1;
  // at moment frontarm 1 and 2 values are same, may need separate line for frontarm2
  mappedDemand = constrain(demand,  -joystickMax, joystickMax);
  mappedDemand = map(mappedDemand, -joystickMax, joystickMax, frontarm1min, frontarm1max);
  pwm1.setPWM(0, 0, mappedDemand);
  pwm1.setPWM(1, 0, mappedDemand);
}
//////////////////////////////////////////////////////////////////////////////
void frontarm1(int demand) {

  int output4 = demand;
  output4 = constrain(output4, 110, 200);
  output4 = map(output4, 110, 200, frontarm1min, frontarm1max);

  /////////////////////
  if (output4 < frontarm1max || output4 > frontarm1min) {
    output4 = constrain(output4, frontarm1min, frontarm1max);
    pwm1.setPWM(0, 0, output4);
  }
  else {
    pwm1.setPWM(0, 0, frontarm1min);
  }
}

////////////////////////////////////////////////////////////////////////////////////////

void frontarm2(int demand) {

  int output4 = demand;
  output4 = constrain(output4, 0, 90);
  output4 = map(output4, 0, 90, frontarm2max, frontarm2min);

  /////////////////////
  if (output4 < frontarm1max || output4 > frontarm1min) {
    output4 = constrain(output4, frontarm2min, frontarm2max);
    pwm1.setPWM(1, 0, output4);
  }
  else {
    pwm1.setPWM(1, 0, frontarm2min);
  }
}

//////

/////////////////////////////////////////////////////////////////////////////////////////
// (pin, 4096, 0) turns pin fully on, (pin, 0, 4096) turns pin fully off
// Body PCA9685 module, front arms, tilt servo and centre lift servo

void servoSetup() {
  pwm1.setPWM(0, 0, frontarm1min); //front arm 1
  pwm1.setPWM(1, 0, frontarm2min); //front arm 2
  pwm1.setPWM(2, 0, 0); //
  pwm1.setPWM(3, 0, liftup); //centre leg lift
  pwm1.setPWM(4, 0, tiltmin);// Body Tilt
  pwm1.setPWM(5, 0, 0);
  pwm1.setPWM(6, 0, 0); //
  pwm1.setPWM(7, 0, 0); //
  pwm1.setPWM(8, 0, 0); //
  pwm1.setPWM(9, 0, 0); //
  pwm1.setPWM(10, 0, 0); //
  pwm1.setPWM(11, 0, 0); //
  pwm1.setPWM(12, 0, 0); //
  pwm1.setPWM(13, 0, 300); //
  pwm1.setPWM(14, 4096, 0); // led on
  pwm1.setPWM(15, 0, 4096);  //led off
}
