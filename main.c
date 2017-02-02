#pragma config(UART_Usage, UART1, uartVEXLCD, baudRate19200, IOPins, None, None)
#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in2,    Gyro,           sensorGyro)
#pragma config(Sensor, in3,    ,               sensorGyro)
#pragma config(Sensor, in4,    LiftPot,        sensorPotentiometer)
#pragma config(Sensor, in6,    Selector,       sensorPotentiometer)
#pragma config(Sensor, in7,    PowerExpander,  sensorAnalog)
#pragma config(Sensor, dgtl1,  ClawSolenoid,   sensorDigitalOut)
#pragma config(Sensor, dgtl2,  Transmission,   sensorDigitalOut)
#pragma config(Sensor, I2C_1,  REncoder,       sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_2,  LEncoder,       sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port2,           FLDrive,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           BLDrive,       tmotorVex393_MC29, openLoop, encoderPort, I2C_2)
#pragma config(Motor,  port4,           RDrive,        tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port5,           LLift,         tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           RLift,         tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port7,           LDrive,        tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           BRDrive,       tmotorVex393_MC29, openLoop, reversed, encoderPort, I2C_1)
#pragma config(Motor,  port9,           FRDrive,       tmotorVex393_MC29, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(VEX2);
#pragma competitionControl(Competition);
#include "Vex_Competition_Includes.c";
/*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*/

#include "preauton/lcd.h"
#include "preauton/lcd.c"

#include "drive/drive.h"
#include "drive/toggle.h"
#include "drive/controls.c"
#include "drive/functions.c"

#include "PID/PID.h"
#include "PID/PID.c"

#include "auton/constants.h"
#include "auton/functions.h"
#include "auton/auton.h"

void pre_auton() {
	calibrate();
	startTask(LCD);
}

task autonomous() {
	backFenceLeft();
}

task usercontrol() {
	Toggle claw;
	claw.state = 0;
	claw.buffer = 0;

	startTask(controls);
	startTask(lift);

	while (true) {
		// arcade drive
		moveDrive(c.LDrive, c.RDrive);

		// claw toggle.
		if (c.claw && (c.claw != claw.buffer))  // if button is pressed and it was not pressed before
	   	claw.state = claw.state ? 0 : 1;

		claw.buffer = c.claw;
		clawState(claw.state);

		// transmission
		if (c.transmissionOn)
			transmissionState(OPEN);

		if (c.transmissionOff)
			transmissionState(CLOSED);

		if (vexRT[Btn8L])
			startTask(deploy);

		wait1Msec(20);
	}
}
// 680 - lift down state, position needed for turns
