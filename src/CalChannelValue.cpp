#ifndef __CALCHANNELVALUE_H__
#define __CALCHANNELVALUE_H__

#include "CalChannelValue.h"
#include "Arduino.h"

CalChannelValue::CalChannelValue()
{
}

CalChannelValue::~CalChannelValue()
{
}

void CalChannelValue::initCtrlValue(int16_t value1, int16_t value2, int16_t value3, int16_t value4)
{
    if (m_initOnlyOnce)
    {
        m_initValue.clear();
        OneCtrlValue _cvalue1, _cvalue2, _cvalue3, _cvalue4;
        _cvalue1.init = value1;
        _cvalue2.init = value2;
        _cvalue3.init = value3;
        _cvalue4.init = value4;
        m_initValue.insert(std::make_pair(m_CtrlValueName_1, _cvalue1));
        m_initValue.insert(std::make_pair(m_CtrlValueName_2, _cvalue2));
        m_initValue.insert(std::make_pair(m_CtrlValueName_3, _cvalue3));
        m_initValue.insert(std::make_pair(m_CtrlValueName_4, _cvalue4));

        m_initOnlyOnce = false;
    }
    m_initValue[m_CtrlValueName_1].updateValue(value1);
    m_initValue[m_CtrlValueName_2].updateValue(value2);
    m_initValue[m_CtrlValueName_3].updateValue(value3);
    m_initValue[m_CtrlValueName_4].updateValue(value4);
}

void CalChannelValue::quitInit()
{
    m_initOnlyOnce = true;
}

float CalChannelValue::rangLimit(float value)
{
    if (value < 0)
    {
        return 0;
    }
    else if (value > 1)
    {
        return 1;
    }

    return value;
}

void CalChannelValue::diffSpeed(int16_t value1, int16_t value2, int16_t value3, int16_t value4)
{
    OneCtrlValue _cvalue = m_initValue[m_CtrlValueName_1];
    OneCtrlValue _hIvalue_L = m_initValue[m_CtrlValueName_2];

    m_channleValue_1 = m_channleValue_2 = map(value1, _cvalue.min, _cvalue.max, 0, 100);
    if (value2 > _hIvalue_L.init)
    {
        float _hPro = rangLimit(float(value2 - _hIvalue_L.init) / float(_hIvalue_L.max - _hIvalue_L.init));
        m_channleValue_2 = m_channleValue_2 * (1 - _hPro * 0.4);
    }
    else if (value2 < _hIvalue_L.init)
    {
        float _hPro = rangLimit(float(_hIvalue_L.init - value2) / float(_hIvalue_L.init));
        m_channleValue_1 = m_channleValue_1 * (1 - _hPro * 0.4);
    }

    OneCtrlValue _VIvalue_R = m_initValue[m_CtrlValueName_3];
    if (value3 > _VIvalue_R.init)
    {
        float _hPro = rangLimit(float(value3 - _VIvalue_R.init) / float(_VIvalue_R.max - _VIvalue_R.init));
        m_channleValue_3 = _hPro * 90.0 + 90;
    }
    else if (value3 < _VIvalue_R.init)
    {
        float _hPro = rangLimit(float(_VIvalue_R.init - value3) / float(_VIvalue_R.init));
        m_channleValue_3 = 90 - _hPro * 90.0;
    }
    else
    {
        m_channleValue_3 = 90;
    }

    OneCtrlValue _hIvalue_R = m_initValue[m_CtrlValueName_4];
    if (value4 > _hIvalue_R.init)
    {
        float _hPro = rangLimit(float(value4 - _hIvalue_R.init) / float(_hIvalue_R.max - _hIvalue_R.init));
        m_channleValue_4 = _hPro * 90.0 + 90;
    }
    else if (value4 < _hIvalue_R.init)
    {
        float _hPro = rangLimit(float(_hIvalue_R.init - value4) / float(_hIvalue_R.init));
        m_channleValue_4 = 90 - _hPro * 90.0;
    }
    else
    {
        m_channleValue_4 = 90;
    }
}

void CalChannelValue::Mixing(int16_t value1, int16_t value2, int16_t value3, int16_t value4)
{
    OneCtrlValue _cvalue = m_initValue[m_CtrlValueName_1];
    OneCtrlValue _hIvalue_L = m_initValue[m_CtrlValueName_2];
    m_channleValue_1 = map(value1, _cvalue.min, _cvalue.max, 0, 100);

    if (value2 > _hIvalue_L.init)
    {
        float _hPro = rangLimit(float(value2 - _hIvalue_L.init) / float(_hIvalue_L.max - _hIvalue_L.init));
        m_channleValue_2 = _hPro * 90.0 + 90;
    }
    else if (value2 < _hIvalue_L.init)
    {
        float _hPro = rangLimit(float(_hIvalue_L.init - value2) / float(_hIvalue_L.init));
        m_channleValue_2 = 90 - _hPro * 90.0;
    }
    else
    {
        m_channleValue_2 = 90;
    }

    int16_t _vValue = value3;
    int16_t _hValue = value4;
    OneCtrlValue _vIValue = m_initValue[m_CtrlValueName_3];
    OneCtrlValue _hIvalue = m_initValue[m_CtrlValueName_4];

    if (_vValue > _vIValue.init)
    {
        float _vPro = rangLimit(float(_vValue - _vIValue.init) / float(_vIValue.max - _vIValue.init));

        if (_hValue > _hIvalue.init)
        {
            float _hPro = rangLimit(float(_hValue - _hIvalue.init) / float(_hIvalue.max - _hIvalue.init));
            if (_vPro < _hPro)
            {
                m_channleValue_3 = _hPro * 90.0 + 90;
                m_channleValue_4 = 90 - _hPro * 90.0;
            }
            else
            {
                m_channleValue_3 = m_channleValue_4 = _vPro * 90.0 + 90;
            }
        }
        else if (_hValue < _hIvalue.init)
        {
            float _hPro = rangLimit(float(_hIvalue.init - _hValue) / float(_hIvalue.init));
            if (_vPro < _hPro)
            {
                m_channleValue_3 = 90 - _hPro * 90.0;
                m_channleValue_4 = _hPro * 90.0 + 90;
            }
            else
            {
                m_channleValue_3 = m_channleValue_4 = _vPro * 90.0 + 90;
            }
        }
        else
        {
            m_channleValue_3 = m_channleValue_4 = _vPro * 90.0 + 90;
        }
    }
    else if (_vValue < _vIValue.init)
    {
        float _vPro = rangLimit(float(_vIValue.init - _vValue) / float(_vIValue.init));
        if (_hValue > _hIvalue.init)
        {
            float _hPro = rangLimit(float(_hValue - _hIvalue.init) / float(_hIvalue.max - _hIvalue.init));
            if (_vPro < _hPro)
            {
                m_channleValue_3 = _hPro * 90.0 + 90;
                m_channleValue_4 = 90 - _hPro * 90.0;
            }
            else
            {
                m_channleValue_3 = m_channleValue_4 = 90 - _vPro * 90.0;
            }
        }
        else if (_hValue < _hIvalue.init)
        {
            float _hPro = rangLimit(float(_vIValue.init - _hValue) / float(_hIvalue.init));
            if (_vPro < _hPro)
            {
                m_channleValue_3 = 90 - _hPro * 90.0;
                m_channleValue_4 = _hPro * 90.0 + 90;
            }
            else
            {
                m_channleValue_3 = m_channleValue_4 = 90 - _vPro * 90.0;
            }
        }
        else
        {
            m_channleValue_3 = m_channleValue_4 = 90 - _vPro * 90.0;
        }
    }
    else
    {
        if (_hValue > _hIvalue.init)
        {
            float _hPro = rangLimit(float(_hValue - _hIvalue.init) / float(_hIvalue.max - _hIvalue.init));
            m_channleValue_3 = _hPro * 90.0 + 90;
            m_channleValue_4 = 90 - _hPro * 90.0;
        }
        else if (_hValue < _hIvalue.init)
        {
            float _hPro = rangLimit(float(_hIvalue.init - _hValue) / float(_hIvalue.init));
            m_channleValue_3 = 90 - _hPro * 90.0;
            m_channleValue_4 = _hPro * 90.0 + 90;
        }
        else
        {
            m_channleValue_3 = m_channleValue_4 = 90;
        }
    }
}

void CalChannelValue::freshCtrlValue(int16_t value1, int16_t value2, int16_t value3, int16_t value4)
{
    diffSpeed(value1, value2, value3, value4);
}

float CalChannelValue::getChannelValue_1()
{
    return m_channleValue_1;
}

float CalChannelValue::getChannelValue_2()
{
    return m_channleValue_2;
}

float CalChannelValue::getChannelValue_3()
{
    //
    return m_channleValue_3;
}

float CalChannelValue::getChannelValue_4()
{
    return m_channleValue_4;
}

void CalChannelValue::initChannleValue()
{
    m_channleValue_1 = 0;
    m_channleValue_2 = 0;
    m_channleValue_3 = 90;
    m_channleValue_4 = 90;
}

#endif // __CALCHANNELVALUE_H__