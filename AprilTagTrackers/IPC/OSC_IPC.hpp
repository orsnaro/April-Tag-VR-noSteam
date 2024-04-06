#if defined(ATT_OS_WINDOWS) && defined(FORCE_OSC)

#pragma once
#include <array>
#include <atomic>
#include <iostream>
#include <map>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <fstream>

#include <ctime>
#include <lo/lo.h>
#include <lo/lo_cpp.h>

#ifndef WIN32
#    include <unistd.h>
#else
# include <windows.h>  
#endif

namespace SimSuit
{

enum class States
{
    FAIL = -1,
    OK = 0,
    PENDING = 1
};
struct suitBuffer
{
    std::string data;
    States state;
};

int waitLoop(suitBuffer& gatewayBuf, int _timeout);
States logFile(std::string txt, std::string path = "logs/simSuitAprilLog.txt");

class OSC_noSteamGateway
{
public:
    OSC_noSteamGateway(std::string portTargetOSC = "9000", std::string ipTargetOSC = "192.168.1.3");

    suitBuffer& runTst(std::string msg, lo::ServerThread& AprilServerTh, lo::Address& addressAprilSideOSC, std::string& pathTargetOSC, suitBuffer& buffer);

    suitBuffer& run(std::string msg, lo::ServerThread& AprilServerTh, lo::Address& addressAprilSideOSC, std::string& pathTargetOSC, suitBuffer& buffer);

private:
    std::string mPortTargetOSC;
    std::string mIpTargetOSC;
    // save VRChat OSC paths here in a map
    // but later use rapidJson and save the paths in json file (no build for it needed):
    // https://rapidjson.org/

    // each entry has  vector of size 6: 3 for pos and 3 for rot (x,y,z,
    // keys are the OSC paths ( hip, chest, 2feet, 2knees, 2elbows )
    std::unordered_map<std::string, std::vector<float>> trackersInfoMap; //has all mPathTargetSideOSC
};

} // namespace SimSuit
#endif