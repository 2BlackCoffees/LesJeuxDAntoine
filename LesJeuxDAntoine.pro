#-------------------------------------------------
#
# Project created by QtCreator 2016-10-03T14:07:29
#
#-------------------------------------------------

QT       += core gui
QT       += qml quick
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MemoryTest
TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    DynamicEntryModel.cpp \
    PointsStatus.cpp \
    Model.cpp \
    QMLInterface.cpp

HEADERS  += mainwindow.h \
    DynamicEntryModel.h \
    PointsStatus.h \
    Model.h \
    QMLInterface.h

FORMS    += mainwindow.ui

DISTFILES += \
    main.qml \
    PointsMoving.qml \
    StatusDialog.qml \
    LocalToolbar.qml \
    ParticlesInSquare.qml \
    CurtainEffectLeft.qml \
    CurtainEffectRight.qml

#CONFIG+=qml_debug
#QML2_IMPORT_PATH = C:\Full-Backup\Development\CarMemoryTest\MemoryTest

RESOURCES += \
    resources.qrc
