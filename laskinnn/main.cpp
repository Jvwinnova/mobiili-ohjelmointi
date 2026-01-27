#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml/qqml.h>

#include "calculator.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    Calculator calculator;
    QQmlApplicationEngine engine;

    // Expose the Calculator instance to QML
    engine.rootContext()->setContextProperty("calculator", &calculator);

    // Register enum-only type for use in QML
    qmlRegisterUncreatableType<Calculator>(
        "CalculatorEnums", 1, 0, "Calculator", "Enum only"
        );

    // Connect to object creation failure (Android-safe)
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection
        );

    // Load QML module (Android-safe)
    engine.loadFromModule("laskinnn", "Main");

    return app.exec();
}
