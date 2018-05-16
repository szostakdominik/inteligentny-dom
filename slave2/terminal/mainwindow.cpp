/****************************************************************************
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Copyright (C) 2012 Laszlo Papp <lpapp@kde.org>
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QLabel>
#include <QtSerialPort/QSerialPort>

#define samples 1000

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {

    ui->setupUi(this);

    //tworzenie zmiennych
    wywolaj_ruch1 = new QPushButton;
    temperatura1 = new QLCDNumber;
    otworz_drzwi1 = new QCheckBox;
    otworz_okno1 = new QCheckBox;
    ustaw_stezenie1 = new QDoubleSpinBox;

    //przypisywanie zmiennych
    temperatura1 = ui->temperatura1;
    wywolaj_ruch1 = ui->wywolaj_ruch1;
    otworz_drzwi1 = ui->otworz_drzwi1;
    otworz_okno1 = ui->otworz_okno1;
    ustaw_stezenie1 = ui->ustaw_stezenie1;

    //laczenie obiektow
    connect(otworz_drzwi1, SIGNAL(toggled(bool)), this, SLOT(ustaw_drzwi1()));
    connect(otworz_okno1, SIGNAL(toggled(bool)), this, SLOT(ustaw_okno1()));
    connect(wywolaj_ruch1, SIGNAL(clicked(bool)), this, SLOT(ustaw_ruch1()));
    connect(ustaw_stezenie1, SIGNAL(valueChanged(double)), this, SLOT(poziom_dymu1()));

    //do wyswietlania obrazkow
    //zarowki
    obr_zarowki1 = new QLabel;
    obr_zarowki1 = ui->obr_zarowki1;
    zarowka1 = new QPixmap;
    zarowka1->load(":/zarowka_1.png");
    zarowka2 = new QPixmap;
    zarowka2->load(":/zarowka_2.png");
    //okna
    obr_okna1 = new QLabel;
    obr_okna1 = ui->obr_okna1;
    okno1 = new QPixmap;
    okno1->load(":/okno_1.png");
    okno2 = new QPixmap;
    okno2->load(":/okno_2.png");
    //drzwi
    obr_drzwi1 = new QLabel;
    obr_drzwi1 = ui->obr_drzwi1;
    drzwi1 = new QPixmap;
    drzwi1->load(":/drzwi_1.png");
    drzwi2 = new QPixmap;
    drzwi2->load(":/drzwi_2.png");

    //poczatkowe ustawienia obrazkow:
    obr_zarowki1->setPixmap(*zarowka1);
    obr_okna1->setPixmap(*okno1);
    obr_drzwi1->setPixmap(*drzwi1);

    //wstepne ustawienie
    ui->swiatlo1->setText("wyłączone");
    ui->wentylacja1->setText("wyłączona");

    serial = new QSerialPort(this);
    settings = new SettingsDialog;
    serial_rx = new serial_rx_t;


    connect(ui->actionConfigure, SIGNAL(triggered()), settings, SLOT(show()));
    connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(openSerialPort()));
    connect(ui->actionDisconnect, SIGNAL(triggered()), this, SLOT(closeSerialPort()));
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));

    connect(serial, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error), this, &MainWindow::handleError);
    connect(serial, &QSerialPort::readyRead, this, &MainWindow::readData);

    serial_rx->status = idle;
}



MainWindow::~MainWindow() {

    delete settings;
    delete serial;
    delete serial_rx;
    delete ui;

}



void MainWindow::openSerialPort() {

    SettingsDialog::Settings p = settings->settings();

    serial->setPortName(p.name);
    serial->setBaudRate(p.baudRate);
    serial->setDataBits(p.dataBits);
    serial->setParity(p.parity);
    serial->setStopBits(p.stopBits);
    serial->setFlowControl(p.flowControl);

    if (serial->open(QIODevice::ReadWrite)) {

        ui->actionConfigure->setEnabled(false);
        ui->actionConnect->setEnabled(false);
        ui->actionDisconnect->setEnabled(true);
        ui->actionQuit->setEnabled(false);

    } else {

        QMessageBox::critical(this, tr("Error."), serial->errorString());

    }

}



void MainWindow::closeSerialPort() {

    if (serial->isOpen())
        serial->close();

    ui->actionConfigure->setEnabled(true);
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionQuit->setEnabled(true);

}



void MainWindow::writeData(const QByteArray &data) {

        serial->write(data);

}



void MainWindow::readData() {

    QByteArray data = serial->readAll();
    QByteArray komenda1 = "-s21\r";
    QByteArray komenda2 = "-s20\r";
    QByteArray komenda3 = "-w21\r";
    QByteArray komenda4 = "-w20\r";



    for( int i = 0; i < data.size(); i++ ) {





        if( (data.at(i) == '-') && (!serial_rx->status) ) {

            serial_rx->status = receiving_string;
            serial_rx->frame.clear();

        }

        else if( serial_rx->status == receiving_string ) {

            if( data.at(i) != '\r' )
                serial_rx->frame.append(data.at(i));

            else {

                serial_rx->status = idle;
                if(data==komenda1){
                    ui->swiatlo1->setText("włączone");
                    obr_zarowki1->setPixmap(*zarowka2);
                }else if(data==komenda2){
                    ui->swiatlo1->setText("wyłączone");
                    obr_zarowki1->setPixmap(*zarowka1);
                }else if(data==komenda3){
                    ui->wentylacja1->setText("włączone");
                }else if(data==komenda4){
                    ui->wentylacja1->setText("wyłączone");
                }

            }

        }else{
            double val;
            QString wartosc = data;
            val = wartosc.toDouble();
            if(val>100){
            if (watek_temp1){
                if (watek_temp1->stop == false){
                    watek_temp1->stop = true;
                    watek_temp1->wait();
                    delete watek_temp1;
                    watek_temp1 = NULL;
                }
            }
            val=val-100.00;
            watek_temp1 = new watek_temperatura(temperatura1->value(),val);
            connect(watek_temp1, SIGNAL(obecna_temp(double)), temperatura1, SLOT(display(double)));
            connect(watek_temp1, SIGNAL(obecna_temp(double)), temperatura1, SLOT(wyslij_temp()));
            watek_temp1->start();
            }
        }




    }

}



void MainWindow::handleError(QSerialPort::SerialPortError error) {

    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error."), serial->errorString());
        closeSerialPort();
    }

}



void MainWindow::on_otworz_okno1_toggled(bool checked)
{
    if(checked){
        writeData("-o21\r");
        obr_okna1->setPixmap(*okno2);
    }else{
        writeData("-o20\r");
        obr_okna1->setPixmap(*okno1);
    }
}

void MainWindow::on_otworz_drzwi1_toggled(bool checked)
{
    if(checked){
        writeData("-d21\r");
        obr_drzwi1->setPixmap(*drzwi2);
    }else{
        writeData("-d20\r");
        obr_drzwi1->setPixmap(*drzwi1);
    }
}

void MainWindow::on_wywolaj_ruch1_clicked()
{
    writeData("-r2\r");
}

void MainWindow::on_ustaw_stezenie1_editingFinished()
{
    double value = ustaw_stezenie1->value()+100.00;
    QString valuestring = QString::number(value);
    QByteArray array = valuestring.toUtf8();
    writeData(array);
}
