#include "BeeperCtrl.h"

void BeeperCtrl::run()
{
    Serial.println("BeeperCtrl::run0");
    while (m_isRungning)
    {
        std::unique_lock<std::mutex> _lock(m_mutex);
        while (m_taskVector.empty())
        {
            m_conVar.wait(_lock);
        }

        BeeperConfig _config = m_taskVector.front();
        m_taskVector.pop();
        Serial.println("BeeperCtrl::run1");
        while (_config.nums > 0)
        {
            digitalWrite(m_pin, HIGH);
            delay(_config.internal);
            digitalWrite(m_pin, LOW);
            delay(_config.internal);
            _config.nums--;
        }
    }
}

BeeperCtrl::BeeperCtrl(/* args */)
{
    pinMode(m_pin, OUTPUT);
}

BeeperCtrl::~BeeperCtrl()
{
    stop();
}

void BeeperCtrl::start()
{
    m_isRungning = true;
    m_thread = std::thread(std::bind(&BeeperCtrl::run, this));
}

void BeeperCtrl::stop()
{
    m_isRungning = false;
    m_conVar.notify_all();
    if (m_thread.joinable())
    {
        m_thread.join();
    }
}

void BeeperCtrl::addBeeperTask(BeeperCtrl::BeeperConfig &task)
{
    std::unique_lock<std::mutex> _lock(m_mutex);
    m_taskVector.push(task);
    m_conVar.notify_all();
}