// Copyright (c) 2020 FRC Team 3512. All Rights Reserved.

#include <chrono>
#include <iostream>
#include <thread>

#include "autonselector/AutonSelector.hpp"

using namespace std::chrono_literals;

class Robot {
public:
    void initFunc1() { std::cout << "Auto Init 1" << std::endl; }

    void initFunc2() { std::cout << "Auto Init 2" << std::endl; }

    void initFunc3() { std::cout << "Auto Init 3" << std::endl; }

    void periodicFunc1() { std::cout << "Auto Periodic 1" << std::endl; }

    void periodicFunc2() { std::cout << "Auto Periodic 2" << std::endl; }

    void periodicFunc3() { std::cout << "Auto Periodic 3" << std::endl; }
};

int main() {
    frc3512::AutonSelector autonSelector(5800);
    Robot robot;

    autonSelector.AddMethod("Auto 1", std::bind(&Robot::initFunc1, &robot),
                            std::bind(&Robot::periodicFunc1, &robot));
    autonSelector.AddMethod("Auto 2", std::bind(&Robot::initFunc2, &robot),
                            std::bind(&Robot::periodicFunc2, &robot));
    autonSelector.AddMethod("Auto 3", std::bind(&Robot::initFunc3, &robot),
                            std::bind(&Robot::periodicFunc3, &robot));

    while (1) {
        std::this_thread::sleep_for(100ms);
    }
}
