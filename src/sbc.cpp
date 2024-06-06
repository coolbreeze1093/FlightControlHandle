#include "sbc.h"
#include "EspNewMan.h"

sbc::sbc(GetCtrlValue *getV, BeeperCtrl *beep) : m_getV(getV), m_beeper(beep)
{
}

sbc::~sbc()
{
}

void sbc::run()
{

    GetCtrlValue::CtrlType _cType = m_getV->CtrlValue();
    if (m_lastCtrlType != _cType)
    {
        Serial.println(int(_cType));
    }

    if (_cType == GetCtrlValue::CtrlType::init)
    {
        OnlyDebugOnceInitValue = true;
        calV.initCtrlValue(m_getV->ThrottleInitValue(), m_getV->HorizenDirectionInitValueL(), m_getV->VerticalDirectionInitValue(), m_getV->HorizenDirectionInitValueR());
    }

    sendCtrlMsg(_cType);

    m_lastCtrlType = _cType;
    if (m_oled)
    {
        m_oled->setSbcStatus(m_status);
    }
}

void sbc::OnDataSent(esp_now_send_status_t status)
{
    if (status != ESP_NOW_SEND_SUCCESS)
    {
        Serial.println("Delivery Fail");
    }
}

void sbc::OnDataRecv(const uint8_t *data, int data_len)
{
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, data);
    // String _json(data,data_len);
    // Serial.println(_json);
    //  Test if parsing succeeds
    if (error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
    }
    if (doc.containsKey("InitValue"))
    {
        BeeperCtrl::BeeperConfig _task;
        _task.playInternal = 1000;
        _task.stopInternal = 100;
        m_beeper->addBeeperTask(_task);
    }

    if (doc.containsKey("volatile"))
    {
        m_status.m_masterVol = m_PowerCalculation->getRemainPower();
        m_status.m_slaveVol = doc["volatile"].as<int>();
        if (m_beeper && (m_status.m_masterVol <= 30 || m_status.m_slaveVol <= 30) && m_lastCtrlType == GetCtrlValue::CtrlType::open)
        {
            BeeperCtrl::BeeperConfig _task;
            _task.playInternal = 100;
            _task.stopInternal = 100;
            _task.nums = 2;
            m_beeper->addBeeperTask(_task);
        }
    }
}

void sbc::setOled(OledMang *oled)
{
    m_oled = oled;
}

void sbc::setPowerCalculation(PowerCalculation *c)
{
    m_PowerCalculation = c;
}

void sbc::outInitValue()
{
    if (OnlyDebugOnceInitValue)
    {
        for (auto var : calV.getInitValue())
        {
            Serial.print(var.first.c_str());
            Serial.print("min:");
            Serial.print(var.second.min);
            Serial.print("init:");
            Serial.print(var.second.init);
            Serial.print("max:");
            Serial.print(var.second.max);
            Serial.println(" ");
        }
        OnlyDebugOnceInitValue = false;
    }
}

void sbc::pong()
{
    connected();
}

void sbc::connected()
{
    m_connectState = ConnectState::connected;
    m_status.m_conState = m_connectState;
    if (m_beeper)
    {
        BeeperCtrl::BeeperConfig _task;
        _task.playInternal = 100;
        _task.stopInternal = 50;
        _task.nums = 1;
        m_beeper->addBeeperTask(_task);
        _task.playInternal = 50;
        _task.stopInternal = 50;
        _task.nums = 1;
        m_beeper->addBeeperTask(_task);
        _task.playInternal = 50;
        _task.stopInternal = 50;
        _task.nums = 1;
        m_beeper->addBeeperTask(_task);
    }
}

void sbc::disconnect()
{
    m_connectState = ConnectState::disconnect;
    m_status.m_conState = m_connectState;

    BeeperCtrl::BeeperConfig _task;
    _task.playInternal = 1500;
    _task.stopInternal = 100;
    _task.nums = 1;
    m_beeper->addBeeperTask(_task);
}

void sbc::sendCtrlMsg(GetCtrlValue::CtrlType type)
{
    JsonDocument doc;
    bool _needSendValue = false;
    if (m_lastCtrlType == GetCtrlValue::CtrlType::init && type == GetCtrlValue::CtrlType::close)
    {
        _needSendValue = true;
        outInitValue();
        calV.quitInit();
        calV.initChannleValue();
        m_status.m_channel1 = calV.getChannelValue_1();
        m_status.m_channel2 = calV.getChannelValue_2();
        m_status.m_channel3 = calV.getChannelValue_3();
        m_status.m_channel4 = calV.getChannelValue_4();
        JsonObject _obj = doc["InitValue"].to<JsonObject>();
        _obj["channel1"] = m_status.m_channel1;
        _obj["channel2"] = m_status.m_channel2;
        _obj["channel3"] = m_status.m_channel3;
        _obj["channel4"] = m_status.m_channel4;
    }
    else if (m_lastCtrlType == GetCtrlValue::CtrlType::open && type == GetCtrlValue::CtrlType::close)
    {
        _needSendValue = true;
        doc["close"] = 0;
    }
    else if (type == GetCtrlValue::CtrlType::open && m_lastCtrlType == GetCtrlValue::CtrlType::close)
    {
        _needSendValue = true;
        doc["open"] = 0;
    }
    else if (type == GetCtrlValue::CtrlType::open && m_lastCtrlType == GetCtrlValue::CtrlType::open)
    {
        _needSendValue = true;
        calV.freshCtrlValue(m_getV->ThrottleValue(), m_getV->HorizenDirectionValueL(), m_getV->VerticalDirectionValue(), m_getV->HorizenDirectionValueR());
        m_status.m_channel1 = calV.getChannelValue_1();
        m_status.m_channel2 = calV.getChannelValue_2();
        m_status.m_channel3 = calV.getChannelValue_3();
        m_status.m_channel4 = calV.getChannelValue_4();
        JsonObject _obj = doc["HardCtrl"].to<JsonObject>();
        _obj["channel1"] = m_status.m_channel1;
        _obj["channel2"] = m_status.m_channel2;
        _obj["channel3"] = m_status.m_channel3;
        _obj["channel4"] = m_status.m_channel4;
    }
    if (_needSendValue)
    {
        String json;
        serializeJson(doc, json);
        EspNewMan_H.send(getMacAddr(), (uint8_t *)json.c_str(), json.length());
    }
}

void sbc::sendHeartBeatMsg()
{
    JsonDocument doc;
    doc["HeartBeat"] = 0;
    String json;
    serializeJson(doc, json);
    EspNewMan_H.send(getMacAddr(), (uint8_t *)json.c_str(), json.length());
}

void sbc::ping()
{
    JsonDocument doc;
    doc["ping"] = 0;
    String json;
    serializeJson(doc, json);
    EspNewMan_H.send(getMacAddr(), (uint8_t *)json.c_str(), json.length());
}