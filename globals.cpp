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
QSettings *gSettings;
QTextStream gQout(stdout);

int base_width = 0;
int base_height = 0;
int width_multiplier = 0;
int height_multiplier = 0;
//QApplication *qApp = NULL;
