/*
 * RailPathController.h
 *
 *  Created on: 2016撟�7���9�
 *      Author: kwanhoman
 */

#ifndef SRC_RAILPATHCONTROLLER_H_
#define SRC_RAILPATHCONTROLLER_H_


class RailPathController {
private:
	float p_reading=0;
	float reading=0;
	float p_d_reading=0;
	float d_reading=0;
	int offset=0;
public:
	RailPathController();
	int get_angle(int center_angle,
			int l_reading,
			int r_reading,
			float calibrate_ratio,
			int high_range,
			float Kp,
			float Ki,
			float Kd);

};

#endif /* SRC_RAILPATHCONTROLLER_H_ */
