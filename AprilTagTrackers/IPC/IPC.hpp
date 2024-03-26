#pragma once

#include "utils/Assert.hpp"
#include "utils/Types.hpp"
#include <atomic>
#include <array>
#include <memory>
#include <string>
#include <string_view>

#include "OSC_IPC.hpp"

namespace IPC
{

template <Index NSize>
class IOBuffer
{
public:
protected:
    constexpr std::string_view GetBufferStringView(Index stringLength) const
    {
        ATT_ASSERT(stringLength >= 0);
        ATT_ASSERT(stringLength <= NSize);
        return std::string_view(mBuffer.data(), stringLength);
    }
    constexpr char* GetBufferPtr() { return mBuffer.data(); }
    constexpr Index GetBufferSize() const { return NSize - 1; }

private:
    std::array<char, NSize> mBuffer{};
};

class IConnection : public IOBuffer<1024>
{
public:
    virtual ~IConnection() = default;
    virtual void Send(std::string_view message) = 0;
    /// @return temporary view of buffer, invalidated when SendRecv is called again
    [[nodiscard]] virtual std::string_view Recv() = 0;
};

// Interface for inter-process-communication, be that over network, udp, or pipes, multithreaded or not
class IServer
{
public:
    virtual ~IServer() = default;
    [[nodiscard]] virtual std::unique_ptr<IConnection> Accept() = 0;
};

// Interface for inter-process-communication, be that over network, udp, or pipes, multithreaded or not
class IClient : public IOBuffer<1024>
{
public:
    virtual ~IClient() = default;
    /// @return temporary view of buffer, invalidated when SendRecv is called again
    [[nodiscard]] virtual std::string_view SendRecv(std::string message) = 0;
};


//OSC class starts init/connect process of our OSC_noSteam client
//why to make another caller of osc while we have the OSC class?
//this totally separates our client/server
//implementation of Reality Simsuit noSteam connection and preserves coherence in ATT source code
//all ATT source code business ends when calling IPC::OSC::SendRecv() which calls our OSC_noSteamGateway()
//HAS our Target side OSC and actual OSC objects
class OSC : public IClient
{

public:
    //one invoke == (one send and one recv).
    //even if failed after invoke it counts as an invoke!
    static std::atomic<int> InvokesCount; //only one copy initiated with first instance and shared with all objects

    OSC(std::string rootPathTargetOSC); //rootPathTargetOSC="AprilTagPipeIn
    //OSC(std::string rootPathTargetOSC, std::string pathAprilSideOSC, std::string ipAprilSideOSC, std::string portAprilSideOSC);
    std::string_view SendRecv(std::string message) final;

    SimSuit::OSC_noSteamGateway mGateway;

private:
    //m prefix for member var
    std::string mPathTargetOSC; ///TODO: this supposed to be with other target vars in OSC_noSteamGateway (resolve dependencies first!)

    std::string mPathAprilOSC;
    std::string mIpAprilSideOSC;
    std::string mPortAprilSideOSC;

};

class WindowsNamedPipe : public IClient
{
public:
    explicit WindowsNamedPipe(std::string pipeName);

    std::string_view SendRecv(std::string message) final;

private:
    std::string mPipeName;
};

class UNIXSocket : public IClient
{
public:
    explicit UNIXSocket(std::string socketName);

    std::string_view SendRecv(std::string message) final;

private:
    std::string mSocketPath;
};

//check its implementation and reference in VRDriver.h
inline std::unique_ptr<IClient> CreateDriverClient()
{
    const std::string driverPath = "ApriltagPipeIn"; //ORSNARO NOTE: will prepend `/` before the path for OSC client!
#if defined(ATT_OS_WINDOWS) && !defined(FORCE_OSC)
    return std::make_unique<WindowsNamedPipe>(driverPath);
#elif defined(ATT_OS_WINDOWS) && defined(FORCE_OSC)
    //ORSNARO EDIT
    SimSuit::logFile("From IPC.hpp CreatDriverclient() is Invoked!: `return std::make_unique<OSC>(driverPath);` ");
    return std::make_unique<OSC>(driverPath);
#else
    return std::make_unique<UNIXSocket>(driverPath);
#endif
}

[[nodiscard]] std::unique_ptr<IServer> CreateDriverServer(); // apparently only used for testing and debugging

}; // namespace IPC
