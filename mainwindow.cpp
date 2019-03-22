#include <cmath>
#include <memory>

#include <QString>
#include <QFileDialog>
#include <QDir>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QTextStream>
#include <QPixmap>
#include <QMessageBox>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QFile>
#include <QFileInfo>
#include <QList>
#include <QImageReader>

#include "imagelist.h"
#include "zgraphicsview.h"
#include "cropbox.h"
#include "globals.h"
#include "util.h"
#include "cfgdialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settings.h"

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

    QCoreApplication::setOrganizationName(gOrganization);
    QCoreApplication::setApplicationName(gAppName);

    this->wList = nullptr;
    this->_image = nullptr;
    this->setActiveCrop(nullptr);
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
    Util::loadListFromText(&categoriesV, Settings::Get("tags_file").toString());
    for(i = categoriesV.begin(); i != categoriesV.end(); ++i)
        ui->lstTags->addItem(*i);

    QList<QString> qualityV;
    Util::loadListFromText(&qualityV, Settings::Get("quality_file").toString());
    for(i = qualityV.begin(); i != qualityV.end(); ++i)
        ui->lstQuality->addItem(*i);
}

void MainWindow::loadAndDisplay(QString imageurl){
    QFile csv(Util::fileGetCsvUrl(imageurl));
    csv.open(QIODevice::ReadOnly | QIODevice::Text);

    this->ui->spnGo->blockSignals(true);
    this->ui->spnGo->setValue(this->wList->getPosition() + 1);
    this->ui->spnGo->blockSignals(false);

    //clear GUI
    this->mainScene->clear();
    this->setActiveCrop(nullptr);
    this->ui->lstBoxes->blockSignals(true);
    this->ui->lstBoxes->clear();
    this->ui->lstBoxes->blockSignals(false);
    this->ui->lstTags->setCurrentRow(-1);
    this->ui->lstQuality->setCurrentRow(-1);
    this->cCrops.clear();

    QString newTitle = "Datos de imagen (" + QString::number(this->wList->getPosition() + 1) + " de " + QString::number(this->wList->getSize()) + ")";
    this->ui->groupBox->setTitle(newTitle);

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
    QImageReader reader(imageurl);
    reader.setAutoTransform(true);
    QImage *image = new QImage();
    reader.read(image);

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

    gPixWidth = static_cast<int>(gImageHeight/1000);
    if(gPixWidth <= 0)
        gPixWidth = 1;

    this->setImage(image);
    this->setImageLoaded(true);
    this->setImageRotation(degrees);
    this->ui->lblRotation->setText(QString::number(degrees));

    //load data from CSV file (if any)
    QList<CropBox*> croplist;
    if(csv.isOpen()){
        csv.seek(0);
        QTextStream qin(&csv);
        QString line;
        while(!qin.atEnd()){
            line = qin.readLine();
            if(line == "" || line.at(0) == '#' || line.at(0) == '$' || line.at(0) == '%')
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
        this->updateListItem(*b);
    }
    this->setUnsaved(false);
    this->ui->qgvMain->setFocus();
    this->ui->qgvMain->show();
}

void MainWindow::displayImage(QImage *img){
    if(!img){
        this->setImageLoaded(false);
        return;
    }
    //load and add image
    this->setActiveCrop(nullptr);
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

    ui->qgvMain->show();
    this->ui->qgvMain->setFocus();
}

void MainWindow::setCategory(int index){
    this->ui->lstTags->setCurrentRow(index);
}

void MainWindow::setQuality(int index){
    this->ui->lstQuality->blockSignals(true);
    this->ui->lstQuality->setCurrentRow(index);
    this->ui->lstQuality->blockSignals(false);
}

void MainWindow::updateListItem(CropBox *cb){
    this->ui->lstBoxes->blockSignals(true);
    QString newlabel = this->getCropboxString(cb);
    this->ui->lstBoxes->currentItem()->setText(newlabel);
    this->ui->lstBoxes->blockSignals(false);
}

void MainWindow::writeOutCrops(){
    QString outfileurl = Util::folderGetOuterPath(this->wList->getFolder()) + Util::folderGetName(this->wList->getFolder()) + "_csv/";

    if(!QDir(outfileurl).exists())
        QDir().mkdir(outfileurl);

    if(this->cCrops.size() <= 0){
        if(QFile::exists(outfileurl)){
            gQout << "Deleting " << outfileurl << "\n";
            QFile file (outfileurl);
                if(file.remove())
                    return;
                else{
                    QMessageBox msg;
                    msg.setText("Error al eliminar archivo CSV de imagen.");
                    msg.exec();
                    return;
                }
        }
    }

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
    qoutf << "%" << gImageHeight << ":" << gImageWidth << "\n";

    for(auto const &n : this->cCrops){
        qoutf << static_cast<int>(n->left())
             << "," << static_cast<int>(n->top())
             << "," << static_cast<int>(n->x() + n->width())
             << "," << static_cast<int>(n->y() + + n->height())
             << "," << this->ui->lstTags->item(n->getTagIndex())->text()
             << "," << this->ui->lstQuality->item(n->getQualityIndex())->text()
             << "\n";
    }
    f.close();
    this->ui->statusBar->showMessage("Archivo '" + outfileurl + "' guardado.", 2000);
}

MainWindow::~MainWindow(){
    delete ui;
    delete this->wList;
    delete this->mainScene;
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
    if(this->wList == nullptr || this->wList->isEmpty())
        return;
    if(this->currentFilter() == imagelist::FilterMode::All)
        this->wList->Cycle(1);
    else
        this->wList->Seek(1, this->currentFilter());
    if(this->getActiveCrop() != nullptr){
        delete this->getActiveCrop();
        this->setActiveCrop(nullptr);
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
    if(this->wList == nullptr || this->wList->isEmpty())
        return;
    if(this->currentFilter() == imagelist::FilterMode::All)
        this->wList->Cycle(-1);
    else
        this->wList->Seek(-1, this->currentFilter());
    if(this->getActiveCrop() != nullptr){
        delete this->getActiveCrop();
        this->setActiveCrop(nullptr);
    }
    loadAndDisplay(this->wList->getPath());
}

void MainWindow::on_amiSelectFolder_triggered(){
    if(gLastFolder == "")
            gLastFolder = "/home";
    QString folderName = QFileDialog::getExistingDirectory(this, "Directorio", gLastFolder, QFileDialog::ShowDirsOnly);
    if(folderName.isEmpty())
        return;
    gLastFolder = folderName;
    Settings::Set("last_folder", gLastFolder);

    this->wList = new imagelist(folderName);
    this->ui->spnGo->setMinimum(1);
    this->ui->spnGo->setMaximum(this->wList->getSize());
    this->ui->spnGo->setWrapping(true);
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
    if(this->getActiveCrop() != nullptr)
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
    this->ui->lstBoxes->blockSignals(true);
    this->ui->lstQuality->blockSignals(true);
    this->ui->lstTags->blockSignals(true);

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
    this->updateListItem(this->getActiveCrop());
    this->setUnsaved(true);

    this->ui->lstBoxes->blockSignals(false);
    this->ui->lstQuality->blockSignals(false);
    this->ui->lstTags->blockSignals(false);
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

void MainWindow::init(){
    //check cropbox values
    base_width = Settings::Get("base_width").toInt();
    base_height = Settings::Get("base_height").toInt();
    width_multiplier = Settings::Get("width_multiplier").toInt();
    height_multiplier = Settings::Get("height_multiplier").toInt();
    w_to_h_ratio = base_width/base_height;
    h_to_w_ratio = base_height/base_width;
    gLastFolder = Settings::Get("last_folder").toString();

    if(base_width <= 0
            || base_height <= 0
            || width_multiplier <= 0
            || height_multiplier <= 0
            || !QFileInfo::exists(Settings::Get("tags_file").toString())
            || !QFileInfo::exists(Settings::Get("quality_file").toString())){
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
    if(this->getActiveCrop() == nullptr)
        return;
    this->getActiveCrop()->increaseSize();
    this->updateListItem(this->getActiveCrop());
}

void MainWindow::on_btnDecrease_clicked(){
    if(this->getActiveCrop() == nullptr)
        return;
    this->getActiveCrop()->decreaseSize();
    this->updateListItem(this->getActiveCrop());
}

void MainWindow::on_btnDeleteCrop_clicked(){
    if(this->ui->lstBoxes->currentRow() < 0)
        return;
    this->cCrops.erase(this->cCrops.begin() + this->ui->lstBoxes->currentRow());
    this->ui->lstBoxes->blockSignals(true);
    this->ui->lstBoxes->takeItem(this->ui->lstBoxes->currentRow());
    delete this->getActiveCrop();
    this->setActiveCrop(nullptr);
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
    if(this->cCrops.size() > 0){ // write file with crops
        this->writeOutCrops();
    }
    else{ // else, check if CSV exists - remove if true
        QString csvpath = Util::fileGetCsvUrl(this->wList->getPath());
        if(QFileInfo::exists(csvpath)){
            QFile file(csvpath);
            file.remove();
        }
    }
    this->setUnsaved(false);
    this->on_btnNext_clicked();
}

void MainWindow::on_actionOpt_triggered(){
    CfgDialog *cfg = new CfgDialog();
    cfg->exec();
}

void MainWindow::on_btnRotateRight_clicked(){
    if(!this->isImageLoaded())
        return;
    this->setImage(Util::getRotated(this->getImage(), 90));
    this->displayImage(this->getImage());
    this->setImageRotation(this->getImageRotation() + 90);
    this->ui->lblRotation->setText(QString::number(this->getImageRotation()));
    this->setUnsaved(true);
}

void MainWindow::on_btnRotateLeft_clicked(){
    if(!this->isImageLoaded())
        return;
    this->setImage(Util::getRotated(this->getImage(), -90));
    this->displayImage(this->getImage());
    this->setImageRotation(this->getImageRotation() - 90);
    this->ui->lblRotation->setText(QString::number(this->getImageRotation()));
    this->setUnsaved(true);
}

void MainWindow::resizeEvent(QResizeEvent *e){
    if(gPixmap != nullptr){
        ui->qgvMain->fitInView(gPixmap, Qt::KeepAspectRatio);
        ui->qgvMain->show();
    }
}

QString MainWindow::getCropboxString(CropBox *b){
    QString tag = b->getTagIndex() >= 0? this->ui->lstTags->item(b->getTagIndex())->text() : "*";
    QString quality = b->getQualityIndex() >= 0? this->ui->lstQuality->item(b->getQualityIndex())->text() : "*";
    return tag + " | " + quality
            + " | W: " + QString::number(static_cast<int>(b->width()))
            + " | H: " + QString::number(static_cast<int>(b->height()))
            + " | X: " + QString::number(static_cast<int>(b->x()))
            + " | Y: " + QString::number(static_cast<int>(b->y()));
}

int MainWindow::getImageRotation() const{
    return this->image_rotation;
}

void MainWindow::setImageRotation(const int &value){
    this->image_rotation = value % 360;
}

int MainWindow::currentFilter()
{
    if(this->ui->rdoAll->isChecked())
        return imagelist::FilterMode::All;
    if(this->ui->rdoCropped->isChecked())
        return imagelist::FilterMode::Cropped;
    if(this->ui->rdoUncropped->isChecked())
        return imagelist::FilterMode::Uncropped;
    return -1;
}

void MainWindow::on_lstTags_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if(this->cCrops.size() == 0 || this->ui->lstBoxes->count() == 0)
        return;
    this->getActiveCrop()->setTagIndex(ui->lstTags->currentIndex().row());
    this->updateListItem(this->getActiveCrop());
}

void MainWindow::on_lstBoxes_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if(this->cCrops.size() == 0 || this->ui->lstBoxes->count() == 0)
        return;
    int index = this->ui->lstBoxes->currentIndex().row();
    for(const auto &n : this->cCrops)
        n->deactivate();
    this->setActiveCrop(this->cCrops.at(index));
    this->getActiveCrop()->activate();
}

void MainWindow::on_lstQuality_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if(this->cCrops.size() == 0 || this->ui->lstBoxes->count() == 0)
        return;
    this->getActiveCrop()->setQualityIndex(ui->lstQuality->currentIndex().row());
    this->updateListItem(this->getActiveCrop());
}

void MainWindow::on_spnGo_editingFinished()
{
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
    if(this->wList == nullptr || this->wList->isEmpty())
        return;

    int pos = this->ui->spnGo->value();
    if(pos > this->wList->getSize() || pos <= 0)
        pos = 1;
    this->wList->setPosition(pos - 1);
    if(this->getActiveCrop() != nullptr){
        delete this->getActiveCrop();
        this->setActiveCrop(nullptr);
    }
    loadAndDisplay(this->wList->getPath());
}
