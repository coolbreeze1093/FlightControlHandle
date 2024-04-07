#include <thread>
#include <condition_variable>
#include <mutex>
class SbcInterface;
class SbcThread
{
private:
    /* data */
    std::thread m_thread;
    int m_internal=0;
    bool m_isRunning=false;
    std::condition_variable m_conVar;
    std::mutex m_mutex;
    SbcInterface *m_sbc=nullptr;
private:
    void run();

public:
    SbcThread(SbcInterface*sbc);
    ~SbcThread();
    void start(int internal);
    void stop();
};
