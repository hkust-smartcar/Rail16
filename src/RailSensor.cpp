/*
 * RailSensor.cpp
 *
 *  Created on: Jun 29, 2016
 *      Author: wing_luk
 */

#include "RailSensor.h"

using namespace libbase::k60;

FtmQuadDecoder::Config getFtmDecoderConfig(const RailSensor::Config &config)
{
	FtmQuadDecoder::Config ftmConfig;
	ftmConfig.a_pin = config.a_pin;
	ftmConfig.b_pin = config.b_pin;
	ftmConfig.encoding_mode = FtmQuadDecoder::Config::EncodingMode::kCountDirection;
	return ftmConfig;
}

RailSensor::RailSensor(const Config &config)
:
	m_ftmDecoder(getFtmDecoderConfig(config)),
	m_lastCount(0)
{}

uint16_t RailSensor::update(void)
{
	m_lastCount = m_ftmDecoder.GetCount();
	m_ftmDecoder.ResetCount();
	return m_lastCount;
}

uint16_t RailSensor::getCount(void)
{
	return m_lastCount;
}
