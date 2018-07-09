#include "cropbox.h"
#include "globals.h"
#include <cmath>

CropBox::CropBox(int x, int y, int width, int height) :
    _pen(Qt::red, gPixWidth, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin),
    _rb(x + width - 20, y + height - 20, 20, 20, this)
{
    this->setX(x);;
    this->setY(y);;
    this->_width = width <= gImageWidth? width : gImageWidth;
    this->_height = height <= gImageHeight? height : gImageHeight;
    this->_enabled = true;
    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->category_index = -1;
    this->quality_index = -1;

    int w_mult = (ceil(this->_width/base_width))-1;
    int h_mult = (ceil(this->_height/base_height))-1;

    this->self_multiplier = w_mult > h_mult? w_mult : h_mult;
    prepareGeometryChange();
}

void CropBox::mouseMoveEvent(QGraphicsSceneMouseEvent *e){
    if((e->buttons() & (Qt::LeftButton) && this->_enabled)){
        QGraphicsItem::mouseMoveEvent(e);
        if(this->left() < 0)
            this->setX(0);
        if(this->top() < 0)
            this->setY(0);
        if(this->right() > gImageWidth)
            this->setX(gImageWidth - this->_width);
        if(this->bottom() > gImageHeight)
            this->setY(gImageHeight - this->_height);

        //update resizebox position
        this->_rb.updatePosition(this->x() + this->_width - this->_rb.rect().width(),
                                 this->y() + this->_height - this->_rb.rect().height());

        gWindow->updateList(this);
        gWindow->setUnsaved(true);
    }
}

void CropBox::mousePressEvent(QGraphicsSceneMouseEvent *e){
    QGraphicsItem::mousePressEvent(e);
}

void CropBox::resize(int new_w, int new_h){
    if(this->_width == new_w && this->_height == new_h)
        return;

    this->_width = new_w;
    this->_height = new_h;

    if(this->x() + new_w >= gImageWidth)
        this->setX(gImageWidth - new_w);
    if(this->y() + new_h >= gImageHeight)
        this->setY(gImageHeight - new_h);
    if(new_w >= gImageWidth)
        this->setX(0);
    if(new_h >= gImageHeight)
        this->setY(0);

    prepareGeometryChange();
    gWindow->setUnsaved(true);
    gWindow->updateList(this);
}

bool CropBox::isActive(){
    return this->_enabled;
}

void CropBox::activate(){
    this->_pen = QPen(Qt::red);
    this->_rb.setBrush(QBrush(Qt::red));
    this->_rb.setPen(QPen(Qt::red));
    this->_enabled = true;
    this->setZValue(1);
    prepareGeometryChange();
    gWindow->setCategory(this->category_index);
    gWindow->setQuality(this->quality_index);
}

void CropBox::deactivate(){
    this->_pen = QPen(Qt::white);
    this->_rb.setBrush(QBrush(Qt::white));
    this->_rb.setPen(QPen(Qt::white));
    this->_enabled = false;
    this->setZValue(0);
    prepareGeometryChange();
}

ResizeBox *CropBox::getRB(){
    return &this->_rb;
}

int CropBox::right(){
    return this->x() + this->_width - 1;
}

int CropBox::left(){
    return this->x();
}

int CropBox::top(){
    return this->y();
}

int CropBox::bottom(){
    return this->y() + this->_height - 1;
}

int CropBox::width(){
    return this->_width;
}

int CropBox::height(){
    return this->_height;
}

void CropBox::setQualityIndex(int index){
    if(this->quality_index != index){
        this->quality_index = index;
        gWindow->setUnsaved(true);
    }
}

void CropBox::setTagIndex(int index){
    if(this->category_index != index){
        this->category_index = index;
        gWindow->setUnsaved(true);
    }
}

int CropBox::getQualityIndex(){
    return this->quality_index;
}

int CropBox::getTagIndex(){
    return this->category_index;
}

void CropBox::increaseSize(){
    if(this->width() >= gImageWidth && this->height() >= gImageHeight)
        return;
    int s = this->self_multiplier+1;

    int new_w = base_width;
    int new_h = base_height;
    for(int i = 0; i < s; i++)
        new_w *= width_multiplier;
    for(int i = 0; i < s; i++)
        new_h *= height_multiplier;
    if(new_w > gImageWidth)
        new_w = gImageWidth;
    if(new_h > gImageHeight)
        new_h = gImageHeight;

    if(new_w > gImageWidth)
        new_w = gImageWidth;
    if(new_h > gImageHeight)
        new_h = gImageHeight;

    this->resize(new_w, new_h);
    this->self_multiplier = s;

    //update resizebox position
    this->_rb.updatePosition(this->x() + this->_width - this->_rb.rect().width(),
                             this->y() + this->_height - this->_rb.rect().height());
}

void CropBox::decreaseSize(){
    int s = this->self_multiplier - 1;
    if(s < 0){
        this->resize(base_width, base_height);
        return;
    }

    int new_w = base_width;
    for(int i = 0; i < s; i++)
        new_w *= width_multiplier;
    int new_h = base_height;
    for(int i = 0; i < s; i++)
        new_h *= height_multiplier;

    if(new_w < base_width)
        new_w = base_width;
    if(new_h < base_height)
        new_h = base_height;
    if(new_w > gImageWidth)
        new_w = gImageWidth;
    if(new_h > gImageHeight)
        new_h = gImageHeight;

    this->resize(new_w, new_h);
    this->self_multiplier = s;

    //update resizebox position
    this->_rb.updatePosition(this->x() + this->_width - this->_rb.rect().width(),
                             this->y() + this->_height - this->_rb.rect().height());
}

CropBox::~CropBox(){

}

void CropBox::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    QRectF rect = this->boundingRect();
    this->_pen.setWidth(gPixWidth);
    painter->setPen(this->_pen);
    painter->drawRect(rect);
}

QRectF CropBox::boundingRect() const{
    return QRectF(0, 0, this->_width, this->_height);
}
