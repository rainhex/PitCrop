#include "cfgdialog.h"
#include "globals.h"
#include "ui_cfgdialog.h"
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>

CfgDialog::CfgDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CfgDialog)
{
    ui->setupUi(this);

    this->ui->txtTagsFile->setText(gSettings->value("tags_file").toString());
    this->ui->txtQualityFile->setText(gSettings->value("quality_file").toString());

    this->ui->txtBaseWidth->setText(gSettings->value("base_width").toString());
    this->ui->txtBaseHeight->setText(gSettings->value("base_height").toString());
    this->ui->txtWidthMult->setText(gSettings->value("width_multiplier").toString());
    this->ui->txtHeightMult->setText(gSettings->value("height_multiplier").toString());
}

CfgDialog::~CfgDialog(){
    delete ui;
}

void CfgDialog::on_btnSave_clicked(){
    //check if tagging files exist
    QFile tfile(this->ui->txtTagsFile->text());
    if(!tfile.exists()){
        QMessageBox mgs;
        mgs.setText("El archivo de etiquetas no existe.");
        mgs.exec();
        return;
    }
    QFile qfile(this->ui->txtQualityFile->text());
    if(!qfile.exists()){
        QMessageBox mgs;
        mgs.setText("El archivo de calidades no existe.");
        mgs.exec();
        return;
    }

    //check cropbox values
    if(this->ui->txtBaseHeight->text().toInt() <= 0
            || this->ui->txtBaseWidth->text().toInt() <= 0
            || this->ui->txtBaseHeight->text().toInt() <= 0
            || this->ui->txtWidthMult->text().toInt() <= 0
            || this->ui->txtHeightMult->text().toInt() <= 0){
        QMessageBox mgs;
        mgs.setText("Uno o más de los valores para recortes no es valido.");
        mgs.exec();
        return;
    }

    gSettings->setValue("tags_file", this->ui->txtTagsFile->text());
    gSettings->setValue("quality_file", this->ui->txtQualityFile->text());
    gSettings->setValue("base_width", this->ui->txtBaseWidth->text());
    gSettings->setValue("base_height", this->ui->txtBaseHeight->text());
    gSettings->setValue("width_multiplier", this->ui->txtWidthMult->text());
    gSettings->setValue("height_multiplier", this->ui->txtHeightMult->text());

    base_width = gSettings->value("base_width").toInt();
    base_height = gSettings->value("base_height").toInt();
    width_multiplier = gSettings->value("width_multiplier").toInt();
    height_multiplier = gSettings->value("height_multiplier").toInt();

    QMessageBox mgs;
    mgs.setText("Configuracion guardada.");
    mgs.exec();
}

void CfgDialog::on_btnPickTags_clicked(){
    QString oldval = gSettings->value("tags_file").toString();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Elegir archivo de etiquetas"), "/home");
    if(fileName != "" && fileName != oldval){
        gSettings->setValue("tags_file", fileName);
        this->ui->txtTagsFile->setText(fileName);
    }
}

void CfgDialog::on_btnPickQuality_clicked(){
    QString oldval = gSettings->value("quality_file").toString();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Elegir archivo de calidades"), "/home");
    if(fileName != "" && fileName != oldval){
        gSettings->setValue("quality_file", fileName);
        this->ui->txtQualityFile->setText(fileName);
    }
}

void CfgDialog::on_CfgDialog_rejected(){
    delete this;
}

void CfgDialog::on_btnExit_clicked(){
    this->close();
}
