#ifndef WATEK_RUCH_H
#define WATEK_RUCH_H

#include <QThread>
#include <QObject>
#include <QString>

class watek_ruch : public QThread
{
    Q_OBJECT
public:
    watek_ruch();
    void run();

    QString jaki_ruch;
    bool stop;
    int licznik;
signals:
    void opis_ruchu(QString);
};

#endif // WATEK_RUCH_H
