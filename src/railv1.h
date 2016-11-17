#ifndef __RAILV1_HPP__
#define __RAILV1_HPP__

#include <cassert>
#include <cstdint> // uint16_t
#include <cstring>
#include <cmath>
#include <array> // std::array
#include <libbase/k60/mcg.h>
#include <libsc/system.h>
#include <libsc/st7735r.h>  // LCD
#include <libsc/lcd_typewriter.h>// LCD WriteString
#include <libsc/lcd_console.h>
#include <libsc/mini_lcd.h>
#include <libbase/k60/adc.h> // sensor
#include <libsc/futaba_s3010.h> // servo
#include <libsc/joystick.h>//joystick
#include <libsc/simple_buzzer.h>//buzzer
#include <libsc/button.h>//button
#include <libbase/k60/gpio.h>
#include <libsc/led.h>//led
#include <libsc/dir_motor.h>
#include <libsc/ab_encoder.h>
#include <sstream>
#include <string>
#include <libutil/kalman_filter.h>
#include <libutil/pGrapher.h>
#include <libsc/ldc1000.h>


class Railcar{
public:


	bool Start = false;
	bool MotorEnable = false;

	//mapping
	float plusValue = 0;
	float crossValue = 1;

	//steering control
	float tKp = 0.065;
	int16_t runSpeed = 270;
	float tKi = 0;
	float tKd = 0;
	float tError = 0;
	float tPreError = 0;
//	float tKpl = 1.065;
//	float tKil = 0;
//	float tKdl = 0;
//	float MaxBound = 200;
//	float MinBound = 200;
//	float MaxReading = rightMax - MaxBound;//16600- 185000
//	float MinReading = rightMin + MinBound; //13000- 133000

	float angle = 0;
	uint32_t frequency = 0;
	uint16_t data = 0;

	//filtering
	uint16_t f1 = 0;
	uint16_t f2 = 0;
	uint16_t f3 = 0;
	uint16_t f1a[10]={0};
	uint16_t f2a[10]={0};
	uint16_t f3a[10]={0};
	int count=0;



	float turningMiddle = 1040;
	float turningMax = turningMiddle + 210;
	float turningMin = turningMiddle - 210;
	float leftTemp =0;
	float leftMin = 0;
	float leftMax = 0;
	float rightMin = 0;
	float rightMax = 0;
	float leftRange = 0;
	float rightRange = 0;
	float middleRange = 0;
	float middleMin = 0;
	float middleMax = 0;
	float middleThreshold = 0;
	float threshold = 0;


	float noise = 300;

	//speed control
	float mKp = 1;
	float mKi = 0;
	float mKd = 0;
	float mError = 0;
	float mPreError = 0;
	float mPrePreError = 0;
	float targetSpeed = 60;
//	int16_t runSpeed = 250;
	int32_t realSpeed = 0;
	float low = 5;
	float high = 35;
	float LeftInMiddle = 0;
	float RightInMIddle = 0;


	float mappingTemp = 0;
	float mappingTemp2 = 0;
	float crossValue2 = 1;
	float plusValue2 =0;

	float leftSensor = 0;
	float rightSensor = 0;
	float middleSensor = 0;
	float prevL=0;
	float prevR=0;
	float prevM=0;
	float q=35;
	float r=100-q;


	bool leftLarge = 0;
	bool rightLarge = 0;
	bool rightOut = 0;
	bool leftOut = 0;

	bool IsMap = 0;



//mapping2
	float rightSensorP = 0;
	float leftSensorP = 0;
	float middleSensorP = 0;

	bool tELeft = 0;
	bool tERight = 0;

	bool Map1 = 0;
	bool Map2 = 0;
	bool Map3 = 0;
	bool Map4 = 0;

	bool middleOut = 0;

	float Ldc1000Right;
	float Ldc1000Left;
	float Ldc1000Middle;

//	int *i = 0;
//	int *j = 0;

	libsc::Led* Led0;
	libsc::Led* Led1;
	libsc::Led* Led2;
	libsc::Led* Led3;
	libsc::AbEncoder* encoder;
	libsc::FutabaS3010* servo;
	libsc::DirMotor* motor;
	libsc::Button* button1;
	libsc::Button* button2;
	libsc::Joystick* joystick;
	libsc::SimpleBuzzer* buzzer;
//	libutil::KalmanFilter* Ldc1000Right;
//	libutil::KalmanFilter* Ldc1000Left;
//	libutil::KalmanFilter* Ldc1000Middle;
	libsc::Ldc1000 *Ldc1000_0;
	libsc::Ldc1000 *Ldc1000_1;
	libsc::Ldc1000 *Ldc1000_2;

	void steering();
	void middleInTheFront();
	void steering2();

	void speedControl();
	void config();
	void collectSensorData();
	void getThreshold();
	void filtering();
	void mappingHAHA();
//	void SavePrevReading();

//	float leftSensorA[10] = 0;
//	float middleSenaorA[10] = 0;
//	float rightSensorA[10] = 0;
};

#endif
