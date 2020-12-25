#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "backend.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    // allocate example before the engine to ensure that it outlives it
    QScopedPointer<SingletonEpubReader> reader(new SingletonEpubReader);

    QQmlApplicationEngine engine;
    // Third, register the singleton type provider with QML by calling this
    // function in an initialization function.
    qmlRegisterSingletonInstance("Qt.Reader", 1, 0, "MyApi", reader.get());

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
