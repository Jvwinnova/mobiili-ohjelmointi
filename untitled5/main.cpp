#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>   // Requires Qt Quick Controls 2 linked

#include "player.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // 🔹 REQUIRED on Android: force a Qt Quick Controls style
    QQuickStyle::setStyle("Material");  // Use "Basic" if you want minimal

    QQmlApplicationEngine engine;

    // 🔹 Fail-safe: exit if QML fails to load
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection
        );

    // 🔹 Expose Player to QML safely (heap allocation, parent = engine)
    Player *player = new Player(&engine);
    engine.rootContext()->setContextProperty("player", player);

    // 🔹 Load QML module
    engine.loadFromModule("untitled5", "Main");

    // 🔹 Safety check
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
