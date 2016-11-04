#include <QtGui>
#include <QtQml>

#include "DynamicEntryModel.h"
#include "PointsStatus.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // register the type DataEntryModel
    // under the url "org.example" in version 1.0
    // under the name "DataEntryModel"
    qmlRegisterType<PointsStatus>("points.status", 1, 0, "PointsStatus");
    qmlRegisterType<DynamicEntryModel>("main.model", 1, 0, "DynamicEntryModel");
    //DynamicEntryModel::declareQML();

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
