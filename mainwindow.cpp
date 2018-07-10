#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <imagelist.h>
#include <cmath>
#include <QString>
#include <QFileDialog>
#include <QDir>
#include <QGraphicsScene>
#include "zgraphicsview.h"
#include <QGraphicsPixmapItem>
#include <QTextStream>
#include <QPixmap>
#include <QMessageBox>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QFile>
#include <QList>
#include "cropbox.h"
#include "globals.h"
#include "util.h"
#include "cfgdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    cCrops()
{
    ui->setupUi(this);
    this->init();

    this->mainScene = new QGraphicsScene();
    ui->qgvMain->setScene(this->mainScene);
    ui->qgvMain->setBackgroundBrush(QBrush(Qt::black, Qt::SolidPattern));

    gSceneHeight = this->mainScene->height();
    gSceneWidth = this->mainScene->width();
    gSceneHeight = ui->qgvMain->height();
    gSceneWidth = ui->qgvMain->width();
    gScene = this->mainScene;
    gView = ui->qgvMain;

    QCoreApplication::setOrganizationName("Zcropper");
    QCoreApplication::setApplicationName("Zcropper");

    this->wList = NULL;
    this->_image = NULL;
    this->setActiveCrop(NULL);
    this->setImageRotation(0);
    this->ui->lstBoxes->setSelectionBehavior(QAbstractItemView::SelectItems);
    this->ui->lstBoxes->setSelectionMode(QAbstractItemView::SingleSelection);
    this->ui->lstTags->setSelectionBehavior(QAbstractItemView::SelectItems);
    this->ui->lstTags->setSelectionMode(QAbstractItemView::SingleSelection);
    this->ui->lstQuality->setSelectionBehavior(QAbstractItemView::SelectItems);
    this->ui->lstQuality->setSelectionMode(QAbstractItemView::SingleSelection);

    this->setImageLoaded(false);
    this->setUnsaved(false);

    //load items into list widgets
    QList<QString>::iterator i;

    QList<QString> categoriesV;
    Util::loadListFromText(&categoriesV, gSettings->value("tags_file").toString());
    for(i = categoriesV.begin(); i != categoriesV.end(); ++i)
        ui->lstTags->addItem(*i);

    QList<QString> qualityV;
    Util::loadListFromText(&qualityV, gSettings->value("quality_file").toString());
    for(i = qualityV.begin(); i != qualityV.end(); ++i)
        ui->lstQuality->addItem(*i);
}

void MainWindow::loadAndDisplay(QString imageurl){
    QString img_folder_path = Util::fileGetFolder(imageurl);
    QString img_folder_name = Util::folderGetName(img_folder_path);
    QString img_file_name = Util::fileGetName(imageurl);
    QString img_file_ext = Util::fileGetExtension(imageurl);
    QString csvurl = Util::folderGetOuterPath(img_folder_path ) + img_folder_name + "_csv/" + img_file_name + "." + img_file_ext + ".csv";
    QFile csv(csvurl);
    csv.open(QIODevice::ReadOnly | QIODevice::Text);

    //clear GUI
    this->mainScene->clear();
    this->setActiveCrop(NULL);
    this->ui->lstBoxes->blockSignals(true);
    this->ui->lstBoxes->clear();
    this->ui->lstBoxes->blockSignals(false);
    this->ui->lstTags->setCurrentRow(-1);
    this->ui->lstQuality->setCurrentRow(-1);
    this->cCrops.clear();

    //pass csv once to check if image needs to be rotated
    int degrees = 0;
    if(csv.isOpen()){
        QTextStream qin(&csv);
        QString line;
        while(!qin.atEnd()){
            line = qin.readLine();
            if(line.at(0) == '#'){
                degrees = line.right(line.size() - 1).toInt();
                break;
            }
        }
    }

    //load and display image
    QImage *image = new QImage(imageurl);
    if(image->isNull()){
        QMessageBox::information(this, "Image Viewer", "Error Displaying image");
        this->setImageLoaded(false);
        this->setUnsaved(false);
        return;
    }
    //rotate image if necessary
    if(degrees != 0){
        QImage *temp = image;
        image = Util::getRotated(image, degrees);
        delete temp;
    }
    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap::fromImage(*image));
    gPixmap = item;
    this->mainScene->addItem(item);

    gImageWidth = item->boundingRect().width();
    gImageHeight = item->boundingRect().height();

    this->ui->lblName->setText(imageurl);
    this->ui->lblHeight->setText(QString::number(gImageHeight));
    this->ui->lblWidth->setText(QString::number(gImageWidth));

    ui->qgvMain->fitInView(item, Qt::KeepAspectRatio);

    gSceneWidth = this->mainScene->width();
    gSceneHeight = this->mainScene->height();

    gPixWidth = (int)round((float)gImageHeight/1000.0f);
    if(gPixWidth <= 0)
        gPixWidth = 1;

    //load data from CSV file (if any)
    csv.seek(0);
    QList<CropBox*> croplist;
    if(csv.isOpen()){
        QTextStream qin(&csv);
        QString line;
        while(!qin.atEnd()){
            line = qin.readLine();
            if(line.at(0) == '#')
                continue;
            int x = 0, y = 0, w = 0, h = 0;
            QString tag, quality;
            QStringList list = line.split(",");
            for(int i = 0; i < list.size(); ++i){
                switch(i){
                    case 0:
                        x = list.at(i).toInt();
                        break;
                    case 1:
                        y = list.at(i).toInt();
                        break;
                    case 2:
                        w = list.at(i).toInt() - x;
                        break;
                    case 3:
                        h = list.at(i).toInt() - y;
                        break;
                    case 4:
                        tag = list.at(i);
                        break;
                    case 5:
                        quality = list.at(i);
                }
            }

            CropBox *b = new CropBox(x, y, w, h);
            for(int i = 0; i < this->ui->lstTags->count(); i++){
                if(this->ui->lstTags->item(i)->text() == tag){
                    tag = this->ui->lstTags->item(i)->text();
                    b->setTagIndex(i);
                    break;
                }
            }
            for(int i = 0; i < this->ui->lstQuality->count(); i++){
                if(this->ui->lstQuality->item(i)->text() == quality){
                    quality = this->ui->lstQuality->item(i)->text();
                    b->setQualityIndex(i);
                    break;
                }
            }
            croplist.append(b);
        }
        csv.close();
    }

    ui->qgvMain->show();
    this->ui->qgvMain->setFocus();
    this->setImage(image);
    this->setImageLoaded(true);
    this->setUnsaved(false);
    this->setImageRotation(degrees);
    this->ui->lblRotation->setText(QString::number(degrees));

    //add csv crops
    QList<CropBox*>::iterator b;
    for(b = croplist.begin(); b != croplist.end(); ++b){
        this->mainScene->addItem(*b);
        this->mainScene->addItem((*b)->getRB());

        this->cCrops.insert(this->cCrops.end(), *b);
        this->ui->lstBoxes->addItem(this->getCropboxString(*b));
        this->ui->lstBoxes->setCurrentRow(this->ui->lstBoxes->count() - 1);
        this->ui->lstTags->setCurrentRow((*b)->getTagIndex());
        this->ui->lstQuality->setCurrentRow((*b)->getQualityIndex());
        this->updateList(*b);
    }
    this->ui->qgvMain->show();
}

void MainWindow::displayImage(QImage *img){
    if(!img){
        this->setImageLoaded(false);
        return;
    }
    //load and add image
    this->setActiveCrop(NULL);
    this->cCrops.clear();
    this->ui->lstBoxes->blockSignals(true);
    this->ui->lstBoxes->clear();
    this->ui->lstBoxes->blockSignals(false);

    this->mainScene->clear();
    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap::fromImage(*img));
    this->mainScene->addItem(item);
    ui->qgvMain->fitInView(item, Qt::KeepAspectRatio);

    gImageWidth = item->boundingRect().width();
    gImageHeight = item->boundingRect().height();

    this->ui->lblHeight->setText(QString::number(gImageHeight));
    this->ui->lblWidth->setText(QString::number(gImageWidth));
    this->setImageLoaded(true);
    this->setUnsaved(false);

    ui->qgvMain->show();
    this->ui->qgvMain->setFocus();
}

void MainWindow::setCategory(int index){
    this->ui->lstTags->setCurrentRow(index);
}

void MainWindow::setQuality(int index){
    this->ui->lstQuality->setCurrentRow(index);
}

void MainWindow::updateList(CropBox *cb){
    QString newlabel = this->getCropboxString(cb);
    this->ui->lstBoxes->currentItem()->setText(newlabel);
}

void MainWindow::writeOutCrops(){
    QString outfileurl = Util::folderGetOuterPath(this->wList->getFolder()) + Util::folderGetName(this->wList->getFolder()) + "_csv/";

    if(!QDir(outfileurl).exists())
        QDir().mkdir(outfileurl);

    outfileurl += this->wList->getName() + "." + this->wList->getExtension() + ".csv";
    gQout << "Writing " << outfileurl << "\n";
    QFile f(outfileurl);
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text)){
        QMessageBox msg;
        msg.setText("Error al crear archivo CSV para la imagen.");
        msg.exec();
        return;
    }
    QTextStream qoutf(&f);
    qoutf << "#" << this->getImageRotation() << "\n";
    for(auto const &n : this->cCrops){
        qoutf << (int)n->left()
             << "," << (int)n->top()
             << "," << (int)n->x() + n->width()
             << "," << (int)n->y() + + n->height()
             << "," << this->ui->lstTags->item(n->getTagIndex())->text()
             << "," << this->ui->lstQuality->item(n->getQualityIndex())->text()
             << "\n";
    }
    this->ui->statusBar->showMessage("Archivo '" + outfileurl + "' guardado.", 2000);
    f.close();
}

MainWindow::~MainWindow(){
    delete ui;
    delete this->wList;
    delete this->mainScene;
    delete gSettings;
}

CropBox *MainWindow::getActiveCrop() const{
    return this->cCB;
}

void MainWindow::setActiveCrop(CropBox *c){
    this->cCB = c;
}

void MainWindow::on_qgvMain_destroyed(){
    ui->qgvMain->scene()->clear();
}

void MainWindow::on_btnNext_clicked(){
    if(this->unsavedChanges()){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Cambios realizados", "¿Guardar los cambios antes de seguir?", QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        if (reply == QMessageBox::Cancel)
            return;
        else if(reply == QMessageBox::Yes){
            for(auto const &n : this->cCrops)
                if(n->getQualityIndex() == -1 || n->getTagIndex() == -1){
                    QMessageBox msgBox;
                    msgBox.setText("Debe especificarse una etiqueta y calidad para todos los cortes.");
                    msgBox.exec();
                    return;
                }
            this->writeOutCrops();
            this->setUnsaved(false);
        }
    }
    if(this->wList == NULL || this->wList->isEmpty())
        return;
    this->wList->Next();
    if(this->getActiveCrop() != NULL){
        delete this->getActiveCrop();
        this->setActiveCrop(NULL);
    }
    loadAndDisplay(this->wList->getPath());
}

void MainWindow::on_btnPrev_clicked(){
    if(this->unsavedChanges()){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Cambios realizados", "¿Guardar los cambios antes de seguir?", QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        if (reply == QMessageBox::Cancel)
            return;
        else if(reply == QMessageBox::Yes){
            for(auto const &n : this->cCrops)
                if(n->getQualityIndex() == -1 || n->getTagIndex() == -1){
                    QMessageBox msgBox;
                    msgBox.setText("Debe especificarse una etiqueta y calidad para todos los cortes.");
                    msgBox.exec();
                    return;
                }
            this->writeOutCrops();
            this->setUnsaved(false);
        }
    }
    if(this->wList == NULL || this->wList->isEmpty())
        return;
    this->wList->Previous();
    if(this->getActiveCrop() != NULL){
        delete this->getActiveCrop();
        this->setActiveCrop(NULL);
    }
    loadAndDisplay(this->wList->getPath());
}

void MainWindow::on_amiSelectFolder_triggered(){
    QString folderName = QFileDialog::getExistingDirectory(this, "Directorio", "/home", QFileDialog::ShowDirsOnly);
    if(folderName.isEmpty())
        return;
    this->wList = new imagelist(folderName);
    if(this->wList->isEmpty())
        return;
    loadAndDisplay(this->wList->getPath());
}

void MainWindow::on_actionExit_triggered(){
    if(this->unsavedChanges()){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Cambios realizados", "¿Guardar los cambios antes de seguir?", QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        if (reply == QMessageBox::Cancel)
            return;
        else if(reply == QMessageBox::Yes){
            for(auto const &n : this->cCrops)
                if(n->getQualityIndex() == -1 || n->getTagIndex() == -1){
                    QMessageBox msgBox;
                    msgBox.setText("Debe especificarse una etiqueta y calidad para todos los cortes.");
                    msgBox.exec();
                    return;
                }
            this->writeOutCrops();
            this->setUnsaved(false);
        }
    }
    exit(0);
}

void MainWindow::spawnCropbox(qreal x, qreal y){
    if(!this->isImageLoaded())
        return;
    if(this->getActiveCrop() != NULL)
        this->getActiveCrop()->deactivate();

    //correct invalid coordinates
    if(x < 0)
        x = 0;
    if(y < 0)
        y = 0;
    if(x > gImageWidth - base_width)
        x = 1 + gImageWidth - base_width;
    if(y > gImageHeight - base_height)
        y = 1 + gImageHeight - base_height;

    //spawn cropbox and add to boxes vector
    CropBox *newbox = new CropBox(x, y, base_width, base_height);
    this->setActiveCrop(newbox);
    this->mainScene->addItem(newbox);
    this->mainScene->addItem(newbox->getRB());
    this->getActiveCrop()->activate();
    this->cCrops.insert(this->cCrops.end(), newbox);
    this->ui->lstBoxes->addItem(this->getCropboxString(newbox));
    this->ui->lstBoxes->setCurrentRow(this->ui->lstBoxes->count() - 1);
    this->ui->lstTags->setCurrentRow(-1);
    this->ui->lstQuality->setCurrentRow(-1);
    this->updateList(this->getActiveCrop());
    this->setUnsaved(true);
}

void MainWindow::closeEvent(QCloseEvent *e){
    if(this->unsavedChanges()){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Cambios realizados", "¿Guardar los cambios antes de seguir?", QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        if (reply == QMessageBox::Cancel)
            e->ignore();
        else if(reply == QMessageBox::Yes){
            for(auto const &n : this->cCrops)
                if(n->getQualityIndex() == -1 || n->getTagIndex() == -1){
                    QMessageBox msgBox;
                    msgBox.setText("Debe especificarse una etiqueta y calidad para todos los cortes.");
                    msgBox.exec();
                    e->ignore();
                }
            this->writeOutCrops();
            this->setUnsaved(false);
            e->accept();
        }
    }
}

bool MainWindow::unsavedChanges() const{
    return this->unsaved_changes;
}

void MainWindow::setUnsaved(bool value){
    this->unsaved_changes = value;
}

void MainWindow::on_btnNewCrop_clicked(){
    this->spawnCropbox(0, 0);
}

void MainWindow::on_lstBoxes_itemSelectionChanged(){
    if(this->cCrops.size() == 0 || this->ui->lstBoxes->count() == 0)
        return;
    int index = this->ui->lstBoxes->currentIndex().row();
    for(const auto &n : this->cCrops)
        n->deactivate();
    this->setActiveCrop(this->cCrops.at(index));
    this->getActiveCrop()->activate();
}

void MainWindow::init(){
    //load settings
#if _WIN32
    gSettings = new QSettings(QSettings::IniFormat, QSettings::SystemScope, "Zcropper", "Zcropper");
#else
    gSettings = new QSettings();
#endif
    //check if tagging files exist
    QFile tagsfile(gSettings->value("tags_file").toString());
    QFile qltyfile(gSettings->value("quality_file").toString());

    //check cropbox values
    base_width = gSettings->value("base_width").toInt();
    base_height = gSettings->value("base_height").toInt();
    width_multiplier = gSettings->value("width_multiplier").toInt();
    height_multiplier = gSettings->value("height_multiplier").toInt();
    w_to_h_ratio = (float)base_width/(float)base_height;
    h_to_w_ratio = (float)base_height/(float)base_width;

    if(base_width == 0
            || base_height == 0
            || width_multiplier == 0
            || height_multiplier == 0
            || !qltyfile.exists()
            || !tagsfile.exists()){
        //open config
        CfgDialog *cfg = new CfgDialog();
        cfg->exec();
    }
}

bool MainWindow::isImageLoaded() const{
    return this->image_is_loaded;
}

void MainWindow::setImageLoaded(bool value){
    this->image_is_loaded = value;
}

QImage *MainWindow::getImage() const{
    return this->_image;
}

void MainWindow::setImage(QImage *image){
    if(this->_image)
        delete this->_image;
    this->_image = image;
}

void MainWindow::on_btnIncrease_clicked(){
    if(this->getActiveCrop() == NULL)
        return;
    this->getActiveCrop()->increaseSize();
    this->updateList(this->getActiveCrop());
}

void MainWindow::on_btnDecrease_clicked(){
    if(this->getActiveCrop() == NULL)
        return;
    this->getActiveCrop()->decreaseSize();
    this->updateList(this->getActiveCrop());
}

void MainWindow::on_lstTags_itemSelectionChanged(){
    if(this->cCrops.size() == 0 || this->ui->lstBoxes->count() == 0)
        return;
    this->getActiveCrop()->setTagIndex(ui->lstTags->currentIndex().row());
    this->updateList(this->getActiveCrop());
}

void MainWindow::on_btnDeleteCrop_clicked(){
    if(this->ui->lstBoxes->currentRow() < 0)
        return;
    this->cCrops.erase(this->cCrops.begin() + this->ui->lstBoxes->currentRow());
    this->ui->lstBoxes->blockSignals(true);
    this->ui->lstBoxes->takeItem(this->ui->lstBoxes->currentRow());
    delete this->getActiveCrop();
    this->setActiveCrop(NULL);
    this->ui->lstBoxes->blockSignals(false);
    if(this->ui->lstBoxes->count() > 0){
        this->ui->lstBoxes->setCurrentRow(0);
        int index = this->ui->lstBoxes->currentIndex().row();
        for(const auto &n : this->cCrops)
            n->deactivate();
        this->setActiveCrop(this->cCrops.at(index));
        this->getActiveCrop()->activate();
    }
    this->setUnsaved(true);
}

void MainWindow::on_btnAccept_clicked(){
    for(auto const &n : this->cCrops)
        if(n->getQualityIndex() == -1 || n->getTagIndex() == -1){
            QMessageBox msgBox;
            msgBox.setText("Debe especificarse una etiqueta y calidad para todos los cortes.");
            msgBox.exec();
            return;
        }
    if(this->cCrops.size() > 0)
        this->writeOutCrops();
    this->setUnsaved(false);
    this->on_btnNext_clicked();
}

void MainWindow::on_lstQuality_itemSelectionChanged(){
    if(this->cCrops.size() == 0 || this->ui->lstBoxes->count() == 0)
        return;
    this->getActiveCrop()->setQualityIndex(ui->lstQuality->currentIndex().row());
    this->updateList(this->getActiveCrop());
}

void MainWindow::on_actionOpt_triggered(){
    //open config
    CfgDialog *cfg = new CfgDialog();
    cfg->exec();
}

void MainWindow::on_btnRotateRight_clicked(){
    this->setImage(Util::getRotated(this->getImage(), 90));
    this->displayImage(this->getImage());
    this->setImageRotation(this->getImageRotation() + 90);
    this->ui->lblRotation->setText(QString::number(this->getImageRotation()));
    this->setUnsaved(true);
}

void MainWindow::on_btnRotateLeft_clicked(){
    this->setImage(Util::getRotated(this->getImage(), -90));
    this->displayImage(this->getImage());
    this->setImageRotation(this->getImageRotation() - 90);
    this->ui->lblRotation->setText(QString::number(this->getImageRotation()));
    this->setUnsaved(true);
}

void MainWindow::resizeEvent(QResizeEvent *e){
    if(gPixmap != NULL){
        ui->qgvMain->fitInView(gPixmap, Qt::KeepAspectRatio);
        ui->qgvMain->show();
    }
}

QString MainWindow::getCropboxString(CropBox *b){
    QString tag = b->getTagIndex() >= 0? this->ui->lstTags->item(b->getTagIndex())->text() : "*";
    QString quality = b->getQualityIndex() >= 0? this->ui->lstQuality->item(b->getQualityIndex())->text() : "*";
    return "X: " + QString::number((int)(b->x()))
            + " | Y: " + QString::number((int)b->y())
            + " | W: " + QString::number((int)b->width())
            + " | H: " + QString::number((int)b->height())
            + " | " + tag
            + " | " + quality;
}

qreal MainWindow::getImageRotation() const{
    return this->image_rotation;
}

void MainWindow::setImageRotation(const int &value){
    this->image_rotation = value % 360;
}
