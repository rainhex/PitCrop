#pragma once
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QPen>
#include <QKeyEvent>

class CropBox : public QGraphicsItem
{
public:
    CropBox(int x, int y, int width, int height);
    ~CropBox();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    void Redimension(QGraphicsSceneMouseEvent *e);
    void resize(int new_w, int new_h);
    bool isActive();
    void activate();
    void deactivate();

    //edge functions
    int right();
    int left();
    int top();
    int bottom();
    int width();
    int height();
    void setQualityIndex(int index);
    void setTagIndex(int index);
    int getQualityIndex();
    int getTagIndex();
    void increaseSize();
    void decreaseSize();
private:
    int _width;
    int _height;
    bool _enabled;
    QPen _pen;
    int category_index;
    int quality_index;
    int self_multiplier;
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
    void mousePressEvent(QGraphicsSceneMouseEvent *e);
};
