#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

  private slots:

    void on_browsebtn_clicked();

    void on_driverPathtxt_textChanged(const QString &arg1);

    void on_registerbtn_clicked();

    void on_unregisterbtn_clicked();

    void on_startbtn_clicked();

    void on_stopbtn_clicked();

  private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
