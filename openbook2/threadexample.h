/** threadexample.h - Threading sample class header. **/

// ThreadExample: dumps a string including its name a given number of times.
// =========================================================================
class ThreadExample : public ZThread::Runnable
{
    public:
    ThreadExample(std::string xiThreadName, size_t xiIterations);
    void run();

    private:
    std::string mName;
    size_t mLoops;

};

