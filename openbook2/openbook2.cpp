/** openbook2.cpp - Launcher for the OpenBook project. **/

#include "openbook2.h"

bool TEST_MODE = true;

void printHelp()
{
    cout << "Usage: openbook2 <filename> [<screen>]\n\n";
    cout << "filename - the game file.\n";
    cout << "screen   - default: 1024x768:16.\n\n";
    cout << "ie. 640x480:16 = 640x480 resolution, 16bpp.\n\n";
}

void quitWithHelp()
{
    printHelp();
    exit(0);
}

// Program entry point.
// 1. Spin off a thread for the z-machine.
// 2. Initialise GLUT.
// 3. Enter display/animate cycle.
int main(int argc, char *argv[])
{

    Logger::Initialise(true);

    try
    {
        // init display and print managers
        Logger::Log("Initialising OpenBook...", LOG_NB);
        Glutter::Initialise();
        PrintLogic::Initialise();

        // set up root umbrellas
        Logger::Log("Initialising root display objects...", LOG_NB);
        Book* lBook = new Book();

        // add them to display and print lists
        Logger::Log("Registering root display objects...", LOG_NB);
        Glutter::AddUmbrella(lBook);
        PrintLogic::AddUmbrella(lBook);

        // start threads
        Logger::Log("Starting Threads...", LOG_NB);
        ZThread::Thread lFrotzThread(new FrotzerThread());

        // and drop into the GLUT loop in this thread
        Logger::Log("Entering Glut Main Loop in initial thread...", LOG_NB);
        Glutter::ProcessEvents();

        Logger::Log("Glut Main Loop has returned.", LOG_NB);

        // bin out the Frotz thread if GLUT dies suddenly
        lFrotzThread.interrupt();

    }
    catch (char* error)
    {
        Logger::Log(error, LOG_ERROR);
    }
    catch (const ZThread::Synchronization_Exception& e)
    {
        Logger::Log(e.what(), LOG_ERROR);
    }
    catch (...)
    {
        Logger::Log("An unknown error occurred during execution.", LOG_ERROR);
    }

    Logger::Log("Shutting down OpenBook...", LOG_NB);

    PrintLogic::Finish();
    Glutter::Finish();

    Logger::Finish();
    return EXIT_SUCCESS;
}
