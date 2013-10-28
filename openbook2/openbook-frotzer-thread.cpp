/** openbook-frotzer-thread.cpp - Frotzer handling thread. **/

#include "openbook2.h"

FrotzerThread::FrotzerThread()
{
    Logger::Log("Initialising FrotzerThread...", LOG_DEBUG);
}

void FrotzerThread::run()
{
    Logger::Log("Entering FrotzerThread::run...", LOG_DEBUG);

    try
    {
        Frotzer::Initialise(this);
        Frotzer::Execute();
    }
    catch (ZThread::Interrupted_Exception)
    {
        Logger::Log("FrotzerThread has been interrupted.", LOG_DEBUG);
    }

    Logger::Log("Exiting FrotzerThread::run...", LOG_DEBUG);
    Frotzer::Finish();
}

void FrotzerThread::rest()
{
    ZThread::Thread::sleep(100);
}
