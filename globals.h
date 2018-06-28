#pragma once
#include <QGraphicsScene>
#include <QTextStream>
#include <QSettings>
#include "mainwindow.h"
#include "zgraphicsview.h"

extern int gSceneWidth;
extern int gSceneHeight;
extern int gImageWidth;
extern int gImageHeight;
extern bool exit_app;
extern QGraphicsScene *gScene;
extern ZGraphicsView *gView;
extern MainWindow *gWindow;
extern QSettings *gSettings;
extern QTextStream gQout;
extern int base_width;
extern int base_height;
extern int width_multiplier;
extern int height_multiplier;
extern int gPixWidth;
//extern QApplication *qApp;
