#pragma region VEXcode Generated Robot Configuration
// Make sure all required headers are included.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>


#include "vex.h"

using namespace vex;

// Brain should be defined by default
brain Brain;


// START V5 MACROS
#define waitUntil(condition)                                                   \
  do {                                                                         \
    wait(5, msec);                                                             \
  } while (!(condition))

#define repeat(iterations)                                                     \
  for (int iterator = 0; iterator < iterations; iterator++)
// END V5 MACROS


// Robot configuration code.
motor LeftDriveSmart = motor(PORT1, ratio18_1, true);
motor RightDriveSmart = motor(PORT2, ratio18_1, false);
drivetrain Drivetrain = drivetrain(LeftDriveSmart, RightDriveSmart, 319.19, 295, 40, mm, 1);

motor wingLeft = motor(PORT10, ratio18_1, false);

motor wingRight = motor(PORT4, ratio18_1, true);

controller Controller1 = controller(primary);



// Helper to make playing sounds from the V5 in VEXcode easier and
// keeps the code cleaner by making it clear what is happening.
void playVexcodeSound(const char *soundName) {
  printf("VEXPlaySound:%s\n", soundName);
  wait(5, msec);
}



// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;
// define variables used for controlling motors based on controller inputs
bool DrivetrainNeedsToBeStopped_Controller1 = true;

// define a task that will handle monitoring inputs from Controller1
int rc_auto_loop_function_Controller1() {
  // process the controller input every 20 milliseconds
  // update the motors based on the input values
  while(true) {
    if(RemoteControlCodeEnabled) {
      
      // calculate the drivetrain motor velocities from the controller joystick axies
      // left = Axis3 + Axis4
      // right = Axis3 - Axis4
      int drivetrainLeftSideSpeed = Controller1.Axis3.position() + Controller1.Axis4.position();
      int drivetrainRightSideSpeed = Controller1.Axis3.position() - Controller1.Axis4.position();
      
      // check if the values are inside of the deadband range
      if (abs(drivetrainLeftSideSpeed) < 5 && abs(drivetrainRightSideSpeed) < 5) {
        // check if the motors have already been stopped
        if (DrivetrainNeedsToBeStopped_Controller1) {
          // stop the drive motors
          LeftDriveSmart.stop();
          RightDriveSmart.stop();
          // tell the code that the motors have been stopped
          DrivetrainNeedsToBeStopped_Controller1 = false;
        }
      } else {
        // reset the toggle so that the deadband code knows to stop the motors next time the input is in the deadband range
        DrivetrainNeedsToBeStopped_Controller1 = true;
      }
      
      // only tell the left drive motor to spin if the values are not in the deadband range
      if (DrivetrainNeedsToBeStopped_Controller1) {
        LeftDriveSmart.setVelocity(drivetrainLeftSideSpeed, percent);
        LeftDriveSmart.spin(forward);
      }
      // only tell the right drive motor to spin if the values are not in the deadband range
      if (DrivetrainNeedsToBeStopped_Controller1) {
        RightDriveSmart.setVelocity(drivetrainRightSideSpeed, percent);
        RightDriveSmart.spin(forward);
      }
    }
    // wait before repeating the process
    wait(20, msec);
  }
  return 0;
}

task rc_auto_loop_task_Controller1(rc_auto_loop_function_Controller1);

#pragma endregion VEXcode Generated Robot Configuration

// ----------------------------------------------------------------------------
//                                                                            
//    Project:                                               
//    Author:
//    Created:
//    Configuration:        
//                                                                            
// ----------------------------------------------------------------------------

// Include the V5 Library
#include "vex.h"

// Allows for easier use of the VEX Library
using namespace vex;

// Begin project code

bool setVelocityMotors = false;

bool wingRightState = false;
bool wingLeftState = false;;

float wingLeftProtect = 0;
float wingRightProtect = 0;

void setWingPostion(motor &wingMotor, bool &wingState){

  wingMotor.setStopping(hold);

  if(wingState){
    wingMotor.spinToPosition(0, degrees);
    wingState = false;
  }
  else{
    wingMotor.spinToPosition(90, degrees); 
    wingState = true;
  }
}

void wingBothControl(){

  int wingBothVelocity = Controller1.Axis2.position();

  wingLeft.setVelocity(wingBothVelocity, percent);
  wingRight.setVelocity(wingBothVelocity, percent);

  if(wingBothVelocity > 0){

    wingLeft.spin(forward);
    wingRight.spin(forward);
    
  }else if(wingBothVelocity < 0){

    wingLeft.spin(reverse);
    wingRight.spin(reverse);

  }
}

// -> Controla las alas con los botones | función | opción 1

// void wingBothControl(int movement){

//   wingLeft.setVelocity(100, percent);
//   wingRight.setVelocity(100, percent);

//   if(movement == 1){

//     wingLeft.spin(forward);
//     wingRight.spin(forward);
    
//   }else if(movement == 2){

//     wingLeft.spin(reverse);
//     wingRight.spin(reverse);

//   }
// }



void userControl(void) {
  
  Brain.Screen.clearScreen();

  while (true) {
    wait(20, msec);
    wingLeft.setStopping(hold);
    wingRight.setStopping(hold);

    wingLeftProtect = wingLeft.current(amp);
    wingRightProtect = wingRight.current(amp);
    

    // -> Controla las alas con los botones | opción 1

    // if(Controller1.ButtonUp.pressing()){
    //   wingBothControl(1);
    // }else if(Controller1.ButtonDown.pressing()){
    //   wingBothControl(2);
    // }else{
    //   wingLeft.setStopping(hold);
    //   wingRight.setStopping(hold);

    // }


    if(wingRightProtect < 2.13 || wingLeftProtect <  2.13){

      if(Controller1.ButtonL2.pressing()){
        setWingPostion(wingLeft, wingLeftState);
      }
      else if(Controller1.ButtonR2.pressing()){
        setWingPostion(wingRight, wingRightState);
      }
      
      Controller1.Axis2.changed(wingBothControl);
    }
    else{
      wingLeft.setStopping(coast);
      wingRight.setStopping(coast);

      wingLeft.stop();
      wingRight.stop();

      wait(1, seconds);
    }

  }
}


void preAutonomous(void) {
  // actions to do when the program starts
  Brain.Screen.clearScreen();
  Brain.Screen.print("pre auton code");
  wait(1, seconds);
}

void autonomous(void) {
  Brain.Screen.clearScreen();
  Brain.Screen.print("autonomous code");
  // place automonous code here
}
int main() {
  // create competition instance
  competition Competition;

  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(userControl);

  // Run the pre-autonomous function.
  preAutonomous();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}