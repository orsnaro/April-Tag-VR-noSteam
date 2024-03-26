
# if defined(ATT_OS_WINDOWS) && defined(FORCE_OSC)
#include "OSC_IPC.hpp"
#include "IPC.hpp"

///TODO: check if this CMake MACROS actually works
namespace SimSuit //if used unnamed namespaces makes it only accessible in this file!
{
 OSC_noSteamGateway::OSC_noSteamGateway(std::string portTargetOSC, std::string ipTargetOSC) : mPortTargetOSC(portTargetOSC), mIpTargetOSC(ipTargetOSC)
{
    //vector can be replaced with Pose struct {pos,quat_rot}
    //Pose vector has 7 elements: 3 for posVec and 4 for quaternion_rotation_Vec
    this->trackersInfoMap = //last path  will be the tracker ip parsed from SendRecv()
        {
            {"/tracking/trackers/1", {.0f, .0f, .0f, .0f, .0f, .0f, .0f}}, //paths to head: gets/updates trackers data (those also the official VRchat paths)
            {"/tracking/trackers/2", {.0f, .0f, .0f, .0f, .0f, .0f, .0f}},
            {"/tracking/trackers/3", {.0f, .0f, .0f, .0f, .0f, .0f, .0f}},
            {"/tracking/trackers/4", {.0f, .0f, .0f, .0f, .0f, .0f, .0f}},
            {"/tracking/trackers/5", {.0f, .0f, .0f, .0f, .0f, .0f, .0f}},
            {"/tracking/trackers/6", {.0f, .0f, .0f, .0f, .0f, .0f, .0f}},
            {"/tracking/trackers/7", {.0f, .0f, .0f, .0f, .0f, .0f, .0f}},
            {"/tracking/trackers/8", {.0f, .0f, .0f, .0f, .0f, .0f, .0f}},
            {"/tracking/trackers/head", {.0f, .0f, .0f, .0f, .0f, .0f, .0f}},
            {"/tracking/trackers/add", {.0f, .0f, .0f, .0f, .0f, .0f, .0f}},
            {"/tracking/station/1", {.0f, .0f, .0f, .0f, .0f, .0f, .0f}}, //path of UpdateStation cmd
            {"/tracking/settings/", {.0f, .0f, .0f, .0f, .0f, .0f}}, //path of SetSmoothing cmd
            {"/tracking/trackers/count", {0}}};
}

#pragma region TESTING
SimSuit::suitBuffer& OSC_noSteamGateway::runTst(std::string msg, lo::ServerThread& aprilServerTh, lo::Address& addressAprilSideOSC, std::string& pathTargetOSC, suitBuffer& buffer)
{
    //START FOR TESTING FUNCTION runTst ONLY
    std::string testingTargetPath = "/testing";
    auto fullTargetPath = pathTargetOSC + testingTargetPath; // full target path : "/AprilTagPipeIn/testing"
    //END: FOR TESTING FUNCTION runTst ONLY

    if (!aprilServerTh.is_valid())//we checked before! amm it's ok double check :)!
    {
        //ERROR
        std::string err_msg = "ERROR! Couldn't Start April's Side server thread!";
        buffer.state = States::FAIL;
        buffer.data = "";
        logFile(err_msg);
        std::cout << err_msg << std::endl;
        return buffer; // no need since buffer is already passed by reference (just to terminate)
    }

    lo::Message msgOSC;
    msgOSC.add_string(msg); //for multiple args use general add function e.g.(   msgOSC.add_varargs("icf", 121, 'q', 0.3)  )

    lo::Address addressTargetOSC(mIpTargetOSC, mPortTargetOSC);

    //lo::Address::send_from() doesn't take the target address(ip,port) argument? why? cuz it has it internally! 
    //buffer.state = (addressTargetOSC.send_from(aprilServerTh, fullTargetPath, "s", msg) < 0 ? States::FAIL : States::PENDING ); 
    buffer.state = (addressTargetOSC.send(fullTargetPath, msgOSC) < 0 ? States::FAIL : States::PENDING ); 
    //send_from() returns -1 on fail (change pending state to FAIL if -1 )
    //but if sending succeeded don't update to States::OK ! April receiver in `OSC` class will update to `OK` state when one send-receive cycle is done fully successfully)

    return buffer;// no need since buffer is already passed by reference
}
#pragma endregion

/// TODO: each time it's invoked sends 1 or 2 msgs max
/// TODO: update trackersInfoMap before sending to keep track on changes in both sides
SimSuit::suitBuffer& OSC_noSteamGateway::run(std::string msg, lo::ServerThread& aprilServerTh, lo::Address& addressAprilSideOSC, std::string& pathTargetOSC, suitBuffer& buffer)
{
    // HERE CALL YOUR IMPLEMENTATION OF OSC SERVER wither using liblo OSC or else...
    //BOTH SIDES WILL HAVE SERVER THREAD SO WE CAN REPLY and have bi directional connection

    return buffer;
}

    //rest of members are declared in OSC_IPC.hpp

} // namespace SimSuit


std::atomic<int> IPC::OSC::InvokesCount; //static member (one copy shared across all objects in global scope)
namespace IPC
{

//one invoke == (one send and one recv). 
//even if failed after invoke it counts as an invoke!
// 
//has our AprilSideOSC (OSC class defs)

///TODO: test Ipv4 ips like `192.168.1.X`
OSC::OSC(std::string rootPathTargetOSC) : mPathTargetOSC('/' + rootPathTargetOSC),
                                                mPathAprilOSC("/ApriltagPipeBack"),
                                                mIpAprilSideOSC("192.168.1.3"),
                                                mPortAprilSideOSC("9876") 
{
    SimSuit::OSC_noSteamGateway mGateway;
    
}
    

    //OSC(std::string rootPathTargetOSC, std::string pathAprilSideOSC, std::string ipAprilSideOSC, std::string portAprilSideOSC) : mPathTargetOSC('/' + RootPathTargetOSC), /*mPathTargetOSC = "/AprilTagPipeIn*/
    //                                                                                                                             mPathAprilOSC("/AprilTagPipeBack"),
    //                                                                                                                             mIpAprilSideOSC("127.0.0.1"),
    //                                                                                                                             mPortAprilSideOSC("9876") 
    //{
    //    SimSuit::OSC_noSteamGateway mGateway;
    //}

    std::string_view OSC::SendRecv(std::string message)
    {
        SimSuit::suitBuffer gatewayRet{"", SimSuit::States::PENDING}; //Initial values for new Connection
        
        //HERE init your April side server using any C++ OSClibrary is ok
        lo::ServerThread aprilSideServerTh(NULL, mPortAprilSideOSC, NULL, mIpAprilSideOSC, NULL);
        if(!aprilSideServerTh.is_valid())
        {
            std::string err_msg = "ERROR! Couldn't Create April's side Server thread!";
            SimSuit::logFile(err_msg);
            std::cout << err_msg << std::endl;
            gatewayRet.state = SimSuit::States::FAIL;
            return "-1"; //ERROR
        }

        lo::Address addressAprilSideOSC(mIpAprilSideOSC, mPortAprilSideOSC);
        aprilSideServerTh.set_callbacks([&aprilSideServerTh, this]() 
                                        {
                                            std::stringstream thAdd;
                                            thAdd << &aprilSideServerTh;

                                            std::string stinv_msg = "InvokeID: " + std::to_string(InvokesCount) /*start invokation msg*/
                                            + " INITIALIZED! AprilTag SIDE OSC Server Thread with mem.address: " + thAdd.str(); 
                                            SimSuit::logFile(stinv_msg);
                                            std::cout << stinv_msg; 
                                        },
                                        [this]() 
                                        { 
                                            std::string endinv_msg = "InvokeID: " + std::to_string(InvokesCount) + " TERMINATED! AprilTag SIDE OSC Server Thread";
                                            SimSuit::logFile(endinv_msg);
                                            std::cout << endinv_msg; 
                                        }
        );

        //liblo OSC  string types are marked with: 's' or 'S'
        aprilSideServerTh.add_method(mPathAprilOSC, "s",
                                     [&gatewayRet, this](lo_arg** argv, int argc) 
                                     {
                                         std::string cString_received = &argv[0]->s;

                                         std::string recvMsg = "path: " + mPathAprilOSC + " at April Side Received MSG: " + "\"" + cString_received + "\"";
                                         SimSuit::logFile(recvMsg);
                                         std::cout << recvMsg << '\n';

                                         gatewayRet.data = cString_received;
                                         gatewayRet.state = SimSuit::States::OK; //only when one invoke cycle is done OK ( one send one recieve )
                                         fflush(stdout);
                                     }
        ); 


        aprilSideServerTh.start();

        #pragma region TESTING2
        //START: FOR TESTING FUNCTION runTst ONLY
        mGateway.runTst(message, aprilSideServerTh, addressAprilSideOSC, mPathTargetOSC, gatewayRet);
        //END: FOR TESTING FUNCTION runTst ONLY
        #pragma endregion

        //mGateway.run(message, &aprilSideServerTh, &addressAprilSideOSC, mPathTargetOSC, suitBuffer & buffer);
        InvokesCount++;

        //why timeout==500? 5sec 10*500 = 5000ms
        int timeout = SimSuit::waitLoop(gatewayRet, 500);

        //stop to handle the slight chance that Target responds immediately after timing out
        aprilSideServerTh.stop();
        std::string timeoutMsg = "from OSC.hpp SendRecv() Invoke ID: " + std::to_string(InvokesCount) + " " + (timeout > 0 ? "April OSC SIDE GOT Response in Time!" : "TimedOut! OR FAILED due to other issue");
        SimSuit::logFile(timeoutMsg);
        std::cout << timeoutMsg << std::endl;

        if (gatewayRet.state == SimSuit::States::OK and gatewayRet.data != "" and timeout > 0)
        {
            SimSuit::logFile("Invoke ID: " + std::to_string(InvokesCount) + " " + "PASSED initial check! validating the response...");
            std::string msgFromTarget = gatewayRet.data;
            return msgFromTarget;
        }//else
        return "-1"; //FAILED or timed out 
    }

    #pragma region AprilSideCallbackMethods
    //AprilTag Side OSC callback METHOD (for big methods that not good in lambda functions)
    ///TODO: for now we just propagate back the result of each command to AprilTag base code
    ///later: we need to catch the Success message 
    /// e.g. fields like count from `GetTrackerCount` and Pose from `GetTracker`
    
    #pragma endregion 

} // namespace IPC

#endif