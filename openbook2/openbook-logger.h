/** openbook-logger.h - For writing data to log files. Very Static. **/

class Logger
{
    public:
    static void Initialise(bool xiTestMode);
    static void Finish();

    static void Log(std::string* text, eLogType log);
    static void Log(const char* text, eLogType log);

    private:
    static ofstream mLogFile;
    static char LOG_PATH[80];
    static bool mDebugMode;
};
