/********************************************************************************
** Form generated from reading UI file 'cfgdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CFGDIALOG_H
#define UI_CFGDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_CfgDialog
{
public:
    QGroupBox *grpCrops;
    QLineEdit *txtBaseWidth;
    QLabel *lblBaseWidth;
    QLabel *lblBaseHeight;
    QLineEdit *txtBaseHeight;
    QLabel *lblWidthMult;
    QLineEdit *txtWidthMult;
    QLineEdit *txtHeightMult;
    QLabel *lblHeightMult;
    QPushButton *btnSave;
    QPushButton *btnExit;
    QGroupBox *grpTags;
    QLabel *lblTags;
    QLineEdit *txtTagsFile;
    QPushButton *btnPickTags;
    QLineEdit *txtQualityFile;
    QPushButton *btnPickQuality;
    QLabel *lblQuality;

    void setupUi(QDialog *CfgDialog)
    {
        if (CfgDialog->objectName().isEmpty())
            CfgDialog->setObjectName(QStringLiteral("CfgDialog"));
        CfgDialog->resize(486, 426);
        grpCrops = new QGroupBox(CfgDialog);
        grpCrops->setObjectName(QStringLiteral("grpCrops"));
        grpCrops->setGeometry(QRect(20, 20, 441, 111));
        txtBaseWidth = new QLineEdit(grpCrops);
        txtBaseWidth->setObjectName(QStringLiteral("txtBaseWidth"));
        txtBaseWidth->setGeometry(QRect(90, 30, 113, 25));
        lblBaseWidth = new QLabel(grpCrops);
        lblBaseWidth->setObjectName(QStringLiteral("lblBaseWidth"));
        lblBaseWidth->setGeometry(QRect(10, 30, 81, 21));
        lblBaseHeight = new QLabel(grpCrops);
        lblBaseHeight->setObjectName(QStringLiteral("lblBaseHeight"));
        lblBaseHeight->setGeometry(QRect(10, 70, 81, 21));
        txtBaseHeight = new QLineEdit(grpCrops);
        txtBaseHeight->setObjectName(QStringLiteral("txtBaseHeight"));
        txtBaseHeight->setGeometry(QRect(90, 70, 113, 25));
        lblWidthMult = new QLabel(grpCrops);
        lblWidthMult->setObjectName(QStringLiteral("lblWidthMult"));
        lblWidthMult->setGeometry(QRect(230, 30, 91, 21));
        txtWidthMult = new QLineEdit(grpCrops);
        txtWidthMult->setObjectName(QStringLiteral("txtWidthMult"));
        txtWidthMult->setGeometry(QRect(320, 30, 113, 25));
        txtHeightMult = new QLineEdit(grpCrops);
        txtHeightMult->setObjectName(QStringLiteral("txtHeightMult"));
        txtHeightMult->setGeometry(QRect(320, 70, 113, 25));
        lblHeightMult = new QLabel(grpCrops);
        lblHeightMult->setObjectName(QStringLiteral("lblHeightMult"));
        lblHeightMult->setGeometry(QRect(230, 70, 91, 21));
        btnSave = new QPushButton(CfgDialog);
        btnSave->setObjectName(QStringLiteral("btnSave"));
        btnSave->setGeometry(QRect(140, 380, 91, 31));
        btnExit = new QPushButton(CfgDialog);
        btnExit->setObjectName(QStringLiteral("btnExit"));
        btnExit->setGeometry(QRect(240, 380, 91, 31));
        grpTags = new QGroupBox(CfgDialog);
        grpTags->setObjectName(QStringLiteral("grpTags"));
        grpTags->setGeometry(QRect(20, 150, 441, 171));
        lblTags = new QLabel(grpTags);
        lblTags->setObjectName(QStringLiteral("lblTags"));
        lblTags->setGeometry(QRect(10, 30, 151, 17));
        txtTagsFile = new QLineEdit(grpTags);
        txtTagsFile->setObjectName(QStringLiteral("txtTagsFile"));
        txtTagsFile->setGeometry(QRect(10, 50, 381, 25));
        btnPickTags = new QPushButton(grpTags);
        btnPickTags->setObjectName(QStringLiteral("btnPickTags"));
        btnPickTags->setGeometry(QRect(390, 50, 31, 25));
        txtQualityFile = new QLineEdit(grpTags);
        txtQualityFile->setObjectName(QStringLiteral("txtQualityFile"));
        txtQualityFile->setGeometry(QRect(10, 110, 381, 25));
        btnPickQuality = new QPushButton(grpTags);
        btnPickQuality->setObjectName(QStringLiteral("btnPickQuality"));
        btnPickQuality->setGeometry(QRect(390, 110, 31, 25));
        lblQuality = new QLabel(grpTags);
        lblQuality->setObjectName(QStringLiteral("lblQuality"));
        lblQuality->setGeometry(QRect(10, 90, 151, 17));

        retranslateUi(CfgDialog);

        QMetaObject::connectSlotsByName(CfgDialog);
    } // setupUi

    void retranslateUi(QDialog *CfgDialog)
    {
        CfgDialog->setWindowTitle(QApplication::translate("CfgDialog", "Configuracion", nullptr));
        grpCrops->setTitle(QApplication::translate("CfgDialog", "Recortes", nullptr));
        lblBaseWidth->setText(QApplication::translate("CfgDialog", "Largo Base", nullptr));
        lblBaseHeight->setText(QApplication::translate("CfgDialog", "Alto Base", nullptr));
        lblWidthMult->setText(QApplication::translate("CfgDialog", "Mult. Largo", nullptr));
        txtHeightMult->setText(QString());
        lblHeightMult->setText(QApplication::translate("CfgDialog", "Mult. Alto", nullptr));
        btnSave->setText(QApplication::translate("CfgDialog", "Guardar", nullptr));
        btnExit->setText(QApplication::translate("CfgDialog", "Salir", nullptr));
        grpTags->setTitle(QApplication::translate("CfgDialog", "Etiquetado", nullptr));
        lblTags->setText(QApplication::translate("CfgDialog", "Archivo de etiquetas", nullptr));
        btnPickTags->setText(QApplication::translate("CfgDialog", "...", nullptr));
        txtQualityFile->setText(QString());
        btnPickQuality->setText(QApplication::translate("CfgDialog", "...", nullptr));
        lblQuality->setText(QApplication::translate("CfgDialog", "Archivo de calidades", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CfgDialog: public Ui_CfgDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CFGDIALOG_H
