/** openbook-logger.cpp - Logger object **/

#include "openbook2.h"

// Further declarations mysteriously required by C++.
ofstream Logger::mLogFile;
char Logger::LOG_PATH[80];
bool Logger::mDebugMode;

// Initialise
// Establishes filepaths. Opens DEBUG log.
void Logger::Initialise(bool xiDebugMode)
{
    strcpy(LOG_PATH, "C:\\LOG_");
    strcat(LOG_PATH, "BOOK_DEBUG.log");
    mLogFile.open(LOG_PATH);

    mDebugMode = xiDebugMode;
}

void Logger::Finish()
{
    mLogFile.close();
}

// Log
// Records log data to the specified log.
void Logger::Log(string* xiMessage, eLogType xiLog)
{
    switch (xiLog)
    {
    case LOG_DEBUG:
        if (mDebugMode)
        {
            mLogFile  << "[>] debug:  " << xiMessage->c_str() << "\n";
        }
        break;

    case LOG_NB:
        mLogFile  << "[*] note:   " << xiMessage->c_str() << "\n";
        break;

    case LOG_ERROR:
        mLogFile  << "[!] error:  " << xiMessage->c_str() << "\n";
        break;

    case LOG_WARN:
        mLogFile  << "[:] warn:   " << xiMessage->c_str() << "\n";

    case LOG_SCRIPT:
        std::cout << "... script: " << xiMessage->c_str() << "\n";
        break;

    default:
        std::cout << "[?] log?:   " << xiMessage->c_str() << "\n";
    }
}

// Log
// An overload to accept a char*.
void Logger::Log(const char* xiMessage, eLogType xiLog)
{
    string* lMessage = new string(xiMessage);
    Log(lMessage, xiLog);
    delete lMessage;
}
