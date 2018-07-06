#include "zgraphicsview.h"
#include "globals.h"
#include <QApplication>

ZGraphicsView::ZGraphicsView(QWidget *parent) : QGraphicsView(parent){
    this->setFocusPolicy(Qt::ClickFocus);
}

void ZGraphicsView::keyPressEvent(QKeyEvent *event){
    switch (event->key()) {
        case Qt::Key_W:
            gWindow->on_btnIncrease_clicked();
            break;
        case Qt::Key_Q:
            gWindow->on_btnDecrease_clicked();
            break;
        case Qt::Key_Delete:
            gWindow->on_btnDeleteCrop_clicked();
            break;
        case Qt::Key_Right:
            gWindow->on_btnNext_clicked();
            break;
        case Qt::Key_Left:
            gWindow->on_btnPrev_clicked();
            break;
        case Qt::Key_C:{
            QPoint origin = gView->mapFromGlobal(QCursor::pos());
            QPointF relativeOrigin = gView->mapToScene(origin);
            qreal x = relativeOrigin.x() - (base_width/2);
            qreal y = relativeOrigin.y() - (base_height/2);
            gWindow->spawnCropbox(x, y);
        }
            break;
        case Qt::Key_Enter:
        case Qt::Key_Return:
            gWindow->on_btnAccept_clicked();
            break;
        default:
            break;
    }
}

void ZGraphicsView::mousePressEvent(QMouseEvent *event){
    if(event->buttons() & Qt::RightButton){
        QPoint origin = gView->mapFromGlobal(QCursor::pos());
        QPointF relativeOrigin = gView->mapToScene(origin);

        int x = relativeOrigin.x() - (base_width/2);
        int y = relativeOrigin.y() - (base_height/2);
        gWindow->spawnCropbox(x, y);
    }
    QGraphicsView::mousePressEvent(event);
}
