#include "imagelist.h"
#include <QDir>
#include <QDirIterator>
#include <QString>
#include <QTextStream>
#include <QFileInfo>
#include "globals.h"
#include "util.h"

imagelist::imagelist(QString folder){
    this->_list = this->getImagePaths(folder);
    this->_position = 0;
    this->_listsize = this->_list->size();
    this->currentFolder = folder;
}

void imagelist::Seek(int direction, int mode)
{
    int start_position = this->_position;
    if(mode == FilterMode::Cropped){
        do
            this->Cycle(direction);
        while(!QFileInfo::exists(Util::fileGetCsvUrl(this->getPath())) && this->_position != start_position);
    }
    else if(mode == FilterMode::Uncropped){
        do
            this->Cycle(direction);
        while(QFileInfo::exists(Util::fileGetCsvUrl(this->getPath())) && this->_position != start_position);
    }
}

void imagelist::Cycle(int direction)
{
    this->_position += direction;
    if(this->_position < 0)
        this->_position = this->_list->size() - 1;
    else if(this->_position > this->_list->size() - 1)
        this->_position = 0;
}

QString imagelist::getPath(){
    return *this->_list->at(this->_position);
}

QString imagelist::getName(){
    QString path = this->getPath();
    int slashpos = path.lastIndexOf("/");
    int dotpos = path.lastIndexOf(".");
    return(path.mid(slashpos + 1, dotpos - slashpos - 1));
}

QString imagelist::getExtension(){
    QString path = this->getPath();
    QString n = "";
    int dotpos = path.lastIndexOf(".");
    if(dotpos == -1)
        return n;
    else
        return path.right(path.size() - dotpos - 1);
}

QString imagelist::getFolder(){
    return this->currentFolder;
}

int imagelist::getSize(){
    return this->_listsize;
}

bool imagelist::isImage(QString filename){
    QString ext;
    int stringlen = filename.size();
    int dotpos = -1;

    for(int i = 0; i < stringlen; ++i)
        if(filename.at(i) == ".")
            dotpos = i;

    if(dotpos == -1)
        return false;
    for(int i = dotpos + 1; i < stringlen; i++)
        ext.append(filename.at(i));

    if(ext.toLower() == "jpg")
        return true;
    else if(ext.toLower() == "bmp")
        return true;
    else if(ext.toLower() == "png")
        return true;
    else if(ext.toLower() == "jpeg")
        return true;
    else
        return false;
}

QList<QString *> *imagelist::getList() const{
    return this->_list;
}

QList<QString *> *imagelist::getImagePaths(QString path){
    QDirIterator it(path, QDirIterator::NoIteratorFlags);
    QList<QString*> *out = new QList<QString*>;
    while(it.hasNext()){
        QString f = it.next();
        if(isImage(f))
            out->append(new QString(f));
    }
    return out;
}

int imagelist::getPosition(){
    return this->_position;
}

void imagelist::setPosition(int position){
    this->_position = position;
}

imagelist::~imagelist(){
    if(this->_list != nullptr){
        QList<QString *>::iterator i;
        for(i = this->_list->begin(); i != this->_list->end(); ++i)
            delete *i;
        delete this->_list;
    }
}

bool imagelist::isEmpty(){
    return !(this->_list->size() > 0);
}
