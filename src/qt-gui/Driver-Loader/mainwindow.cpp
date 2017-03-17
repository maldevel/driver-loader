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

#include <QFileDialog>

static QString fileName = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //disable maximize button and window resizing
    setWindowFlags(Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);

    ui->versionlbl->setText("Ver: V" APP_VERSION " - " APP_DATE);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_exitbtn_clicked()
{
    QCoreApplication::quit();
}

void MainWindow::on_browsebtn_clicked()
{
    fileName = QFileDialog::getOpenFileName(this, tr("Select Windows Driver"), "", tr("Windows Drivers Files (*.sys)"));

    ui->driverPathtxt->setText(fileName);
    ui->driverVersiontxt->setText(Drivers::GetFileVersion(fileName));
}

void MainWindow::on_driverPathtxt_textChanged(const QString &arg1)
{
    ui->driverVersiontxt->setText(Drivers::GetFileVersion(ui->driverPathtxt->text()));
}
