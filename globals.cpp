#include "globals.h"
#include <memory>

qreal gSceneWidth = 0;
qreal gSceneHeight = 0;
qreal gImageWidth = 0;
qreal gImageHeight = 0;
int gPixWidth = 0;
bool exit_app = false;
QGraphicsScene *gScene = nullptr;
ZGraphicsView *gView = nullptr;
MainWindow *gWindow = nullptr;
QTextStream gQout(stdout);

QString gLastFolder;

qreal base_width = 0;
qreal base_height = 0;
qreal width_multiplier = 0;
qreal height_multiplier = 0;
qreal w_to_h_ratio = 0;
qreal h_to_w_ratio = 0;
QImage *gLoadedImage = nullptr;
QGraphicsPixmapItem *gPixmap = nullptr;

const QString gOrganization = "Universidad Autonoma de Sinaloa";
const QString gAppName = "PitCrop";
