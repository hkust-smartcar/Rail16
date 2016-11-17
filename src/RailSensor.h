/*
 * RailSensor.h
 *
 *  Created on: Jun 29, 2016
 *      Author: wing_luk
 */

#pragma once

#include <libbase/k60/ftm_quad_decoder.h>

using namespace libbase::k60;

class RailSensor
{

public:

	struct Config
	{
		// For Counting
		Pin::Name a_pin;

		// Do Nothing
		Pin::Name b_pin;

		Config(void)
		:
			a_pin(Pin::Name::kDisable),
			b_pin(Pin::Name::kDisable)
		{}

		Config(Pin::Name _a_pin, Pin::Name _b_pin)
		:
			a_pin(_a_pin),
			b_pin(_b_pin)
		{}
	};

	RailSensor(const Config &config);

	uint16_t update(void);
	uint16_t getCount(void);

private:

	FtmQuadDecoder	m_ftmDecoder;
	uint16_t		m_lastCount;

};
