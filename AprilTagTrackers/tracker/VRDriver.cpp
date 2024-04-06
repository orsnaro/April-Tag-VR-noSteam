#include "VRDriver.hpp"

#include "SemVer.h"
#include "utils/Env.hpp"
#include "utils/Error.hpp"
#include "utils/Test.hpp"

#include <sstream>
#include <stdexcept>
#include <string>

inline std::ostream& operator<<(std::ostream& os, const Pose& pose)
{
    return os << pose.position.x << ' '
              << pose.position.y << ' '
              << pose.position.z << ' '
              << pose.rotation.w << ' '
              << pose.rotation.x << ' '
              << pose.rotation.y << ' '
              << pose.rotation.z;
}

inline std::istream& operator>>(std::istream& is, Pose& pose)
{
    return is >> pose.position.x >> pose.position.y >> pose.position.z >>
           pose.rotation.w >> pose.rotation.x >> pose.rotation.y >> pose.rotation.z;
}

namespace
{

template <typename... TArgs>
std::string BuildCommand(std::string_view name, const TArgs&... args)
{
    std::ostringstream ss;
    ss << ' ' << name
       << std::fixed << std::setprecision(6);
    ((ss << ' ' << args), ...);
    return ss.str();
}
TEST_CASE ("BuildCommand")
{
    CHECK(BuildCommand("updatepose") == " updatepose");
    CHECK(BuildCommand("foo", 10) == " foo 10");
    CHECK(BuildCommand("foo", 1.1) == " foo 1.100000");
    CHECK(BuildCommand("foo", 1.1F) == " foo 1.100000");
    CHECK(BuildCommand("foo", 10.1234559) == " foo 10.123456");
    CHECK(BuildCommand("foo", "bar", "baz", 10) == " foo bar baz 10");
    CHECK(BuildCommand("foo", Pose(cv::Point3d(0.1, 0.2, 0.3), cv::Quatd(1, 0, 0, 0))) ==
          " foo 0.100000 0.200000 0.300000 1.000000 0.000000 0.000000 0.000000");
}

template <typename... TArgs>
void VerifyAndParseResponse(std::string_view buffer, std::string_view expectName, TArgs&... outArgs)
{
    std::istringstream ss{std::string(buffer)};
    std::string name;
    ss >> name;
    if (name.empty())
    {
        SimSuit::logFile("From VRDdriver.cpp VerifyAndParseResponse():ERROR! no response from driver(target) ");
        throw utils::MakeError("no response from driver");
    }
    if (name != expectName)
    {
        SimSuit::logFile("From VRDdriver.cpp VerifyAndParseResponse():ERROR! got: " + name + ". awaited command response from target mismatched!");
        throw utils::MakeError("command response indicated failure: ", buffer);
    }
    ((ss >> outArgs), ...);
}
TEST_CASE ("VerifyAndParseResponse")
{
    DOCTEST_CHECK_NOTHROW(VerifyAndParseResponse(" updated", "updated"));
    DOCTEST_CHECK_THROWS(VerifyAndParseResponse(" updated", "idinvalid"));
    int outId = -1;
    DOCTEST_CHECK_NOTHROW(VerifyAndParseResponse(" foo 12", "foo", outId));
    CHECK(outId == 12);
}

} // namespace

namespace tracker
{

VRDriver::VRDriver(const cfg::List<cfg::TrackerUnit>& trackers)
    : mBridge(IPC::CreateDriverClient())
{
    // Only add trackers (and station) if they were not already added
    // also checks driver version, ensures can connect
    if (CmdGetTrackerCount() != trackers.GetSize())
    {
        int index = 0;
        for (const auto& tracker : trackers.AsRange())
        {
            AddTracker(index++, tracker.role);
        }

        if (trackers.GetSize() != CmdGetTrackerCount())
        {
            SimSuit::logFile("From VRDriver() constructor: ERROR!: called CmdGetTrackerCount() and some or all trackers were not registered with driver");
            throw std::runtime_error("some or all trackers were not registered with driver");
        }
        CmdAddStation();
    }
}

void VRDriver::UpdateTracker(int id, Pose pose, double frameTime, double smoothing)
{
    SimSuit::logFile("From VRDdriver.cpp: attempt UpdateTracker cmd... id: " + std::to_string(id + 1));
    const std::string cmd = BuildCommand("updatepose", id, pose, frameTime, smoothing);
    std::string _extraPath = "/tracking/trackers/" + (id == 9 ? "head" : std::to_string(id + 1));
    const std::string_view res = mBridge->SendRecv(cmd, _extraPath);
    VerifyAndParseResponse(res, "updated");
}

void VRDriver::CmdUpdateStation(int id, Pose pose)
{
    SimSuit::logFile("From VRDdriver.cpp: attempt CmdUpdateStation cmd... id: " + std::to_string(id + 1));
    const std::string cmd = BuildCommand("updatestation", id, pose);
    std::string _extraPath = "/tracking/station/" + std::to_string(id + 1); //id is zero indexed and OSC paths are 1 indexed (if causes issues fix path id to 1) using only one camear anyway for multiple reasons (see line 185)
    const std::string_view res = mBridge->SendRecv(cmd, _extraPath);
    VerifyAndParseResponse(res, "updated");
}

void VRDriver::SetSmoothing(double factor, double additional)
{
    SimSuit::logFile("From VRDdriver.cpp: attempt SetSmoothing cmd...");
    constexpr int saved = 120;
    const std::string cmd = BuildCommand("settings", saved, factor, additional);
    std::string _extraPath = "/tracking/settings"; //id is zero indexed and OSC paths are 1 indexed
    const std::string_view res = mBridge->SendRecv(cmd, _extraPath);
    VerifyAndParseResponse(res, "changed");
}

int VRDriver::CmdGetTrackerCount()
{
    SimSuit::logFile("From VRDdriver.cpp: startup.. attempt GetTrackerCount cmd...");

    std::string _extraPath = "/tracking/trackers/count";
    const std::string_view res = mBridge->SendRecv(BuildCommand("numtrackers"), _extraPath);
    int count = -1; //VerifyAndParseResponse() will update it and versionStr
    std::string versionStr;
    VerifyAndParseResponse(res, "numtrackers", count, versionStr);

    #pragma region TESTING
    //ORSNARO TESTING : force count ok
    //versionStr = "0.7.0";
    //count = 3;//must match the ui
    //ORSNARO TESTING : force count ok
    #pragma endregion 

    if (count < 0)
    {
        std::string e_msg{"invalid tracker count: " + std::to_string(count) +  " received msg: " + std::string(res)};

        SimSuit::logFile("From VRDdriver.cpp: CmdGetTrackerCount() : Error!: " + e_msg);
        throw std::runtime_error(e_msg);
    }

    const SemVer expected = utils::GetBridgeDriverVersion();
    if (!SemVer::Compatible(SemVer::Parse(versionStr), expected))
    {
        std::string e_msg2{"incompatible bridge driver: " + versionStr + " expected: " + std::string(res)};

        SimSuit::logFile("From VRDdriver.cpp: CmdGetTrackerCount() : Error(id=2)!: " + e_msg2);
        throw std::runtime_error(e_msg2);
        
    }

    return count;
}

void VRDriver::CmdAddTracker(std::string_view name, std::string_view role)
{
    SimSuit::logFile("From VRDdriver.cpp: attempt AddTracker cmd...");
    const std::string cmd = BuildCommand("addtracker", name, role);
    std::string _extraPath = "/tracking/trackers/add";
    const std::string_view res = mBridge->SendRecv(cmd, _extraPath);
    VerifyAndParseResponse(res, "added");
}

void VRDriver::CmdAddStation()
{
    SimSuit::logFile("From VRDdriver.cpp: attempt AddStation cmd...");
    std::string _extraPath = "/tracking/station/1"; //using one station only (april devs said more only increases the playspace not accuracy of detection and more than 1 camera may cause issues)
    const std::string_view res = mBridge->SendRecv(BuildCommand("addstation"), _extraPath);
    VerifyAndParseResponse(res, "added");
}

VRDriver::GetTrackerResult VRDriver::GetTracker(int id, double timeOffset)
{
    SimSuit::logFile("From VRDdriver.cpp: attempt GetTracker cmd... id: " + std::to_string(id + 1));
    const std::string cmd = BuildCommand("gettrackerpose", id, timeOffset);
    std::string _extraPath = "/tracking/trackers/" + (id == 9 ? "head" : std::to_string(id + 1)); //id is zero indexed but osc tracker paths are 1 indexed and might have names for specific tracers ids also!
    const std::string_view res = mBridge->SendRecv(cmd, _extraPath);

    int outId = -1;
    Pose outPose = Pose::Ident();
    int outStatus = -1;
    VerifyAndParseResponse(res, "trackerpose", outId, outPose, outStatus);
    if (id != outId){
        SimSuit::logFile("From VRDriver.cpp::GetTracker() : Error unexpected tracker id: " +  std::to_string(outId));   
        throw std::runtime_error("unexpected tracker id");
    }
    return GetTrackerResult{outPose, (outStatus == 0)};

#pragma region TESTING
    //comment all function body and uncomment this on testing
    //SimSuit::logFile("From VRDdriver.cpp: attempt GetTracker cmd... id: " + std::to_string(id + 1));
    //const std::string cmd = BuildCommand("gettrackerpose", id, timeOffset);
    //std::string _extraPath = "/tracking/trackers/" + (id == 9 ? "head" : std::to_string(id + 1)); //id is zero indexed but osc tracker paths are 1 indexed and might have names for specific tracers ids also!
    //const std::string_view res = mBridge->SendRecv(cmd, _extraPath);
    //int outId = id;
    //Pose outPose = Pose::Ident();
    //int outStatus = 0;
    ////VerifyAndParseResponse(res, "trackerpose", outId, outPose, outStatus);
    ////if (id != outId) throw std::runtime_error("unexpected tracker id");
    //return GetTrackerResult{outPose, (true)};
#pragma endregion
}

} // namespace tracker
