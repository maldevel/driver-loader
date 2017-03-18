/*
    This file is part of driver-loader
    Copyright (C) 2017 @maldevel

    driver-loader - Load a Windows Kernel Driver.
    https://github.com/maldevel/driver-loader

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    For more see the file 'LICENSE' for copying permission.
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dlconfig.h"
#include "dldrivers.h"
#include "dlcommon.h"
#include "dlservices.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDir>

static QString _fileName = 0;

static void _loadFile(Ui::MainWindow *ui, QString filename)
{
    ui->driverVersiontxt->setText(Drivers::GetFileVersion(filename));
    ui->driverSizetxt->setText(QString::number(Drivers::GetDriverFileSize(filename)) + " bytes");
    ui->driverFileTimetxt->setText(Drivers::GetFileLastWriteTime(filename));

    QFileInfo fi(filename);
    ui->serviceNametxt->setText(fi.baseName());
    ui->serviceDisplayNametxt->setText(fi.baseName());
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //disable maximize button and window resizing
    setWindowFlags(Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);

    ui->versionlbl->setText("Ver: V" APP_VERSION " - " APP_DATE);
    ui->serviceTypecmb->addItem("Driver");
    ui->serviceStartcmb->addItems(QList<QString>() << "Automatic" << "Boot" << "Demand" << "Disabled" << "System");
    ui->serviceStartcmb->setCurrentIndex(2);
    ui->serviceErrorcmb->addItems(QList<QString>() << "Critical" << "Ignore" << "Normal" << "Severe");
    ui->serviceErrorcmb->setCurrentIndex(2);

    ui->serviceNametxt->setValidator(new QRegExpValidator(QRegExp("[\\w]{1,256}"), this));
    ui->serviceDisplayNametxt->setValidator(new QRegExpValidator(QRegExp("[\\w]{1,256}"), this));
}

MainWindow::~MainWindow()
{
    Services::uninit();
    delete ui;
}

void MainWindow::on_browsebtn_clicked()
{
    _fileName = QFileDialog::getOpenFileName(this, tr("Select Windows Driver"), "", tr("Windows Drivers Files (*.sys)"));
    ui->driverPathtxt->setText(_fileName);
}

void MainWindow::on_driverPathtxt_textChanged(const QString &arg1)
{
    (void)arg1;
    _loadFile(ui, ui->driverPathtxt->text());
}

void MainWindow::on_registerbtn_clicked()
{
    unsigned long registrationResult = 0;
    QMessageBox msgBox;

    if (ui->driverPathtxt->text().trimmed().isEmpty())
    {
        msgBox.setText("Please provide a valid driver filepath.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }

    if (ui->serviceNametxt->text().trimmed().isEmpty() ||
            ui->serviceNametxt->text().trimmed().length() > 256)
    {
        msgBox.setText("Please provide a name for the service.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }

    if (ui->serviceDisplayNametxt->text().trimmed().isEmpty() ||
            ui->serviceDisplayNametxt->text().trimmed().length() > 256)
    {
        msgBox.setText("Please provide a service name to display.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }

    registrationResult = Services::Register(ui->driverPathtxt->text().trimmed(),
                                            ui->serviceNametxt->text().trimmed(),
                                            ui->serviceDisplayNametxt->text().trimmed(),
                                            ui->serviceStartcmb->itemText(ui->serviceStartcmb->currentIndex()),
                                            ui->serviceErrorcmb->itemText(ui->serviceErrorcmb->currentIndex()));

    switch (registrationResult)
    {
        case ERROR_SERVICE_EXISTS:
            msgBox.setText("Service registration failed. The service already exists.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
            return;

        case 1:
            msgBox.setText("Service registration failed. Empty or invalid parameters have been provided.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();
            return;

        case 0:
            msgBox.setText("Service registration succeeded.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Information);
            msgBox.exec();
            return;

        default:
            msgBox.setText(QString("Service registration failed. Error code %1.").arg(registrationResult));
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();
            return;
    }
}

void MainWindow::on_unregisterbtn_clicked()
{
    unsigned long unregistrationResult = 0;
    QMessageBox msgBox;

    if (ui->serviceNametxt->text().trimmed().isEmpty() ||
            ui->serviceNametxt->text().trimmed().length() > 256)
    {
        msgBox.setText("Please provide service name.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }

    unregistrationResult = Services::Unregister(ui->serviceNametxt->text().trimmed());

    switch (unregistrationResult)
    {
        case 1:
            msgBox.setText("Service unregistration failed.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
            return;

        case ERROR_SERVICE_DOES_NOT_EXIST:
            msgBox.setText("The specified service does not exist as an installed service.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
            return;

        case 0:
            msgBox.setText("Service unregistration succeeded.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Information);
            msgBox.exec();
            return;

        default:
            msgBox.setText(QString("Service unregistration failed. Error code %1.").arg(unregistrationResult));
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();
            return;
    }
}

void MainWindow::on_startbtn_clicked()
{
    QMessageBox msgBox;
    unsigned long startResult = 0;

    if (ui->serviceNametxt->text().trimmed().isEmpty() ||
            ui->serviceNametxt->text().trimmed().length() > 256)
    {
        msgBox.setText("Please provide service name.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }

    startResult = Services::Start(ui->serviceNametxt->text().trimmed());

    switch (startResult)
    {
        case 1:
            msgBox.setText("Starting service failed.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
            return;

        case ERROR_SHARING_VIOLATION:
            msgBox.setText("The process cannot access the file because it is being used by another process.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
            return;

        case ERROR_SERVICE_DOES_NOT_EXIST:
            msgBox.setText("The specified service does not exist as an installed service.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
            return;

        case 0:
            msgBox.setText("Service started.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Information);
            msgBox.exec();
            return;

        default:
            msgBox.setText(QString("Starting service failed. Error code %1.").arg(startResult));
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();
            return;
    }
}

void MainWindow::on_stopbtn_clicked()
{
    QMessageBox msgBox;
    unsigned long stopResult = 0;

    if (ui->serviceNametxt->text().trimmed().isEmpty() ||
            ui->serviceNametxt->text().trimmed().length() > 256)
    {
        msgBox.setText("Please provide service name.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }

    stopResult = Services::Stop(ui->serviceNametxt->text().trimmed());

    switch (stopResult)
    {
        case 1:
            msgBox.setText("Stopping service failed.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
            return;

        case ERROR_SERVICE_NOT_ACTIVE:
            msgBox.setText("The service has not been started.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
            return;

        case ERROR_SERVICE_DOES_NOT_EXIST:
            msgBox.setText("The specified service does not exist as an installed service.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
            return;

        case 0:
            msgBox.setText("Service stopped.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Information);
            msgBox.exec();
            return;

        default:
            msgBox.setText(QString("Stopping service failed. Error code %1.").arg(stopResult));
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();
            return;
    }
}
