

////////////////////////////////////////////////////////////////////////////////////////

void LED1() {         // function to flash the dome lights when a sound is being played

  if (soundbutton != 0) 
  {//1
    if (currentMillis - previousMillis7 >= ledinterval) 
    {//2
      previousMillis7 = currentMillis;
      if (ledState == LOW) 
      {
        ledState = HIGH;
        ledinterval = random(ledonmin, ledonmax);       //set a random time dealy when the sound is being played
        pwm2.setPWM(0, 4096, 0);
        
      }
      else 
      {
        ledState = LOW;
        ledinterval = random(ledoffmin, ledoffmax);       //set a random time dealy when the sound is being played
        pwm2.setPWM(0, 0, 4096);
       
      }

    }//2
  }
    else 
    {
      pwm2.setPWM(0, 4096, 0);
      
    }

}

void LED2() {         // function to flash the dome lights when a sound is being played

  if (soundbutton != 0) 
  {
    if (currentMillis - previousMillis8 >= ledinterval1) 
    {
      previousMillis8 = currentMillis;
      if (ledState1 == LOW) 
      {
        ledState1 = HIGH;
        ledinterval1 = random(ledonmin, ledonmax);       //set a random time dealy when the sound is being played
        pwm2.setPWM(1, 4096, 0);
      }
      else 
      {
        ledState1 = LOW;
        ledinterval1 = random(ledoffmin, ledoffmax);       //set a random time dealy when the sound is being played
        pwm2.setPWM(1, 0, 4096);
      }

    }
  }
    else 
    {
      pwm2.setPWM(1, 4096, 0);
      
    }
}

void LED3() {         // function to flash the dome lights when a sound is being played

  if (soundbutton != 0) 
  {
    if (currentMillis - previousMillis9 >= ledinterval2) 
    {
      previousMillis9 = currentMillis;
      if (ledState2 == LOW) 
      {
        ledState2 = HIGH;
        ledinterval2 = random(ledonmin, ledonmax);       //set a random time dealy when the sound is being played
        pwm2.setPWM(2, 4096, 0);
      }
      else 
      {
        ledState2 = LOW;
        ledinterval2 = random(ledoffmin, ledoffmax);       //set a random time dealy when the sound is being played
        pwm2.setPWM(2, 0, 4096);
      }

    }
  }
  
    else {
      pwm2.setPWM(2, 4096, 0);
       
    }
}

void LED4() {         // function to flash the dome lights when a sound is being played

  if (soundbutton != 0) 
  {
    if (currentMillis - previousMillis10 >= ledinterval3) 
    {
      previousMillis10 = currentMillis;
      if (ledState3 == LOW) 
      {
        ledState3 = HIGH;
        ledinterval3 = random(ledonmin, ledonmax);       //set a random time dealy when the sound is being played
        pwm2.setPWM(3, 4096, 0);
      }
      else 
      {
        ledState3 = LOW;
        ledinterval3 = random(ledoffmin, ledoffmax);       //set a random time dealy when the sound is being played
        pwm2.setPWM(3, 0, 4096);
      }

    }
  }
    else 
    {
      pwm2.setPWM(3, 4096, 0);
      
    }
  
}

void LED5() {         // function to flash the dome lights when a sound is being played

  if (soundbutton != 0) 
  {//1
    if (currentMillis - previousMillis11 >= ledinterval4) 
    {//2
      previousMillis11 = currentMillis;
      if (ledState4 == LOW) 
      {//3
        ledState4 = HIGH;
        ledinterval4 = random(ledonmin, ledonmax);       //set a random time dealy when the sound is being played
        pwm2.setPWM(7, 4096, 0);
      } //3
      else 
      {//4
        ledState4 = LOW;
        ledinterval4 = random(ledoffmin, ledoffmax);       //set a random time dealy when the sound is being played
        pwm2.setPWM(7, 0, 4096);
      }//4

    }//2
   }//1
    else 
    {//5
      pwm2.setPWM(7, 4096, 0);
      
    }//5
  
}


// these are just called

void holoServo1() {
  //Holo Servo1/////////////////////////////
  randomtime1 = random(500, 2000); //servo 1 random time position generation in milliseconds
  if (currentMillis - previousMillis1 >= randomtime1) {
    previousMillis1 = currentMillis;
    pos1 = random(150, 600);
    pwm2.setPWM(4, 0, pos1);
  }
  else {
    pwm2.setPWM(4, 0, holoCentreVal);
  }
}

void holoServo2() {
  //Holo Servo2//////////////////////////////
  randomtime2 = random(500, 2000); //servo 2 random time position generation in milliseconds
  if (currentMillis - previousMillis2 >= randomtime2) {
    previousMillis2 = currentMillis;
    pos2 = random(150, 600);
    pwm2.setPWM(5, 0, pos2);
  }
  else {
    pwm2.setPWM(5, 0, holoCentreVal);
  }
}

void holoServo3() {
  //Holo Servo3//////////////////////////////
  randomtime3 = random(500, 2000); //servo 3 random time position generation in milliseconds
  if (currentMillis - previousMillis3 >= randomtime3) {
    previousMillis3 = currentMillis;
    pos3 = random(150, 600);
    pwm2.setPWM(6, 0, pos3);
  }
  else {
    pwm2.setPWM(6, 0, holoCentreVal);
  }
}

/////////////////////////////////////////////////////////////////////////////////////////

void holoLED1() {
  //Holo LED 1
  if (currentMillis - previousMillis4 >= hololedinterval1) {
    previousMillis4 = currentMillis;
    if (hololedState1 == LOW) {
      hololedState1 = HIGH;
      hololedinterval1 = random(hololedonmin, hololedonmax);       //set a random time dealy
      pwm2.setPWM(13, 0, 4096);
    }
    else {
      hololedState1 = LOW;
      hololedinterval1 = random(hololedoffmin, hololedoffmax);       //set a random time dealy
      pwm2.setPWM(13, 4096, 0);
    }
  }
}

void holoLED2() {

  //Holo LED 2
  if (currentMillis - previousMillis5 >= hololedinterval2) {
    previousMillis5 = currentMillis;
    if (hololedState2 == LOW) {
      hololedState2 = HIGH;
      hololedinterval2 = random(hololedonmin, hololedonmax);       //set a random time dealy
      pwm2.setPWM(14, 0, 4096);
    }
    else {
      hololedState2 = LOW;
      hololedinterval2 = random(hololedoffmin, hololedoffmax);       //set a random time dealy
      pwm2.setPWM(14, 4096, 0);
    }
  }
}

void holoLED3() {

  //Holo LED 3
  if (currentMillis - previousMillis6 >= hololedinterval3) {
    previousMillis6 = currentMillis;
    if (hololedState3 == LOW) {
      hololedState3 = HIGH;
      hololedinterval3 = random(hololedonmin, hololedonmax);       //set a random time dealy
      pwm2.setPWM(15, 0, 4096);
    }
    else {
      hololedState3 = LOW;
      hololedinterval3 = random(hololedoffmin, hololedoffmax);       //set a random time dealy
      pwm2.setPWM(15, 4096, 0);
    }
  }
}

void blinkPeriscopeLed(){
  if (periscopestate == 1) { // blink the periscope LED
    if (currentMillis - previousMillis12 >= periledinterval) {
      previousMillis12 = currentMillis;
      if (periledState == LOW) {
        periledState = HIGH;
        periledinterval = random(periledonmin, periledonmax);       //set a random time dealy
        pwm2.setPWM(11, 0, 4096);
      }
      else {
        periledState = LOW;
        periledinterval = random(periledoffmin, periledoffmax);       //set a random time dealy
        pwm2.setPWM(11, 4096, 0);
      }
    }
  }
  else if (periscopestate == 0) {
    pwm2.setPWM(11, 0, 4096); // turn off the led in the periscope
  }
  
}

void updatePeriscopePosition(){
  periscopestate = !periscopestate;
  periscopeposition = periscopestate==0? peridown : periup ;
  pwm2.setPWM(10, 0, periscopeposition);
}

// Dome PCA9685 module, lights and Holo proector servos, board will require the A0 tab to be bridged with solder
// Pulse ranges for the servo positions to the PCA9685 board 0-180 to 150-600

void servoSetup2() {
  pwm2.setPWM(0, 4096, 0); //Dome lights
  pwm2.setPWM(1, 4096, 0); //Dome lights
  pwm2.setPWM(2, 4096, 0); //Dome lights
  pwm2.setPWM(3, 4096, 0); //Dome lights
  pwm2.setPWM(4, 0, holoCentreVal); // Holo Servo 1
  pwm2.setPWM(5, 0, holoCentreVal); // Holo Servo 2
  pwm2.setPWM(6, 0, holoCentreVal); // Holo Servo 3
  pwm2.setPWM(7, 4096, 0); // Dome lights
  pwm2.setPWM(8, 0, 0); //
  pwm2.setPWM(9, 0, 0); //
  pwm2.setPWM(10, 0, peridown); //Periscope
  pwm2.setPWM(11, 4096, 0); //Periscope LED
  pwm2.setPWM(12, 0, 0); //
  pwm2.setPWM(13, 4096, 0); // holo light 1
  pwm2.setPWM(14, 4096, 0); // holo light 2
  pwm2.setPWM(15, 4096, 0); // holo light 3
}
