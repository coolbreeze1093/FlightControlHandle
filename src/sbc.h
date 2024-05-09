#pragma once
#include <vector>
#include "CalChannelValue.h"
#include "GetCtrlValue.h"
#include "ArduinoJson.h"
#include "SbcInterface.h"
#include "OledMang.h"
#include "PowerCalculation.h"
#include "common.h"


class sbc :public SbcInterface
{
private:
    CalChannelValue calV;
    GetCtrlValue* m_getV=nullptr;
    bool OnlyDebugOnceInitValue = true;
    GetCtrlValue::CtrlType m_lastCtrlType=GetCtrlValue::CtrlType::error;
    bool m_revInitValueR=true;
    bool m_revOpenValueR=true;
    bool m_revCloseValueR=true;
    bool m_revHardCtrlValueR=true;
    clock_t m_hearBeatTime;
    clock_t m_VolWarning;
    BeeperCtrl *m_beeper=nullptr;
    OledMang*m_oled=nullptr;
    PowerCalculation*m_PowerCalculation=nullptr;
    SbcStatus m_status;
private:
    void outInitValue();
    void pong() override;
    
    void connected();
    void disconnect();
    void sendCtrlMsg(GetCtrlValue::CtrlType type);
    void sendHeartBeatMsg();
public:
    sbc(GetCtrlValue*getV,BeeperCtrl *beep);
    ~sbc();
    void run() override final;
    
    void ping();

    void OnDataSent(esp_now_send_status_t status)override final;
    void OnDataRecv(const uint8_t *data, int data_len)override final;

    void setOled(OledMang*oled);
    void setPowerCalculation(PowerCalculation*c);
};
