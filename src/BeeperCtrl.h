#pragma once

#include <thread>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <Arduino.h>
class BeeperCtrl
{
public:
    struct BeeperConfig
    {
        int internal = 0;
        int nums = 0;
    };

private:
    std::queue<BeeperConfig> m_taskVector;
    std::condition_variable m_conVar;
    std::mutex m_mutex;
    std::thread m_thread;
    void run();
    bool m_isRungning = false;
    int m_pin = 23;

public:
    BeeperCtrl(/* args */);
    ~BeeperCtrl();
    void start();
    void stop();
    void addBeeperTask(BeeperConfig &task);
};
