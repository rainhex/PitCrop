#include "resizebox.h"
#include "globals.h"
#include "cropbox.h"
#include <QBrush>

ResizeBox::ResizeBox(int x, int y, int w, int h, CropBox *parent){
    this->setRect(x, y, w, h);
    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setBrush(QBrush(Qt::red));
    this->setPen(QPen(Qt::red));
    this->_parent = parent;
    this->setZValue(parent->zValue()+1);
}

void ResizeBox::updatePosition(int x, int y){
    if(x < this->_parent->x() + base_width - this->rect().width()) x = this->_parent->x() + base_width - this->rect().width();
    if(y < this->_parent->y() + base_height - this->rect().height()) y = this->_parent->y() + base_height - this->rect().height();
    if(x > gImageWidth - this->rect().width()) x = gImageWidth - this->rect().width();
    if(y > gImageHeight - this->rect().height()) y = gImageHeight - this->rect().height();
    this->setRect(x, y, this->rect().width(), this->rect().height());
}

void ResizeBox::mouseMoveEvent(QGraphicsSceneMouseEvent *e){
    if(this->_parent->isActive()){
        this->updatePosition(e->pos().x(), e->pos().y());
        int nw = this->rect().x() - this->_parent->x() + this->rect().width();
        int nh = this->rect().y() - this->_parent->y() + this->rect().height();
        this->_parent->resize(nw, nh);
        prepareGeometryChange();
    }
}
