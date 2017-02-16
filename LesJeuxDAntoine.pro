#-------------------------------------------------
#
# Project created by QtCreator 2016-10-03T14:07:29
#
#-------------------------------------------------

QT       += core gui
QT       += qml quick
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LesJeuxDAntoine
TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    DynamicEntryModel.cpp \
    PointsStatus.cpp \
    Model.cpp \
    QMLInterface.cpp \
    TraceDebug.cpp

HEADERS  += mainwindow.h \
    DynamicEntryModel.h \
    PointsStatus.h \
    Model.h \
    QMLInterface.h \
    TraceDebug.hpp

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
