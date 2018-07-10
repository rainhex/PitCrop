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
        qreal mouse_x = e->pos().x(), mouse_y = e->pos().y();

        int nw = this->rect().x() - this->_parent->x() + this->rect().width();
        int nh = this->rect().y() - this->_parent->y() + this->rect().height();

        int mx_dist = mouse_x - this->_parent->x();
        int my_dist = mouse_y - this->_parent->y();
        nw = (int)(mx_dist+my_dist)/2;
        nh = nw;

        if(nw > gImageWidth)
            nw = gImageWidth;
        if(nh > gImageHeight)
            nh = gImageHeight;
        if(w_to_h_ratio > 1)
            nw = nh*w_to_h_ratio;
        if(h_to_w_ratio > 1)
            nh = nw*h_to_w_ratio;

        this->_parent->resize(nw, nh);
        this->updatePosition(this->_parent->x() + this->_parent->width() - this->rect().width(),
                             this->_parent->y() + this->_parent->height() - this->rect().height());
        //this->_parent->resize(nw, nh);
        prepareGeometryChange();
    }
}
