/*
 *  This file is part of LesJeuxDAntoine.
 *
 *  LesJeuxDAntoine is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  EasyTimeTracker is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with EasyTimeTracker.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  LesJeuxDAntoine: First creation by 2BlackCoffees: http://www.twoblackcoffees.com/
 *
 **/
#include <QtGui>
#include <QtQml>

#include "DynamicEntryModel.h"
#include "PointsStatus.h"
#include "QMLInterface.h"
#include "TraceDebug.hpp"
int main(int argc, char *argv[])
{
    START_TRACE_PERFORMANCE(main);
    QGuiApplication app(argc, argv);

    // register the type DataEntryModel
    // under the url "org.example" in version 1.0
    // under the name "DataEntryModel"
    ADD_TRACE_PERFORMANCE(main, "qmlRegisterType<QMLInterface>(QMLInterface)");
    qmlRegisterType<QMLInterface>("QMLInterface", 1, 0, "QMLInterface");
    ADD_TRACE_PERFORMANCE(main, "qmlRegisterType<QMLInterface>(PointsStatus)");
    qmlRegisterType<PointsStatus>("PointsStatus", 1, 0, "PointsStatus");
    ADD_TRACE_PERFORMANCE(main, "qmlRegisterType<QMLInterface>(DynamicEntryModel)");
    qmlRegisterType<DynamicEntryModel>("ModelCards", 1, 0, "DynamicEntryModel");

    QQmlApplicationEngine engine;
    ADD_TRACE_PERFORMANCE(main, "Loading QML");
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
