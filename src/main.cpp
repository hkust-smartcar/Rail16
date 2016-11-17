#include "railv1.h"
#include <libutil/pGrapher.h>
#include <libsc/ldc1000.h>

namespace libbase
{
	namespace k60
	{

		Mcg::Config Mcg::GetMcgConfig()
		{
			Mcg::Config config;
			config.external_oscillator_khz = 50000;
			config.core_clock_khz = 150000;
			return config;
		}

	}
}

using namespace libsc;
using namespace libbase::k60;
using namespace libutil;

int main(void)
{
  	Timer::TimerInt t = 0;

	System::Init();
	Railcar run;
	run.config();
//	run.checkReading();

	pGrapher pg;
	pg.addWatchedVar(&run.f1,"f1");
	pg.addWatchedVar(&run.f2,"f2");
	pg.addWatchedVar(&run.f3,"f3");

////	pg.addWatchedVar(&run.leftSensor, "leftSensor");
////	pg.addWatchedVar(&run.rightSensor, "rightSensor");
//	pg.addWatchedVar(&run.middleSensor,"middleSensor");
	pg.addWatchedVar(&run.tError,"error");
//	pg.addWatchedVar(&run.tPreError, "tPreError");
	pg.addWatchedVar(&run.threshold, "threshold");
//	pg.addWatchedVar(&run.tError, "tError");

//	pg.addWatchedVar(&run.leftSensorP, "lPercent");
//	pg.addWatchedVar(&run.rightSensorP, "rPercent");
//	pg.addWatchedVar(&run.middleSensorP, "mPercent");
//	pg.addWatchedVar(&run.tError, "tError");
//	pg.addWatchedVar(&run.middleSensor,"middleSensor");
//	pg.addWatchedVar(&run.rightMean, "rightMean");
//	pg.addWatchedVar(&run.middleMean, "middleSensor");
//	pg.addWatchedVar(&run.middleOut, "middleOut");
//	pg.addWatchedVar(&run.temp1,"temp1");
//	pg.addWatchedVar(&run.temp2,"temp2");
//	pg.addWatchedVar(&run.plusValue,"plus");
//	pg.addWatchedVar(&run.crossValue,"cross");
//	pg.addWatchedVar(&run.leftMin,"leftMin");
//	pg.addWatchedVar(&run.rightMin,"rightMin");
//	pg.addWatchedVar(&run.rightMax, "rightMax");
//	pg.addWatchedVar(&run.angle,"angle");
//	pg.addWatchedVar(&run.tPreError,"tPreError");


	pg.addSharedVar(&run.tKp,"tKp");
	pg.addSharedVar(&run.tKd,"tKd");
	pg.addSharedVar(&run.runSpeed, "speed");
	pg.addSharedVar(&run.turningMiddle,"turningMiddle");

//	pg.addSharedVar(&run.MaxBound,"MaxBound");
//	pg.addSharedVar(&run.MinBound,"MinBound");
	int i = 0;
	int j = 0;
	int k = 0;
	int a = 0;

	while (1){
		if (t == System::Time()) continue;
		t = System::Time();

		if(t % 10 == 0){
			run.collectSensorData();
			run.filtering();

			if(!run.Start){
				if(t % 20 == 0){
					if(run.button1 -> IsDown() && (!run.Map1)){
						if(i < 5){
							run.leftMax += run.leftSensor;
							i++;
						}
						if(i == 5){
							run.leftMax /= 5;
							run.Led1->Switch();
							run.Map1 = 1;
						}
					}
					if(run.button2 -> IsDown() && run.Map1 && (!run.Map2)){
						if(j < 5){
							run.rightMax += run.rightSensor;
							j++;
						}

						if(j == 5){
							run.rightMax /= 5;
							run.Led1->Switch();
							run.Map2 = 1;
						}
					}
					if(run.button1 -> IsDown() && run.Map2 ==1 && (!run.Map3)){
						if(k< 5){
							run.middleMax += run.middleSensor;
							k++;
						}
						if(k == 5){
							run.middleMax /= 5;
							run.Led2->Switch();
							run.Map3 = 1;
						}
					}
					if(run.button2 -> IsDown() && run.Map3 ==1 && (!run.Map4)){
						if(a< 5){
							run.middleMin += run.middleSensor;
							run.leftMin += run.leftSensor;
							run.rightMin += run.rightSensor;
							a++;
						}
						if(a == 5){
							run.middleMin /= 5;
							run.leftMin /= 5;
							run.rightMin /= 5;
							run.mappingHAHA();
							run.Map4 = 1;
							run.IsMap = 1;
						}
					}
					if(run.button1 -> IsDown() && run.IsMap == 1){
						System::DelayS(3);
						run.Start = 1;
					}
					pg.sendWatchData();
				}
			}



			if(t % 20 == 0){
				if(run.Start){
				run.middleInTheFront();
				run.motor->SetPower(run.runSpeed);
				pg.sendWatchData();
			}
		}

	}
	}
		return 0;

	}
