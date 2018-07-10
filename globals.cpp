#include "globals.h"

int gSceneWidth = 0;
int gSceneHeight = 0;
int gImageWidth = 0;
int gImageHeight = 0;
int gPixWidth = 0;
bool exit_app = false;
QGraphicsScene *gScene = NULL;
ZGraphicsView *gView = NULL;
MainWindow *gWindow = NULL;
QSettings *gSettings = NULL;
QTextStream gQout(stdout);

int base_width = 0;
int base_height = 0;
int width_multiplier = 0;
int height_multiplier = 0;
qreal w_to_h_ratio = 0;
qreal h_to_w_ratio = 0;
QImage *gLoadedImage = NULL;
QGraphicsPixmapItem *gPixmap = NULL;
