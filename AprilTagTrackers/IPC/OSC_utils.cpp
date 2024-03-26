#if defined(ATT_OS_WINDOWS) && defined(FORCE_OSC)

#include "OSC_IPC.hpp"

namespace SimSuit
{
int waitLoop(suitBuffer& gatewayBuf, int _timeout)
{
    //5sec 10*500 = 5000ms

    int pendingMsgTrigger = 0;
    while (gatewayBuf.state == States::PENDING and --_timeout > 0)
    {
        if (pendingMsgTrigger >= 1000)
        {
            SimSuit::logFile("From OSC_utils.cpp waitLoop(): PEDNING OSC Gateway UPDATE");
            std::cout << "PEDNING OSC Gateway UPDATE...";

            pendingMsgTrigger = 0;
        }
        else
        {
            pendingMsgTrigger += 10;
        }

#    ifdef WIN32
        Sleep(10);
#    else
        usleep(10 * 1000);
#    endif
    }
    return _timeout;
}

States logFile(std::string txt, std::string path)
{
    time_t now = time(0);
    char b[100] = "[";
    char* datetime = ctime(&now);
    std::strcat(b, datetime);
    std::string datetime_str = std::string(b);
    datetime_str = datetime_str.replace(datetime_str.size() - 1, 1, "]");
    //format will be `[day month date hour:min:sec year]`

    std::ofstream oFile(path, std::ios::app);
    if (oFile.is_open())
    {
        oFile << datetime_str << " " << txt << '\n';
        oFile.close();
    }
    else
    {
        return States::FAIL;
    }
    return States::OK;
}



}
#endif