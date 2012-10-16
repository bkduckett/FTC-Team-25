#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  none)
#pragma config(Sensor, S2,     compassSensor,  sensorHiTechnicIRSeeker1200)
#pragma config(Sensor, S3,     lightSensor,    sensorLightActive)
#pragma config(Sensor, S4,     sonarSensor,    sensorSONAR)
#pragma config(Motor,  mtr_S1_C1_1,     motorD,        tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     motorE,        tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     motorF,        tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     motorG,        tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     motorH,        tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C3_2,     motorI,        tmotorTetrix, openLoop, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*+++++++++++++++++++++++++++++++++++++++++++++| Notes |++++++++++++++++++++++++++++++++++++++++++++++
TETRIX Scissorbot (Compass) - Remote Control
- NOTE: Scissorbot is best at picking up rings when the gripper PARALLEL to the ring.
- This program allows you to remotely control your robot using the USB Logitech Dual
  Action Joysticks.
- This program also ignores low values that would cause your robot to move when the joysticks fail to
  return back to exact center.  You may need to adjust the 'threshold' value to get it just right.
- For USB values to be transmitted to the robot, you must follow several steps:
  - Use the "#include "JoystickDriver.c"" line at the beginning of your program
  - Call the "getJoystickSettings(joystick);" in a loop to repeatedly retrieve the latest values
  - Open the Robot > Debugger Windows > "Joystick Control - Basic" window after downloading your
    program to the virtual robot
----------------------------------------------------------------------------------------------------*/

#pragma debuggerWindows("joystickSimple");
#include "JoystickDriver.c"

task main()
{
  //Integer variable that allows you to specify a "deadzone" where values (both positive or negative)
  //less than the threshold will be ignored.
  int threshold = 10;

  //Loop Forever
  while(1 == 1)
  {
    //Get the Latest joystick values
    getJoystickSettings(joystick);

    //Driving Control
    if(abs(joystick.joy1_y2) > threshold)     // If the right analog stick's Y-axis readings are either above or below the threshold...
    {
      motor[motorD] = joystick.joy1_y2;				// ...move the right side of the robot.
      motor[motorF] = joystick.joy1_y2;
    }
    else                                      // Else the readings are within the threshold, so...
    {
      motor[motorD] = 0;											// ...stop the right side of the robot.
      motor[motorF] = 0;
    }
    if(abs(joystick.joy1_y1) > threshold)     // If the left analog stick's Y-axis readings are either above or below the threshold...
    {
      motor[motorE] = joystick.joy1_y1;				// ...move the left side of the robot.
      motor[motorG] = joystick.joy1_y1;
    }
    else                                      // Else the readings are within the threshold, so...
    {
      motor[motorE] = 0;											// ...stop the left side of the robot.
      motor[motorG] = 0;
    }

		//Scissor Lift Control
    if(joy1Btn(6) == 1)       // If button 6 (RB) is pressed...
    {
      motor[motorH] = 70;     // ...raise the scissor lift.
      motor[motorI] = 70;
    }
    else if(joy1Btn(8) == 1)  // Else, if button 8 (RT) is pressed...
    {
      motor[motorH] = -70;    // ...lower the scissor lift.
      motor[motorI] = -70;
    }
    else                      // Else (neither button is pressed)...
    {
     	motor[motorH] = 0;      // ...stop the scissor lift.
      motor[motorI] = 0;
    }

    //Wrist Motor Control
    if(joy1Btn(5) == 1)       // If button 5 (LB) is pressed...
    {
      motor[motorB] = 75;     //...rotate the gripper up.
    }
    else if(joy1Btn(7) == 1)  // Else, if button 7 (LT) is pressed...
    {
      motor[motorB] = -75;    // ...rotate the gripper down.
    }
    else                      // Else (neither button is pressed)...
    {
      motor[motorB] = 0;      // ...stop rotating the gripper.
    }

    //Gripper Motor Control
    if(joy1Btn(1) == 1)       // If button 1 (X) is pressed...
    {
      motor[motorA] = 50;     // ...open the gripper.
    }
    else if(joy1Btn(2) == 1)  // Else, if button 2 (A) is pressed...
    {
      motor[motorA] = -50;    // ...close the gripper.
    }
    else                      // Else (neither button is pressed)...
    {
      motor[motorA] = 0;      // ...stop the gripper.
    }

  }
}
