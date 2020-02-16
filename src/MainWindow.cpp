// Copyright (c) 2020 FRC Team 3512. All Rights Reserved.

#include "MainWindow.hpp"

#include <QMessageBox>
#include <QtEndian>

MainWindow::MainWindow() : m_buffer(0xffff - 28) {
    setCentralWidget(&centralWidget);

    // Make new combo box selection trigger UDP send
    connect(&m_autoSelect,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::activated),
            [this](int index) {
                char data[16] = "autonSelect\r\n";
                data[13] = index;

                m_dataSocket.writeDatagram(data, sizeof(data), m_remoteIP,
                                           m_dataPort);
            });

    // Add combo box to layout, then layout to central widget
    auto layout = new QVBoxLayout();
    layout->addWidget(&m_autoSelect);
    layout->setAlignment(Qt::AlignTop);
    centralWidget.setLayout(layout);

    // Configure UDP receive
    m_dataSocket.bind(m_settings.getInt("dsDataPort"));
    connect(&m_dataSocket, SIGNAL(readyRead()), this, SLOT(handleSocketData()));

    // Retrieve remote IP and port from settings file
    m_remoteIP = QHostAddress{
        QString::fromUtf8(m_settings.getString("robotIP").c_str())};
    m_dataPort = m_settings.getInt("robotDataPort");

    // Configure connection timeout
    connect(&m_connectTimer, &QTimer::timeout, [this] {
        if (!m_connectDlgOpen) {
            char data[16] = "connect\r\n";
            m_dataSocket.writeDatagram(data, sizeof(data), m_remoteIP,
                                       m_dataPort);
        }

        m_connectTimer.start(kTimeout);
    });
    m_connectTimer.start(kTimeout);
}

void MainWindow::handleSocketData() {
    while (m_dataSocket.hasPendingDatagrams()) {
        size_t packetPos = 0;
        m_buffer.resize(m_dataSocket.pendingDatagramSize());
        m_dataSocket.readDatagram(m_buffer.data(), m_buffer.size());

        std::string header;
        packetToVar(m_buffer, packetPos, header);

        /* If this instance has connected to the server before, receiving any
         * packet resets the timeout. This check is necessary in case a previous
         * instance caused packets to be redirected here.
         */
        if (m_connectedBefore) {
            m_connectTimer.start(kTimeout);
        }

        if (header == "autonList\r\n") {
            if (!m_connectedBefore) {
                m_connectedBefore = true;
            }
            /* Unpacks the following variables:
             *
             * Autonomous Modes (contained in rest of packet):
             * std::string: autonomous routine name
             * <more autonomous routine names>...
             */

            std::vector<std::string> autoNames;
            std::string autoName;
            while (packetPos < m_buffer.size() &&
                   packetToVar(m_buffer, packetPos, autoName)) {
                autoNames.emplace_back(autoName);
            }

            m_autoSelect.clear();
            for (auto& str : autoNames) {
                m_autoSelect.addItem(str.c_str());
            }
        } else if (header == "autonConfirmed\r\n") {
            /* If a new autonomous mode was selected from the robot, it
             * sends back this packet as confirmation
             */
            std::string autoName = "Autonomous mode changed to\n";

            std::string tempName;
            packetToVar(m_buffer, packetPos, tempName);
            autoName += tempName;

            int idx = m_autoSelect.findText(QString::fromStdString(tempName));
            if (idx != -1) {
                m_autoSelect.setCurrentIndex(idx);
            }

            QMessageBox* connectDlg = new QMessageBox(this);
            connectDlg->setAttribute(Qt::WA_DeleteOnClose);
            connectDlg->setWindowTitle("Autonomous Change");
            connectDlg->setText(autoName.c_str());
            connectDlg->open();
        }
    }
}

bool MainWindow::packetToVar(const std::vector<char>& data, size_t& pos,
                             std::string& dest) {
    if (pos + sizeof(uint32_t) <= data.size()) {
        uint32_t size = qFromBigEndian<uint32_t>(
            reinterpret_cast<const uint8_t*>(&data[pos]));
        pos += sizeof(size);

        if (pos + size <= data.size()) {
            dest.assign(&data[pos], size);
            pos += size;

            return true;
        }
    }

    /* Return false if either size of string or string data couldn't be
     * extracted
     */
    return false;
}
