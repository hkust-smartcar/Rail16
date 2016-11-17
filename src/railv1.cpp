#include "railv1.h"

using namespace libsc;
using namespace libbase::k60;
using namespace libutil;

void Railcar::config(){
	Led0 = new Led({0, true});
	Led0->SetEnable(false);

	Led1 = new Led({1, true});
	Led1->SetEnable(false);

	Led2 = new Led({2,true});
	Led2->SetEnable(false);

	Led3 = new Led({3,true});
	Led3->SetEnable(false);

	FutabaS3010::Config servo_config;
	servo_config.id = 0;
	servo = new FutabaS3010(servo_config);
	servo->SetDegree(turningMiddle);

	DirMotor::Config motor_config;
	motor_config.id = 0;
	motor = new DirMotor(motor_config);
	motor->SetPower(0);
	motor->SetClockwise(false);

	Button::Config ButtonConfig;
	ButtonConfig.id=0;
	ButtonConfig.is_active_low=true;
	Button::Config ButtonConfig1;
	ButtonConfig1.id=1;
	ButtonConfig1.is_active_low=true;
	button1 = new Button(ButtonConfig);
	button2 = new Button(ButtonConfig1);

//	joystick = new Joystick(GetJoystickConfig());

	SimpleBuzzer::Config buzzerConfig;
	buzzerConfig.id = 0;
	buzzerConfig.is_active_low = true;
	buzzer = new SimpleBuzzer(buzzerConfig);

//
//	AbEncoder::Config AbEncoder1;
//	encoder = new AbEncoder(AbEncoder1);

//id1 == spi

	//id=0, spi0
	//id=1, spi2
	//id=2, spi1, lcd

	//Ldc1000_0 = new Ldc1000({1});//f1			//Kitty: Need checking	// Ldc1000_2 swapped with Ldc1000_1
	Ldc1000_2 = new Ldc1000({1});//0//f2//LCD
	Ldc1000_1 = new Ldc1000({0});//2//f3//right
	Ldc1000_0 = new Ldc1000({2});//f1//left
}

void Railcar::collectSensorData(){
	Ldc1000_0->Update();
	Ldc1000_1->Update();
	Ldc1000_2->Update();
	f1 = Ldc1000_0->GetData();
	f2 = Ldc1000_1->GetData();
	f3 = Ldc1000_2->GetData();
}
void Railcar::filtering(){

//	middleSensor = middleSensor - middleMin;
//	leftSensor = crossValue* leftSensor + plusValue - middleMin;
//	rightSensor = crossValue2* rightSensor + plusValue2 - middleMin;

	for(int i=0; i<9;i++){
			f2a[i]=f2a[i+1];
			f1a[i]=f1a[i+1];
			f3a[i]=f3a[i+1];
	}
	f2a[9]=f2;
	f3a[9]=f3;
	f1a[9]=f1;
//	if(count<10){//TODO
//		count++;
//		f2a[9]=f2;
//		f3a[9]=f3;
//		f1a[9]=f1;
//	}
//	if(count>=10){
//		if(abs(f1-prevL)<1000)f1a[9]=f1;
//		else f1a[9]=prevL;
//		if(abs(f2-prevR)<1000)f2a[9]=f2;
//		else f2a[9]=prevR;
//		if(abs(f3-prevM)<1000)f3a[9]=f3;
//		else f3a[9]=prevM;
//	}
	rightSensor=0;
	leftSensor=0;
	middleSensor=0;
	for(int i=0; i<10;i++){
		rightSensor+=f2a[i];
		middleSensor+=f3a[i];
		leftSensor+=f1a[i];
	}
	rightSensor/=10;
	leftSensor/=10;
	middleSensor/=10;

	leftSensor = crossValue* leftSensor + plusValue;
	rightSensor = crossValue2* rightSensor + plusValue2;


}
void Railcar::mappingHAHA(){
	rightRange = rightMax - rightMin;
	middleRange = middleMax - middleMin;
	leftRange = leftMax - leftMin;

	crossValue = middleRange/leftRange;
	mappingTemp = crossValue * leftMin;
	plusValue = middleMin - mappingTemp;

	crossValue2 = middleRange/rightRange;
	mappingTemp2 = crossValue2 * rightMin;
	plusValue2 = middleMin - mappingTemp2;

	threshold = middleMin + middleRange/150;


//	rightMax=rightMax*crossValue2+plusValue2;
//	rightMin=rightMin*crossValue2+plusValue2;
//	leftMax=leftMax*crossValue+plusValue;
//	leftMin=leftMin*crossValue+plusValue;


//	Start = true;//delete it later;
	Led2 ->Switch();
}
void Railcar::speedControl(){
	encoder->Update();
	realSpeed = encoder->GetCount();
//	mPrePreError = mPreError;
	mPreError = mError;
	mError = targetSpeed - realSpeed;
	runSpeed += (int16_t)(mKp *( mError - mPreError)+ mKi * (mError));
	motor->SetPower(runSpeed);

}
//
void Railcar::steering(){
	tPreError = tError;

//	if(middleSensor >= middleRange/2+ middleMin){
//		middleOut = 0;
//	}
	if(leftSensor <= middleMin + 500
		&& rightSensor <= middleMin + 500
		&& middleSensor <= middleMin + 500){
		tError = tPreError;
		Led0->SetEnable(true);

		if(tError < 0 && (!rightLarge)){
			angle = turningMin;//right large turn
			leftLarge = 1;
		}
		else if(tError>0 && (!leftLarge)){
			angle = turningMax;//left large turn
			rightLarge = 1;
		}
		else{
			Led3->SetEnable(true);//default
		}

//out of the road
//		middleOut = 1;
//		Led0->SetEnable(middleOut);


	}
//	else if(middleOut){
//		tError = tPreError;
//		Led0->SetEnable(middleOut);
//	}
	else if(leftSensor > rightSensor){
		tError = middleMax - middleSensor;
		leftLarge = 0;
		rightLarge = 0;
		Led0->SetEnable(false);
//		Led0->SetEnable(middleOut);
//			tError =  middleRange - middleSensor;
	}
	else if(rightSensor > leftSensor){
		tError = middleSensor - middleMax;
		leftLarge = 0;
		rightLarge = 0;
		Led0->SetEnable(false);

//		Led0->SetEnable(middleOut);
//			tError = -(middleRange - middleSensor);
	}
	else{
	tError = 0;
	leftLarge = 0;
	rightLarge = 0;
	Led0->SetEnable(false);
	 }

	angle = turningMiddle +(tKp*tError + tKd*(tError - tPreError));

	if(angle > turningMax)	angle = turningMax;
	if(angle < turningMin )	angle = turningMin;

	servo->SetDegree(angle);
}
//
void Railcar::middleInTheFront(){
	tPreError = tError;
	//error > 0 turn left
	//error < 0 turn right


	if(middleSensor <= threshold){
		//turning right
		if(tPreError < 0){
			if(rightSensor <= threshold
					&& leftSensor <= threshold
					&& (!leftOut)){
						rightOut = 1;
						Led1 -> SetEnable(true);
			}
//			rightSensor = rightMax + rightMax - rightSensor;
			rightSensor = middleMax + middleMax - rightSensor;
			Led0 -> SetEnable(true);
			Led1 -> SetEnable(false);
			//if 3 sensors are out and turning right previously

		}
		//turning left
		//if 3 sensors are out and turning left previously
		if(tPreError > 0){
			if(rightSensor <= threshold
				&& leftSensor <= threshold
				&& (!rightOut)){
				leftOut = 1;
				Led3 -> SetEnable(true);
			}
//			leftSensor = leftMax + leftMax - leftSensor;
			leftSensor = middleMax + middleMax - leftSensor;
			Led2 -> SetEnable(true);
			Led3 -> SetEnable(false);


		}
	}
	else{
		leftOut = 0;
		rightOut = 0;
		Led0 -> SetEnable(false);
		Led1 -> SetEnable(false);
		Led2 -> SetEnable(false);
		Led3 -> SetEnable(false);
	}
	tError = leftSensor - rightSensor;

	angle = turningMiddle +(tKp*tError + tKd*(tError - tPreError));


	if(angle > turningMax)	angle = turningMax;
	if(angle < turningMin )	angle = turningMin;

	servo->SetDegree(angle);
}

