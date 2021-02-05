#include <QFileInfo>
#include <QMessageBox>
#include <QFileDialog>

#include "cfgdialog.h"
#include "globals.h"
#include "ui_cfgdialog.h"
#include "settings.h"

CfgDialog::CfgDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CfgDialog)
{
    ui->setupUi(this);

    this->ui->txtTagsFile->setText(Settings::Get("tags_file").toString());
    this->ui->txtQualityFile->setText(Settings::Get("quality_file").toString());

    this->ui->txtBaseWidth->setText(Settings::Get("base_width").toString());
    this->ui->txtBaseHeight->setText(Settings::Get("base_height").toString());
    this->ui->txtWidthMult->setText(Settings::Get("width_multiplier").toString());
    this->ui->txtHeightMult->setText(Settings::Get("height_multiplier").toString());
}

CfgDialog::~CfgDialog(){
    delete ui;
}

void CfgDialog::on_btnSave_clicked(){
    //check if tags and qualities files exist
    if(!QFileInfo::exists(this->ui->txtTagsFile->text())){
        QMessageBox mgs;
        mgs.setText("El archivo de etiquetas no existe.");
        mgs.exec();
        return;
    }
    if(!QFileInfo::exists(this->ui->txtQualityFile->text())){
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

    Settings::Set("tags_file", this->ui->txtTagsFile->text());
    Settings::Set("quality_file", this->ui->txtQualityFile->text());
    Settings::Set("base_width", this->ui->txtBaseWidth->text());
    Settings::Set("base_height", this->ui->txtBaseHeight->text());
    Settings::Set("width_multiplier", this->ui->txtWidthMult->text());
    Settings::Set("height_multiplier", this->ui->txtHeightMult->text());

    base_width = Settings::Get("base_width").toInt();
    base_height = Settings::Get("base_height").toInt();
    width_multiplier = Settings::Get("width_multiplier").toInt();
    height_multiplier = Settings::Get("height_multiplier").toInt();

    QMessageBox mgs;
    mgs.setText("Configuracion guardada.");
    mgs.exec();
}

void CfgDialog::on_btnPickTags_clicked(){
    QString oldval = Settings::Get("tags_file").toString();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Elegir archivo de etiquetas"), "/home");
    if(fileName != "" && fileName != oldval){
        Settings::Set("tags_file", fileName);
        this->ui->txtTagsFile->setText(fileName);
    }
}

void CfgDialog::on_btnPickQuality_clicked(){
    QString oldval = Settings::Get("quality_file").toString();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Elegir archivo de calidades"), "/home");
    if(fileName != "" && fileName != oldval){
        Settings::Set("quality_file", fileName);
        this->ui->txtQualityFile->setText(fileName);
    }
}

void CfgDialog::on_CfgDialog_rejected(){
    delete this;
}

void CfgDialog::on_btnExit_clicked(){
    this->close();
}
