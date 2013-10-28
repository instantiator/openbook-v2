/** openbook-frotzer-thread.h - Frotzer handling thread. **/

class FrotzerThread : public ZThread::Runnable
{
    public:
    FrotzerThread();
    void run();
    void rest();
};

