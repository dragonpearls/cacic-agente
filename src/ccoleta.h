/*-----------------------------------------------------------------------------------------
 *
 * Project Cacic Agente
 *    File created by Lightbase
 *
 * Developers: Eric Menezes Noronha (eric.m.noronha@lightbase.com.br); GitHub: ericmenezes
 *             Thiago Rocha         (thiago.rocha@lightbase.com.br)  ;
 *
 *-----------------------------------------------------------------------------------------*/

#ifndef CCOLETA_H
#define CCOLETA_H
#include <QtCore>
#include <QObject>
#include <QJsonObject>
#include <QDebug>
#include <ccacic.h>
#include <cacic_computer.h>
#include <cacic_software.h>
#include <cacic_hardware.h>
#include "logcacic.h"

class CColeta : public QObject
{
    Q_OBJECT

private:

    CACIC_Computer oComputer;
    cacic_software oSoftware;
    cacic_hardware oHardware;
    bool hardwareIsFinish;
    bool softwareIsFinish;
    LogCacic *logcacic;

public:
    explicit CColeta(QObject *parent = 0);

    CACIC_Computer getOComputer() const;
    cacic_software getOSoftware() const;
    cacic_hardware getOHardware() const;
    bool waitToCollect();
    QJsonObject toJsonObject();
    void setHardwareExceptionClasses(const QHash<QString, QStringList> &exceptions);
    void setHardwareExceptionClasses(const QJsonObject &exceptions);
    void setSoftwareExceptionClasses(const QHash<QString, QStringList> &exceptions);
    void setSoftwareExceptionClasses(const QJsonObject &exceptions);

public slots:
    void coletaHardware();
    void coletaSoftware();
    void configuraColetas();
    void hardwareReady();
    void softwareReady();
    void run();

signals:
    void beginComputer();
    void beginHardware();
    void beginSoftware();
    void hardwareFinish();
    void softwareFinish();
    void finished();
};

#endif // CCOLETA_H
