#pragma once
#include <QListWidget>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QList>
#include <QImage>
#include <imagelist.h>
#include <cropbox.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    ~MainWindow();
    explicit MainWindow(QWidget *parent = 0);
    void loadAndDisplay(QString ref);
    void displayImage(QImage *img);
    void setCategory(int index);
    void setQuality(int index);
    void updateList(CropBox *cb);
    void writeOutCrops();
    void loadCrops(QString imageurl);
    void spawnCropbox(qreal x, qreal y);
    void closeEvent(QCloseEvent *e);
    bool unsavedChanges() const;
    void setUnsaved(bool value);
    void setActiveCrop(CropBox *c);
    CropBox *getActiveCrop() const;
    QImage *getImage() const;
    void setImage(QImage *image);
    bool isImageLoaded() const;
    void setImageLoaded(bool value);

public slots:
    void on_btnIncrease_clicked(); //Q

    void on_btnDecrease_clicked(); //W

    void on_btnNext_clicked(); //right

    void on_btnPrev_clicked(); //left

    void on_btnNewCrop_clicked(); //C

    void on_btnDeleteCrop_clicked(); //del

    void on_btnAccept_clicked(); //enter

private slots:
    void on_qgvMain_destroyed();

    void on_amiSelectFolder_triggered();

    void on_actionExit_triggered();

    void on_lstBoxes_itemSelectionChanged();

    void on_lstTags_itemSelectionChanged();

    void on_lstQuality_itemSelectionChanged();

    void on_actionOpt_triggered();

    void on_btnRotateRight_clicked();

    void on_btnRotateLeft_clicked();

private:
    Ui::MainWindow *ui;
    imagelist *wList;
    QList<CropBox*> cCrops;
    QGraphicsScene *mainScene;
    int lstIndex;
    void init();
    bool image_is_loaded;
    bool unsaved_changes;
    CropBox *cCB;
    QImage *_image;
};
