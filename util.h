#pragma once
#include <QList>
#include <QString>

class Util
{
public:
    static void loadListFromText(QList<QString> *v, QString filepath);
    static void processCrops(QString directory);
    static QString fileGetName(QString url);
    static QString fileGetExtension(QString url);
    static QString fileGetFolder(QString url);
    static QString folderGetName(QString path);
    static QString folderGetOuterPath(QString path);
};
