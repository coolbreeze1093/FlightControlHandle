#include "SbcThread.h"
#include "SbcInterface.h"

SbcThread::SbcThread(SbcInterface*sbc):m_sbc(sbc)
{

}

SbcThread::~SbcThread()
{
}

void SbcThread::start(int internal)
{
    m_isRunning=true;
    m_internal = internal;
    m_thread = std::thread(std::bind(&SbcThread::run,this));
}

void SbcThread::stop()
{
    m_isRunning=false;
    m_conVar.notify_all();
    if(m_thread.joinable())
    {
        m_thread.join();
    }
}
void SbcThread::run()
{
    while (m_isRunning)
    {
        /* code */
        std::unique_lock<std::mutex> _lock(m_mutex);
        if(m_internal>0)
        {
            m_conVar.wait_until(_lock,std::chrono::system_clock::now()+std::chrono::milliseconds(m_internal));
            /* code */
        }
        m_sbc->run();
    }   
}
