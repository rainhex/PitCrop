#pragma once
#include <QString>
#include <QList>

class imagelist
{
public:
    enum FilterMode {All, Cropped, Uncropped};
    imagelist(QString folder);
    ~imagelist();
    int getPosition();
    void setPosition(int position);
    void Seek(int direction, int mode);
    void Cycle(int direction);
    QString getPath();
    QString getName();
    QString getExtension();
    QString getFolder();
    int getSize();
    bool isEmpty();
    QList<QString *> *getList() const;

private:
    QList<QString*> *getImagePaths(QString path);
    bool isImage(QString filename);
    int _position;
    int _listsize;
    QList<QString*> *_list;
    QString currentFolder;
};
