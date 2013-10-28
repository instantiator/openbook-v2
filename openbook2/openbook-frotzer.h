/** openbook-frotzer.h - Launches and interacts with Frotz. **/
#include "openbook-frotzer-thread.h"

class Frotzer
{
    public:
    static void Initialise(const FrotzerThread* xiMyThread);
    static void Finish();

    static void Execute();
    static void PrintText(char* xiText);
    static void PrintChar(char xiChar);

    static bool InputReady;
    static string* InputString;
    static void AddInputChar(char xiChar);
    static const char* GetInputString(float xiTimeout);
    static const char  GetInputChar(float xiTimeout);

    static const FrotzerThread* MyThread;

};
