#pragma once
#include <vector>
#include "CalChannelValue.h"
#include "GetCtrlValue.h"
#include "ArduinoJson.h"
#include "SbcInterface.h"

class sbc :public SbcInterface
{
private:
    CalChannelValue calV;
    GetCtrlValue getV;
    bool OnlyDebugOnceInitValue = true;
    GetCtrlValue::CtrlType m_lastCtrlType=GetCtrlValue::CtrlType::error;
    bool m_revInitValueR=true;
    bool m_revOpenValueR=true;
    bool m_revCloseValueR=true;
    bool m_revHardCtrlValueR=true;
private:
    void outInitValue();
    void pong() override;
public:
    sbc(/* args */);
    ~sbc();
    void run() override final;
    
    void ping();

    void OnDataSent(esp_now_send_status_t status)override final;
    void OnDataRecv(const uint8_t *data, int data_len)override final;
    clock_t m_hearBeatTime;
};
