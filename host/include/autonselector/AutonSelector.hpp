// Copyright (c) 2016-2020 FRC Team 3512. All Rights Reserved.

#pragma once

#include <stdint.h>

#include <atomic>
#include <chrono>
#include <functional>
#include <mutex>
#include <string>
#include <string_view>
#include <thread>
#include <tuple>
#include <vector>

#include "autonselector/Packet.hpp"
#include "autonselector/UdpSocket.hpp"

namespace frc3512 {
/**
 * The Autonomous Selector is a program we wrote and a protocol we invented to
 * facilitate running multiple autonomous modes, each being selectable from the
 * Driver Station.
 *
 * USAGE:
 * 1) Instantiate AutonSelector with the port on which communications will be
 *    received (probably 1130).
 * 2) Call AddMethod() with each available autonomous mode.
 * 3) Call ExecAutonomousInit() and ExecAutonomousPeriodic() in the init and
 *    periodic functions of the robot class.
 */
class AutonSelector {
public:
    explicit AutonSelector(int port);
    ~AutonSelector();

    AutonSelector(const AutonSelector&) = delete;
    AutonSelector& operator=(const AutonSelector&) = delete;

    /**
     * Adds an autonomous function pair to the selection list.
     */
    void AddMethod(std::string_view methodName, std::function<void()> initFunc,
                   std::function<void()> periodicFunc);

    /**
     * Empties the autonomous mode selection list.
     */
    void DeleteAllMethods();

    /**
     * Runs the init function of the currently selected autonomous mode.
     *
     * Call this in TimedRobot::AutonomousInit().
     */
    void ExecAutonomousInit();

    /**
     * Runs the periodic function of the currently selected autonomous mode.
     *
     * Call this in TimedRobot::AutonomousPeriodic().
     */
    void ExecAutonomousPeriodic();

    /**
     * Override currently selected autonomous mode.
     *
     * This is used for unit testing.
     *
     * @param name Name of autonomous mode.
     */
    void SelectMethod(std::string_view name);

private:
    using steady_clock = std::chrono::steady_clock;

    UdpSocket m_socket;  // socket for sending data to Driver Station
    uint32_t m_dsIP;     // IP address of Driver Station
    int m_dsPort;        // port to which to send data

    // Rate-limits keepalive
    steady_clock::time_point m_prevTime = steady_clock::now();

    // Stores IP address temporarily during receive
    uint32_t m_recvIP;

    // Stores port temporarily during receive
    uint16_t m_recvPort = 0;

    // Buffer for Driver Station requests
    char m_recvBuffer[256];

    // Holds number of bytes received from Driver Station
    size_t m_recvAmount = 0;

    std::vector<
        std::tuple<std::string, std::function<void()>, std::function<void()>>>
        m_autonModes;
    std::atomic<char> m_curAutonMode;

    std::thread m_recvThread;
    std::mutex m_ipMutex;
    std::atomic<bool> m_recvRunning{false};

    /**
     * Sends the given packet to the Driver Station.
     */
    void SendToDS(Packet& packet);

    /**
     * Receives control commands from Driver Station and processes them.
     */
    void ReceiveFromDS();
};
}  // namespace frc3512
