#include "WPILib.h"

class DefaultRobot : public SimpleRobot
{

	DriverStation* ds;
	Jaguar* leftmotor;
	Jaguar* rightmotor;
	Jaguar* gyro;
	Jaguar* test;
	Jaguar* shooter;
	Joystick* leftStick;
	Joystick* rightStick;
	Joystick* thirdStick;
	Joystick* fourthStick;
	Relay* armlift;
	Relay* armmove;
	RobotDrive* myRobot;
	Jaguar* sally;
	Compressor* compressor;
	Relay* compress;
	Solenoid* servo;
	Solenoid* servor;
	Solenoid* sole;
	Solenoid* soler;
	DigitalInput* limit;

//always put in code
	enum JUMPERS					// Driver Station jumpers to control program operation
	{ ARCADE_MODE = 1,				// Tank/Arcade jumper is on DS Input 1 (Jumper present is arcade)
	  DISABLE_AUTONOMOUS = 2,		// Autonomous/Teleop jumper is on DS Input 2 (Jumper absent is autonomous)
	  AUTO_TURN_LEFT = 4			// putting jumper on 4 makes robot turn left, not right, in autonomous mode
	};


public:

	void TurnRight(int);				// turn right <angle> degrees
	void TurnLeft(int);					// turn left <angle> degrees
	void SensorDrive(int, float, bool);	// drive certain distance (IN FEET) forwards or backwards using gyro and geartooth sensors


	DefaultRobot(void)  //labels what is pluged into where with the name you gave it
	{
		ds = DriverStation::GetInstance();
		myRobot = new RobotDrive(1, 2, 3, 4);	// create robot drive base
		myRobot->SetInvertedMotor(myRobot->kFrontLeftMotor, true);
		//myRobot->SetInvertedMotor(myRobot->kRearLeftMotor, true);
		//myRobot->SetInvertedMotor(myRobot->kFrontRightMotor, true);
		myRobot->SetInvertedMotor(myRobot->kRearRightMotor, true);
		leftmotor = new Jaguar(1);
		rightmotor = new Jaguar(2);
		gyro = new Jaguar(3);
		test = new Jaguar(4);
		shooter = new Jaguar(5);
		leftStick = new Joystick(1);
		rightStick = new Joystick(2);
		thirdStick= new Joystick(3);
		fourthStick = new Joystick(4);
		compressor = new Compressor(1, 3);
		compressor->Start();
		armlift = new Relay(1);
		armmove = new Relay(2);
		sally = new Jaguar(6);
		compress = new Relay(3);
		servo = new Solenoid(8, 3);
		servor = new Solenoid(8, 4);
		sole = new Solenoid(8, 1);
		soler = new Solenoid(8, 2);
		limit = new DigitalInput(1);



		//Update the motors at least every 100ms.
		GetWatchdog().SetExpiration(100);
		//watchdoggie makes it so if their is a problem in the program it wont run
	}


	void Autonomous(void)
                 //mode when robot moves on own without controls
	{
		GetWatchdog().SetEnabled(true); //wont run is doggie doesnt work

		{
			Watchdog().Feed();
		 compressor->Start();
			Wait(0.1);
			armmove->Set(Relay::kForward);
			Wait(0.58);
			armmove->Set(Relay::kOff);
			Wait(0.1);
			armlift->Set(Relay::kForward);
			Wait(4.0);
			armlift->Set(Relay::kOff);
			Wait(0.1);
			servo->Set(false);
			servor->Set(true);
			Wait(0.1);
			sole->Set(false);
			soler->Set(true);
			Wait(0.5);
			armmove->Set(Relay::kForward);
			Wait(1.5);
			armlift->Set(Relay::kForward);
			Wait(2.4);
			armmove->Set(Relay::kOff);
			armlift->Set(Relay::kOff);


			for (int i =0; i < 5000;i++)
			{
				Watchdog().Feed();
				myRobot->Drive(-0.57, 0.0);
				Wait(0.001);


			}

			sole->Set(true);
		    soler->Set(false);




				}

		GetWatchdog().SetEnabled(false);

	}




	void OperatorControl(void)
	{


		while (IsOperatorControl())
		{
			Watchdog().Feed();
			              //feed doggie

            // determine if tank or arcade mode; default with no jumper is for tank drive
			if (!ds->GetDigitalIn(ARCADE_MODE) == 0) {
				myRobot->TankDrive(leftStick, rightStick);	 // drive with tank style
			}

			else {
				myRobot->ArcadeDrive(leftStick);	         // drive with arcade style (use right stick)
			}

			//move up and down


			if (thirdStick->GetRawAxis(2) > 0.25){
				armmove->Set(Relay::kForward);
			}
			else if (thirdStick->GetRawAxis(2) < -0.25){
				armmove->Set(Relay::kReverse);
			}
			else {
				armmove->Set(Relay::kOff);
			}


							//in and out
	    	if (fourthStick->GetRawAxis(2) < -0.25 ){
				armlift->Set(Relay::kForward);
			}
			else if (fourthStick->GetRawAxis(2) > 0.25){
				armlift->Set(Relay::kReverse);
			}
			else{
				armlift->Set(Relay::kOff);
			}

		    if (thirdStick->GetRawButton(2)){
		    	servo->Set(true);
			   	servor->Set(false);
		    }
		    else if(thirdStick->GetRawButton(1))  {
			   	servo->Set(false);
			   	servor->Set(true);
		    }

		    else {
		    	servo->Set(false);
		    	servor->Set(false);
		   }

			if(fourthStick->GetRawButton(1)){
				sole->Set(false);
				soler->Set(true);
			}
			else if(fourthStick->GetRawButton(2)){
				sole->Set(true);
				soler->Set(false);
			}
			else  {
				sole->Set(true);
				soler->Set(false);
			}

















		}
	}
};


/* SensorDrive(Distance, Speed, Autonomous?): Drives robot straight
 * forward a set distance, uses gyro sensor to keep on course, uses geartooth sensor
 * @param Autonomous? : If this function is being called from an autonomous routine, this parameter should be true.
 *		Otherwise, it should be false. NOTE: This is used to immediately break out of the function if mode is
 *		switched to teleop. If this parameter is true and the mode is set to teleoperated, then the code will not execute.
 */


START_ROBOT_CLASS(DefaultRobot);


