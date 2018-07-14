#include <QDir>
#include <QDirIterator>
#include <QTextStream>
#include <QFile>
#include <QStringList>
#include <QPixmap>
#include <QImage>
#include <QMatrix>
#include "util.h"

void Util::loadListFromText(QList<QString> *v, QString filepath){
    if(v == NULL)
        return;
    QFile f(filepath);
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    while(!f.atEnd())
        v->append(f.readLine().simplified());
    f.close();
}

QString Util::fileGetName(QString path){
    int slashpos = path.lastIndexOf("/");
    int dotpos = path.lastIndexOf(".");
    return(path.mid(slashpos + 1, dotpos - slashpos - 1));
}

QString Util::folderGetName(QString path){
    int slashpos = path.lastIndexOf("/");
    int n = path.size() - slashpos;
    return(path.right(n));
}

QString Util::folderGetOuterPath(QString path){
    int slashpos = path.lastIndexOf("/");
    int n = path.size() - slashpos;
    return(path.left(path.size() - n));
}

QImage *Util::getRotated(QImage *src, qreal degrees){
    QPoint center = src->rect().center();
    QMatrix matrix;
    matrix.translate(center.x(), center.y());
    matrix.rotate(degrees);
    QImage dstImg = src->transformed(matrix);
    return new QImage(dstImg);
}

QString Util::fileGetExtension(QString path){
    QString n = "";
    int dotpos = path.lastIndexOf(".");
    if(dotpos == -1)
        return n;
    else
        return path.right(path.size() - dotpos - 1);
}

QString Util::fileGetFolder(QString url){
    int slashpos = url.lastIndexOf("/");
    return url.left(slashpos);
}

int Util::clamp(int value, int min, int max){
    if(value < min)
        return min;
    if(value > max)
        return max;
    return value;
}
