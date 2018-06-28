#include <QDir>
#include <QDirIterator>
#include <QTextStream>
#include <QFile>
#include <QStringList>
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
