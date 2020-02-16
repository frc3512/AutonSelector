// Copyright (c) 2020 FRC Team 3512. All Rights Reserved.

#include <QApplication>
#include <QDesktopWidget>
#include <QIcon>

#include "MainWindow.hpp"

int main(int argc, char* argv[]) {
    Q_INIT_RESOURCE(AutonSelector);

    QApplication app(argc, argv);
    app.setOrganizationName("FRC Team 3512");
    app.setApplicationName("Auton Selector");

    MainWindow mainWin;
    mainWin.setWindowIcon(QIcon(":/images/Spartatroniks.ico"));
    mainWin.show();

    return app.exec();
}
