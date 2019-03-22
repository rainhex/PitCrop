#include "settings.h"
#include "globals.h"

QSettings *Settings::_settings = nullptr;

QVariant Settings::Get(QString name){
    if(_settings == nullptr)
        initialize();
    return _settings->value(name);
}

void Settings::Set(QString name, QVariant value){
    if(_settings == nullptr)
        initialize();
    _settings->setValue(name, value);
}

void Settings::initialize(){
#ifdef _WIN32
    _settings = new QSettings(QSettings::IniFormat, QSettings::SystemScope, gOrganization, gAppName);
#else
    _settings = new QSettings();
#endif
}
