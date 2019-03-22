#pragma once
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QPen>
#include <QKeyEvent>
#include "resizebox.h"

class CropBox : public QGraphicsItem
{
public:
    CropBox(qreal x, qreal y, qreal width, qreal height);
    ~CropBox();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    void resize(qreal new_w, qreal new_h);
    bool isActive();
    void activate();
    void deactivate();
    ResizeBox *getRB();

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
    ResizeBox _rb;
    int category_index;
    int quality_index;
    int self_multiplier;
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
    void mousePressEvent(QGraphicsSceneMouseEvent *e);
};
