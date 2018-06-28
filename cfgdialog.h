#pragma once
#include <QDialog>

namespace Ui {
class CfgDialog;
}

class CfgDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CfgDialog(QWidget *parent = 0);
    ~CfgDialog();

private slots:
    void on_btnSave_clicked();

    void on_btnPickTags_clicked();

    void on_btnPickQuality_clicked();

    void on_CfgDialog_rejected();

    void on_btnExit_clicked();

private:
    Ui::CfgDialog *ui;
};
