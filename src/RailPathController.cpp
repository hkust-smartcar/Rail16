/*
 * RailPathControl.cpp
 *
 *  Created on: 2016撟�7���9�
 *      Author: kwanhoman
 */

#include "RailPathController.h"

RailPathController::RailPathController()
{
}

int RailPathController::get_angle( int center_angle,
		 int l_reading,
		 int r_reading,
		 float calibrate_ratio,
		 int high_range,
		 float Kp,
		 float Ki,
		 float Kd
){
	p_reading=reading;
	p_d_reading=d_reading;
	reading=(float)(r_reading-l_reading)/(r_reading+l_reading);

	if(reading<=calibrate_ratio&&
			reading>=-calibrate_ratio&&
			l_reading>=high_range&&
			r_reading>=high_range
	){
		offset=0;
	}

	d_reading=reading-p_reading;

	offset+=Kp*d_reading+
			Ki*reading+
			Kd*(d_reading-p_d_reading);


	return center_angle+offset;
}


