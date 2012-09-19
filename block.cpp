#include "WPILib.h"

class DefaultRobot : public SimpleRobot
{ 

	DriverStation* DS;
	Jaguar* Porter;			
	Jaguar* Ports;		
	Jaguar* LeftPorter;									
	Jaguar* RightPorter;               
	Jaguar* ThirdPorter;          
	Joystick* Leftstick;
	Joystick* RightStick;	
	Joystick* OtherLeftstick;		
	Joystick* OtherRightstick;		
	Relay* Lifter;			
	Relay* Mover;		
	RobotDrive* Robot;		
	Jaguar* Cougar;
	Compressor* Air;
	Relay* Thirdmover;
	Solenoid* solarnoid;
	Solenoid* solenoid;
	Solenoid* sole;
	Solenoid* solar;
	DigitalInput* DI;
	
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
		DS = DriverStation::GetInstance();
		Robot = new RobotDrive(1, 2, 3, 4);	// create robot drive base
		//Robot->SetInvertedMotor(Robot->kFrontLeftMotor, true);
		Robot->SetInvertedMotor(Robot->kRearLeftMotor, true);
		//Robot->SetInvertedMotor(Robot->kFrontRightMotor, true);
		Robot->SetInvertedMotor(Robot->kRearRightMotor, true);
		//Porter = new Jaguar(1);
		//Ports = new Jaguar(2);
		//LeftPorter = new Jaguar(3);
		//RightPorter = new Jaguar(4);
		//ThirdPorter = new Jaguar(5);
		Leftstick = new Joystick(1);
		RightStick = new Joystick(2);
		OtherLeftstick= new Joystick(3);
		OtherRightstick = new Joystick(4);
		Air = new Compressor(1, 3);
		Air->Start();
		Lifter = new Relay(1);
		Mover = new Relay(2);
		//Cougar = new Jaguar(6);
		//Thirdmover = new Relay(3);
		solarnoid = new Solenoid(3);
		solenoid = new Solenoid(4);
		sole = new Solenoid(1);
		solar = new Solenoid(2);
		DI = new DigitalInput(1);
		


		//Update the motors at least every 100ms.
		GetWatchdog().SetExpiration(100);
		//watchdoggie makes it so if their is a problem in the program it wont run
	}

void Initialize(void)
{
	CreateRobotDrive(LEFT_MOTOR_PORT, RIGHT_MOTOR_PORT);
	SetWatchdogExpiration(0.1);
}
	
	
	void Autonomous(void)
                 //mode when robot moves on own without controls
	{
		SetWatchdogEnabled(false); //wont run is doggie doesnt work
			Drive(0.5, 0.5);
			Wait(3.5);
			Drive(-0.5, 0.5);
			Wait(1.6);
			Drive(-0.5, -0.5);
			Wait(1.10);
			Lifter(k::On);
			Wait(0.58);
			Lifter(relay::kOFF);
			Drive(0.0, 0.5);
			wait(1.6);
			Drive(1.0, 1.0);
			wait(1.3);
	}
			int i = 0;
			for (i =0; i < 5000;i++)
			{
				Watchdog().Feed();
				Robot->Drive(-0.57, 0.0);
				Wait(0.001);
			
				 
			}
			
			sole->Set(true);
		    solar->Set(false);
			
			
		}
					
				
		
		GetWatchdog().SetEnabled(true);

	}


	

	void OperatorControl(void)
	{


		while (IsOperatorControl())
		{
			Watchdog().Feed();
			              //feed doggie

            // determine if tank or arcade mode; default with no jumper is for tank drive
			if (!DS->GetDigitalIn(ARCADE_MODE) == 0) {
				Robot->TankDrive(Leftstick, RightStick);	 // drive with tank style
			}

			else {
				Robot->ArcadeDrive(Leftstick);	         // drive with arcade style (use right stick)
			}

			//move up and down
			
		
			if (OtherLeftstick->GetRawAxis(2) > 0.25){
				Lifter->Set(Relay::kForward);
			}
			else if (OtherLeftstick->GetRawAxis(2) < -0.25){
				Lifter->Set(Relay::kReverse);
			}
			else {
				Lifter->Set(Relay::kOff);
			}
							
							
							//in and out
	    	if (OtherRightstick->GetRawAxis(2) < -0.25 ){
				Mover->Set(Relay::kForward);
			}
			else if (OtherRightstick->GetRawAxis(2) > 0.25){
				Mover->Set(Relay::kReverse);
			}
			else{
				Mover->Set(Relay::kOff);
			}
							
		    if (OtherLeftstick->GetRawButton(2)){
		    	solarnoid->Set(true);
			   	solenoid->Set(false);
		    }
		    else if(OtherLeftstick->GetRawButton(1))  {
			   	solarnoid->Set(false);
			   	solenoid->Set(true);
		    }
						    
		    else {
		    	solarnoid->Set(false);
		    	solenoid->Set(false);
		   }
							
			if(OtherRightstick->GetRawButton(1)){
				sole->Set(false);
				solar->Set(true);
			}
			else if(OtherRightstick->GetRawButton(2)){
				sole->Set(true);
				solar->Set(false);	
			}
			else  {
				sole->Set(true);
				solar->Set(false);
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


