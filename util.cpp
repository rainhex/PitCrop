#include <QDir>
#include <QDirIterator>
#include <QTextStream>
#include <QFile>
#include <QStringList>
#include <QPixmap>
#include <QImage>
#include <QMatrix>
#include "util.h"

namespace Util{
    void loadListFromText(QList<QString> *v, QString filepath){
        if(v == nullptr)
            return;
        QFile f(filepath);
        if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
            return;
        while(!f.atEnd())
            v->append(f.readLine().simplified());
        f.close();
    }

    QString fileGetName(QString path){
        int slashpos = path.lastIndexOf("/");
        int dotpos = path.lastIndexOf(".");
        return(path.mid(slashpos + 1, dotpos - slashpos - 1));
    }

    QString folderGetName(QString path){
        int slashpos = path.lastIndexOf("/");
        int n = path.size() - slashpos;
        return(path.right(n));
    }

    QString folderGetOuterPath(QString path){
        int slashpos = path.lastIndexOf("/");
        int n = path.size() - slashpos;
        return(path.left(path.size() - n));
    }

    QString fileGetCsvUrl(QString path)
    {
        QString img_folder_path = Util::fileGetFolder(path);
        return Util::folderGetOuterPath(img_folder_path )
                + Util::folderGetName(img_folder_path) + "_csv/"
                + Util::fileGetName(path) + "."
                + Util::fileGetExtension(path) + ".csv";
    }

    QImage *getRotated(QImage *src, qreal degrees){
        QPoint center = src->rect().center();
        QMatrix matrix;
        matrix.translate(center.x(), center.y());
        matrix.rotate(degrees);
        QImage dstImg = src->transformed(matrix);
        return new QImage(dstImg);
    }

    QString fileGetExtension(QString path){
        QString n = "";
        int dotpos = path.lastIndexOf(".");
        if(dotpos == -1)
            return n;
        else
            return path.right(path.size() - dotpos - 1);
    }

    QString fileGetFolder(QString url){
        int slashpos = url.lastIndexOf("/");
        return url.left(slashpos);
    }

    int clamp(int value, int min, int max){
        if(value < min)
            return min;
        if(value > max)
            return max;
        return value;
    }
}
