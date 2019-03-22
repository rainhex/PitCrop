#pragma once
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>

class CropBox;

class ResizeBox : public QGraphicsRectItem
{
public:
    ResizeBox(int x, int y, int w, int h,   CropBox *parent);
    void updatePosition(qreal x, qreal y);

    void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
private:
    CropBox *_parent;
};
