/** openbook-frotzer.cpp - Launch and interact with Frotz. **/

#include "openbook2.h"

// declarations mysteriously required by C++
bool Frotzer::InputReady;
string* Frotzer::InputString;
const FrotzerThread* Frotzer::MyThread;

void Frotzer::Initialise(const FrotzerThread* xiMyFrotzerThread)
{
    MyThread = xiMyFrotzerThread;

    InputReady = false;
    InputString = new string("");
}

void Frotzer::Finish() { }

void Frotzer::Execute()
{
    Logger::Log("Entering Frotzer::Execute...", LOG_DEBUG);

    // blank arguments to frotz?
    // maybe later pass in the filename
    int lArgc = 0;
    char* lArgv[0];
    lArgv[0] = "";

    frotz_original_main(lArgc, lArgv);
}

void Frotzer::PrintText(char* xiText)
{
    Logger::Log("Entering Frotzer::PrintText...", LOG_DEBUG);
    string* lText = new string(xiText);
    PrintLogic::AddText(lText);
    delete lText;
}

void Frotzer::PrintChar(char xiChar)
{
    Logger::Log("Entering Frotzer::PrintChar...", LOG_DEBUG);
    PrintLogic::AddChar(xiChar);
}

void Frotzer::AddInputChar(char xiChar)
{
    Logger::Log((string("Entering Frotzer::AddInputChar... With: ").append(1, xiChar)).c_str(), LOG_DEBUG);

    switch (xiChar)
    {
        case 13: // enter
            InputString->append(1, ' '); // bap a space on the end of the string
            InputReady = true;           // alert the frotz thread
            break;

        case 8: // backspace
            if (InputString->length() > 0)
            {
                // reduce input string by 1 character, and display the change
                string lStringReduced = InputString->substr(0, InputString->length() - 1);
                InputString->swap(lStringReduced);
                PrintChar(xiChar);
            }
            break;

        default:
            // append character to input string, and display
            InputString->append(1, xiChar);
            PrintChar(xiChar);
            break;
    }
}

const char Frotzer::GetInputChar(float xiTimeout)
{
    Logger::Log("Entering Frotzer::GetInputChar...", LOG_DEBUG);

    float lTimeStart = glutGet(GLUT_ELAPSED_TIME);
    bool lTimedOut = false;

    cout << "char get with timeout: " << (int)xiTimeout;

    while ( !lTimedOut )
    {
        if (xiTimeout > 0.0)
        {
            if ((glutGet(GLUT_ELAPSED_TIME) - lTimeStart)/1000.0 > xiTimeout)
            {
                lTimedOut = true;
            }
        }

        // grab FIRST character in input buffer
        if (InputString->length() > 0)
        {
            // grab character, reduce stringsize
            char lRetChar = InputString->at(0);
            string lReduced = InputString->substr(1, InputString->length()-1);
            InputString->swap(lReduced);
            return lRetChar;
        }
    }

    return ZC_TIME_OUT;
}

const char* Frotzer::GetInputString(float xiTimeout)
{
    Logger::Log("Entering Frotzer::GetInputString...", LOG_DEBUG);

    // calculate time difference
    float lTimeStart = glutGet(GLUT_ELAPSED_TIME);
    bool lTimedOut = false;

    // qqLGW - This is THRASHING THE MACHINE to pieces. Find a better way.
    while ( !InputReady && !lTimedOut )
    {
        if (xiTimeout > 0.0)
        {
            // Z-Machine expresses time in 1/10ths   of a second,
            // OpenGL    expresses time in 1/1000ths of a second.
            if ((glutGet(GLUT_ELAPSED_TIME) - lTimeStart)/1000.0 > xiTimeout/10.0)
            {
                lTimedOut = true;
            }
        }

//        try
//        {
            // Traffic-calming measure. Cheap+dirty.
            //MyThread->rest();
//        }
//        catch (ZThread::Interrupted_Exception& e)
//        {
            // NOP
//        }
    }

    if (InputReady)
    {
        InputReady = false;

        Logger::Log("Fetching InputString:", LOG_DEBUG);
        Logger::Log(InputString, LOG_DEBUG);

        string* lReturnedString = new string("");
        InputString->swap(*lReturnedString);
        return lReturnedString->c_str();
    }
    else
    {
        return "";
    }
}
