#pragma once
#include "Arduino.h"
#include "SbcInterface.h"

struct SbcStatus{
float m_channel1=0;
float m_channel2=0;
float m_channel3=0;
float m_channel4=0;
int8_t m_masterVol=0;
int8_t m_slaveVol=0;
SbcInterface::ConnectState m_conState=SbcInterface::ConnectState::disconnect;
};