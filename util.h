#pragma once
#include <QList>
#include <QString>
#include <QGraphicsPixmapItem>

namespace Util
{
    void loadListFromText(QList<QString> *v, QString filepath);
    void processCrops(QString directory);
    QString fileGetName(QString url);
    QString fileGetExtension(QString url);
    QString fileGetFolder(QString url);
    QString folderGetName(QString path);
    QString folderGetOuterPath(QString path);
    QString fileGetCsvUrl(QString path);
    QImage *getRotated(QImage *src, qreal degrees);
    int clamp(int value, int min, int max);
};
