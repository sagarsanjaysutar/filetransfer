#include "NetworkManager.h"
#include "ReceivingServer.h"
#include "SFTPAccess.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTextStream>
#include <QFile>
#include <QMap>
#include <QJsonObject>

int main(int argc, char *argv[]){

    // Standard Qt/QML Configuration
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    SFTPAccess obj;
    return 0;
    NetworkManager discovery;
    ReceivingServer receivingServer(discovery.getLocalHostInterface());

    // Expose backend objects to QML
    QQmlContext *ctxt = engine.rootContext();
    QVector<QQmlContext::PropertyPair> qmlProperties;
    qmlProperties.push_back(QQmlContext::PropertyPair{"discovery", QVariant::fromValue(&discovery)});
    qmlProperties.push_back(QQmlContext::PropertyPair{"receivingServer", QVariant::fromValue(&receivingServer)});
    ctxt->setContextProperties(qmlProperties);

    engine.load(QUrl(QStringLiteral("qrc:qml/main.qml")));

    return app.exec();
}
