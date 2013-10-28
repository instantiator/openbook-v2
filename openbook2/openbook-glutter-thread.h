/** openbook-glutter-thread.h - GLUT handling thread. **/

class GlutterThread : public ZThread::Runnable
{
    public:
    GlutterThread();
    void run();
};

