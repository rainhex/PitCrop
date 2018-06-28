#pragma once
#include <QString>
#include <QList>

class imagelist
{
public:
    imagelist(QString folder);
    ~imagelist();
    unsigned int getPosition();
    void setPosition(unsigned int position);
    void Next();
    void Previous();
    QString getPath();
    QString getName();
    QString getExtension();
    QString getFolder();
    bool isEmpty();
    QList<QString *> *getList() const;

private:
    QList<QString*> *getImagePaths(QString path);
    bool isImage(QString filename);
    unsigned int _position;
    QList<QString*> *_list;
    QString currentFolder;
};
