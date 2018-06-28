#pragma once
#include <QGraphicsView>
#include <QPoint>
#include <QEvent>

class ZGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ZGraphicsView(QWidget *parent = nullptr);
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
signals:

public slots:
};
