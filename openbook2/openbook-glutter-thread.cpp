/** openbook-glutter-thread.cpp - GLUT handling thread. **/

#include "openbook2.h"

GlutterThread::GlutterThread()
{
    Logger::Log("Initialising GlutterThread...", LOG_DEBUG);
}

void GlutterThread::run()
{
    Logger::Log("Entering GlutterThread::run...", LOG_DEBUG);

    // setup (now in same thread)
    Glutter::Initialise();

    // main core GLUT loop
    Glutter::ProcessEvents();

    Logger::Log("Exiting GlutterThread::run...", LOG_DEBUG);
}


