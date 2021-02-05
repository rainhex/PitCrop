#pragma once
#include <QImage>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QTextStream>
#include <QSettings>
#include <QString>
#include "mainwindow.h"
#include "zgraphicsview.h"

extern qreal gSceneWidth;
extern qreal gSceneHeight;
extern qreal gImageWidth;
extern qreal gImageHeight;
extern bool exit_app;
extern QGraphicsScene *gScene;
extern ZGraphicsView *gView;
extern MainWindow *gWindow;
extern QTextStream gQout;
extern qreal base_width;
extern qreal base_height;
extern qreal width_multiplier;
extern qreal height_multiplier;
extern int gPixWidth;
extern qreal w_to_h_ratio;
extern qreal h_to_w_ratio;
extern QImage *gLoadedImage;
extern QGraphicsPixmapItem *gPixmap;
extern QString gLastFolder;

extern const QString gOrganization;
extern const QString gAppName;
