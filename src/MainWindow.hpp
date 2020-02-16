// Copyright (c) 2020 FRC Team 3512. All Rights Reserved.

#pragma once

#include <stdint.h>

#include <string>
#include <vector>

#include <QComboBox>
#include <QHostAddress>
#include <QMainWindow>
#include <QTimer>
#include <QUdpSocket>
#include <QVBoxLayout>

#include "Settings.hpp"

/**
 * Creates application's main window
 */
class MainWindow : public QMainWindow {
    Q_OBJECT
    QWidget centralWidget{this};

public:
    MainWindow();

private slots:
    void handleSocketData();

private:
    static constexpr int kTimeout = 500;

    Settings m_settings{"IPSettings.txt"};

    // Allows the user to select which autonomous mode the robot should run
    QComboBox m_autoSelect;

    QUdpSocket m_dataSocket{this};
    QHostAddress m_remoteIP;
    uint16_t m_dataPort;
    bool m_connectDlgOpen{false};
    bool m_connectedBefore{false};

    std::vector<char> m_buffer;

    QTimer m_connectTimer{this};

    static bool packetToVar(const std::vector<char>& data, size_t& pos,
                            std::string& dest);
};
