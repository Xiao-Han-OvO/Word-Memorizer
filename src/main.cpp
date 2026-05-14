#include "AppController.h"

#include <QCoreApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setOrganizationName(QStringLiteral("ArenaAI"));
    app.setApplicationName(QStringLiteral("VocabMemster"));
    app.setApplicationVersion(QStringLiteral("2.0.0"));

    QQuickStyle::setStyle(QStringLiteral("Material"));

    qmlRegisterUncreatableType<WordListModel>("WordMemorizer", 1, 0, "WordListModel",
                                             QStringLiteral("由 C++ AppController 提供实例"));

    AppController controller;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty(QStringLiteral("appController"), &controller);

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
                     &app, []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);

    engine.loadFromModule("WordMemorizer", "Main");

    return app.exec();
}
