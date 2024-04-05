#include "sbc.h"
#include "EspNewMan.h"

sbc::sbc(/* args */)
{
    m_hearBeatTime = clock();
}

sbc::~sbc()
{
}

void sbc::run()
{
    JsonDocument doc;
    GetCtrlValue::CtrlType _cType = getV.CtrlValue();
    if (m_lastCtrlType != _cType)
    {
        Serial.println(int(_cType));
    }
    bool _needSendValue = false;
    if (_cType == GetCtrlValue::CtrlType::init)
    {
        OnlyDebugOnceInitValue = true;
        calV.initCtrlValue(getV.ThrottleInitValue(), getV.HorizenDirectionInitValueL(), getV.VerticalDirectionInitValue(), getV.HorizenDirectionInitValueR());
    }
    
    if (!m_isConnect)
    {
        m_lastCtrlType = _cType;
        if (clock() - m_hearBeatTime > 3000)
        {
            m_hearBeatTime=clock();
            ping();
            //Serial.println("ping();");
        }
        
        return;
    }
    else
    {
        if (clock() - m_hearBeatTime >= 5000)
        {
            m_isConnect = false;
            BeeperCtrl::BeeperConfig _task;
            _task.internal = 1500;
            _task.nums = 1;
            m_beeper->addBeeperTask(_task);
        }
        else if (clock() - m_hearBeatTime >= 3000)
        {
            doc["HeartBeat"] = 0;
            String json;
            serializeJson(doc, json);
            EspNewMan_H.send(getMacAddr(), (uint8_t *)json.c_str(), json.length());
        }
        else
        {
        }
    }
    if (m_lastCtrlType == GetCtrlValue::CtrlType::init && _cType == GetCtrlValue::CtrlType::close && m_revInitValueR)
    {
        _needSendValue = true;
        outInitValue();
        calV.quitInit();
        calV.initChannleValue();
        JsonObject _obj = doc["InitValue"].to<JsonObject>();
        _obj["channel1"] = calV.getChannelValue_1();
        _obj["channel2"] = calV.getChannelValue_2();
        _obj["channel3"] = calV.getChannelValue_3();
        _obj["channel4"] = calV.getChannelValue_4();
        m_revInitValueR = false;
        m_revHardCtrlValueR = true;
        m_revOpenValueR = true;
        m_revCloseValueR = true;
    }
    else if (m_lastCtrlType == GetCtrlValue::CtrlType::open && _cType == GetCtrlValue::CtrlType::close && m_revCloseValueR)
    {
        _needSendValue = true;
        doc["close"] = 0;
        m_revCloseValueR = false;
        m_revInitValueR = true;
    }
    else if (_cType == GetCtrlValue::CtrlType::open && m_lastCtrlType == GetCtrlValue::CtrlType::close && m_revOpenValueR)
    {
        _needSendValue = true;
        doc["open"] = 0;
        m_revOpenValueR = false;
    }
    else if (_cType == GetCtrlValue::CtrlType::open && m_lastCtrlType == GetCtrlValue::CtrlType::open && m_revHardCtrlValueR)
    {
        _needSendValue = true;
        calV.freshCtrlValue(getV.ThrottleValue(), getV.HorizenDirectionValueL(), getV.VerticalDirectionValue(), getV.HorizenDirectionValueR());
        JsonObject _obj = doc["HardCtrl"].to<JsonObject>();
        _obj["channel1"] = calV.getChannelValue_1();
        _obj["channel2"] = calV.getChannelValue_2();
        _obj["channel3"] = calV.getChannelValue_3();
        _obj["channel4"] = calV.getChannelValue_4();
        m_revHardCtrlValueR = false;
    }
    if (_needSendValue)
    {
        String json;
        serializeJson(doc, json);
        EspNewMan_H.send(getMacAddr(), (uint8_t *)json.c_str(), json.length());
    }

    m_lastCtrlType = _cType;
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
    if (doc.containsKey("pong"))
    {
        Serial.println("connect success");
        pong();
    }
    else if (doc.containsKey("InitValue"))
    {
        m_revInitValueR = true;
    }
    else if (doc.containsKey("HardCtrl"))
    {
        m_revHardCtrlValueR = true;
    }
    else if (doc.containsKey("open"))
    {
        m_revOpenValueR = true;
    }
    else if (doc.containsKey("close"))
    {
        m_revCloseValueR = true;
    }
    else if (doc.containsKey("HeartBeat"))
    {
        m_hearBeatTime = clock();
    }
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
    m_hearBeatTime = clock();
    m_isConnect = true;
    if (m_beeper)
    {
        BeeperCtrl::BeeperConfig _task;
        _task.internal = 100;
        _task.nums = 1;
        m_beeper->addBeeperTask(_task);
        _task.internal = 50;
        _task.nums = 1;
        m_beeper->addBeeperTask(_task);
        _task.internal = 100;
        _task.nums = 1;
        m_beeper->addBeeperTask(_task);
    }
}

void sbc::ping()
{

    JsonDocument doc;
    doc["ping"] = 0;
    String json;
    serializeJson(doc, json);
    EspNewMan_H.send(getMacAddr(), (uint8_t *)json.c_str(), json.length());
}