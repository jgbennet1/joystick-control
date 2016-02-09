#include <SoftwareServo.h>

const int servo1 = 0;       // first servo
const int servo2 = 1;       // second servo
const int pan = 2;        // L/R Parallax Thumbstick
const int pitch = 5;        // U/D Parallax Thumbstick

int servo1Val;           // variable to read the value from the analog pin
int servo2Val;           // variable to read the value from the analog pin
int panVal;
int pitchVal;
int panMv = 0;
int pitchMv = 0;
int joyHctr;
int joyVctr;


SoftwareServo myservo1;  // create servo object to control a servo
SoftwareServo myservo2;  // create servo object to control a servo



void setup() {

  // Servo  
  myservo1.attach(servo1);  // attaches the servo to servo1 pin
  myservo2.attach(servo2);  // attaches the servo to servo2 pin

  // Find center point of joystick
  joyHctr = analogRead(pan);
  joyVctr = analogRead(pitch);

  // Inizialize Serial
  Serial.begin(9600);
}

/**
* Main
*/

void loop(){

    // Display Joystick values using the serial monitor
    // int outputJoystick();
    // Serial.print("pan ctr -- ");
    // Serial.println(joyHctr);
    // Serial.print("pitch ctr -- ");
    // Serial.println(joyVctr);
    // delay(1000);

    // Test for reset condition (analogRead(pan) >1010 & analogRead(pitch) < 10)
	  panVal = analogRead(pan);
	  pitchVal = analogRead(pitch);
    // Serial.print("pan -- ");
    // Serial.print(panMv);
    // Serial.print(" :: pitch -- ");
    // Serial.println(pitchMv);
    // Serial.println("-----------------------------");
    // Serial.println();

	  int resetTest = (panVal - pitchVal);    // set resetTest variable to current joystick setting
	  if (resetTest > 1010){              // If reset condition exists, call reset function
	    resetGimbal();	                  // reset gimbal function
      return;                           // restart loop function
	  }

    else moveSpeed();							    	// call function to determine the speed to move gimbal

    // Based on speed determined by moveSpeed(), move gimbal
    int paL = (panVal - joyHctr);
    int piL = (pitchVal - joyVctr);

    servo1Val = myservo1.read();
    servo2Val = myservo2.read();

    Serial.print("pan servo -- ");
    Serial.print(servo1Val + panMv);
    Serial.print(" :: pitch servo -- ");
    Serial.println(servo2Val + pitchMv);
    // Serial.println();

    myservo1.write(servo1Val + panMv);                           // sets pan servo position
    myservo2.write(servo2Val + pitchMv);                          // sets pitch servo position

    delay(100);
}                                                               // End of loop()


/**
*  Test joystick position and reset gimbal if a reset has been requested (control bottom/right)
*/
int resetGimbal(){
	uint16_t i=1;                   											// Set the counter
	int resetFlag = 0;

	while( i <= 5 ){								                  		// Loop 5 times to make sure the request is real
		if ((analogRead(pan) - analogRead(pitch)) > 1010){	// Test the joysticks value
			resetFlag = 1;									                  // Set reset flag
			i++;												                      // If still in reset, increment the counter
			delay(15);										                    // Add a delay before next joystick check
			}
		else {												                      // If not in reset, break out of function
			resetFlag = 0;
			break;
			}                                       
	}                                                     // End of loop
  
	if(resetFlag = 1){                                    // Test for reset flag and center of set
		servo1Val = 90;										                  // sets the servoVal to middle
		servo2Val = 90;										                  // sets the servoVal to middle
		myservo1.write(servo1Val);                          // sets pan servo position to middle
		myservo2.write(servo2Val);                         	// sets pitch servo position to middle
	}
  delay(100);                                           // waits for the servos to get there
}                                                       // Exit resetGimbal() subroutine


/**
* Determine the speed to move gimbal based on joystick position 
*/
 void moveSpeed(){                                       
  int paL = (panVal - 512);
  int piL = (pitchVal - 512);
  // Serial.print("-- ");
  // Serial.print(paL);
  // Serial.print(" -- ");
  // Serial.println(piL);
  // delay(15);

	if (abs(paL) < 100) (panMv = 0);
	else if (abs(paL) >= 100 && abs(paL) < 200) (panMv = 1);
	else if (abs(paL) >= 200 && abs(paL) < 300) (panMv = 2);
	else if (abs(paL) >= 300 && abs(paL) < 400) (panMv = 3);
	else if (abs(paL) >= 400 && abs(paL) < 500) (panMv = 4);
	else (panMv = 5);

  if (abs(paL) != paL) panMv = ((-1)* panMv);
	
  if (abs(piL) < 100) (pitchMv = 0);
  else if (abs(piL) >= 100 && abs(piL) < 200) (pitchMv = 1);
  else if (abs(piL) >= 200 && abs(piL) < 300) (pitchMv = 2);
  else if (abs(piL) >= 300 && abs(piL) < 400) (pitchMv = 3);
  else if (abs(piL) >= 400 && abs(piL) < 500) (pitchMv = 4);
  else (pitchMv = 5);

  if (abs(piL) != piL) pitchMv = ((-1)* pitchMv);

}



