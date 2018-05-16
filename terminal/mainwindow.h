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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H



#include <QtCore/QtGlobal>
#include <QtSerialPort/QSerialPort>
#include <QMainWindow>
#include <QDoubleSpinBox>
#include <QLCDNumber>
#include <QCheckBox>
#include <QTextBrowser>
#include <QThread>
#include <QPushButton>
#include <QString>
#include <QTimer>
#include "watek_temperatura.h"

#include "settingsdialog.h"



typedef enum {
    idle = 0,
    receiving_data,
    receiving_string
} serial_rx_status_t;

typedef struct {
    serial_rx_status_t status;
    QByteArray frame;
} serial_rx_t;



QT_BEGIN_NAMESPACE

class QLabel;

namespace Ui {
    class MainWindow;
}

QT_END_NAMESPACE



class SettingsDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    watek_temperatura *watek_temp1 = NULL;

private slots:
    void openSerialPort();
    void closeSerialPort();
    void writeData(const QByteArray &data);
    void readData();
    void handleError(QSerialPort::SerialPortError error);

    void on_wl_swiatlo1_toggled(bool checked);

    void on_wl_wentylacja1_toggled(bool checked);

    void on_wl_swiatlo2_toggled(bool checked);
    void on_wl_wentylacja2_toggled(bool checked);

    void on_limit_dymu1_editingFinished();

    void on_limit_dymu2_editingFinished();

    void on_ustaw_temp1_editingFinished();

    void on_ustaw_temp2_editingFinished();

public slots:
    void brak_ruchu();


private:
    Ui::MainWindow *ui;

    QDoubleSpinBox *temp1;
    QDoubleSpinBox *temp2;
    QDoubleSpinBox *limit_dymu1;
    QDoubleSpinBox *limit_dymu2;
    double stezenie1;
    double stezenie2;

    QLCDNumber *m_temp_wysw1;
    QLCDNumber *m_temp_wysw2;

    QCheckBox *swiatlo1;
    QCheckBox *swiatlo2;
    QCheckBox *wentylacja1;
    QCheckBox *wentylacja2;

    QTextBrowser *czujnik_ruchu1;
    QTextBrowser *czujnik_ruchu2;
    QTextBrowser *kontrola_dymu1;
    QTextBrowser *kontrola_dymu2;
    QTextBrowser *czujnik_drzwi1;
    QTextBrowser *czujnik_drzwi2;
    QTextBrowser *czujnik_okna1;
    QTextBrowser *czujnik_okna2;

    QString tymczasowy_napis;

    SettingsDialog *settings;
    QSerialPort *serial;
    serial_rx_t *serial_rx;

    //do obrazkow
    QLabel *obr_zarowki1;
    QLabel *obr_okna1;
    QLabel *obr_drzwi1;
    QPixmap *zarowka1;
    QPixmap *zarowka2;
    QPixmap *okno1;
    QPixmap *okno2;
    QPixmap *drzwi1;
    QPixmap *drzwi2;

};



#endif // MAINWINDOW_H
