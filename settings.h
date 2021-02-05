#pragma once
#include <QString>
#include <QSettings>
#include <QVariant>

class Settings
{
public:
    static QVariant Get(QString name);
    static void Set(QString name, QVariant value);
private:
    static void initialize();
    static QSettings *_settings;
};
