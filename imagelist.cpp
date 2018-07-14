#include "imagelist.h"
#include <QDir>
#include <QDirIterator>
#include <QString>
#include <QTextStream>

imagelist::imagelist(QString folder){
    this->_list = this->getImagePaths(folder);
    this->_position = 0;
    this->currentFolder = folder;
}

void imagelist::Next(){
    if(this->_position < (unsigned int)this->_list->size() - 1)
        this->_position++;
    else
        this->_position = 0;
}

void imagelist::Previous(){
    if(this->_position > 0)
        this->_position--;
    else
        this->_position = (int)this->_list->size() - 1;
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

unsigned int imagelist::getPosition(){
    return this->_position;
}

void imagelist::setPosition(unsigned int position){
    this->_position = position;
}

imagelist::~imagelist(){
    if(this->_list != NULL){
        QList<QString *>::iterator i;
        for(i = this->_list->begin(); i != this->_list->end(); ++i)
            delete *i;
        delete this->_list;
    }
}

bool imagelist::isEmpty(){
    return !(this->_list->size() > 0);
}
