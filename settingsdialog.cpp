#include <QSettings>

#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    connect(this, SIGNAL(accepted()),
            this, SLOT(saveSettings()));
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::saveSettings()
{
    QSettings s;
    s.setValue("phone", ui->numEdit->text());
    s.setValue("passwd", ui->passwdEdit->text());
}
