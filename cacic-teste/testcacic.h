#ifndef TESTINSTALLCACIC_H
#define TESTINSTALLCACIC_H

#include <cacic_comm.h>
#include <ccacic.h>
#include <cacic_computer.h>
#include <cacic_software.h>
#include <cacic_hardware.h>
#include <console.h>
#include <operatingsystem.h>
#include <QtTest/QtTest>
#include <QDebug>
#include <QList>
#include <QProcess>
#include <QStringList>
#include <iostream>
#include <QtNetwork/QtNetwork>
#include <QJsonObject>
#include <ccoleta.h>

class CTestCacic : public QObject
{
  Q_OBJECT
public:
  explicit CTestCacic(QObject *parent = 0);

private:
    CacicComm OCacicComm;
    CCacic OCacic;
    CACIC_Computer OCacicComp;
    cacic_software OCacicSoftware;
    cacic_hardware OCacicHardware;
    QString testPath;
    QString testIniPath;
    QJsonObject session;
    QString cripTeste;
    CColeta oColeta;
//    cacicD OcacicD;

signals:

public slots:

private slots:
  void initTestCase();
  void testCommStatus();
  void testComm();
  void testGetAndSetValueFromFile();
  void testGetValueFromTags();
  void testCreateFolder();
  void testDeleteFolder();
  void testDeleteFile();
  void testInterfaceDeRede();
  void testConsole();
  void testPegarUsu();
  void testJsonValueFromJsonString();
  void testcoletaIdOs();
  void testpegarOS();
  void testLogin();
  void testSslConnection();
  void testEnCrypt();
  void testDeCrypt();
  void testCacicCompToJsonObject();
  void testOperatingSystemToJsonObject();
  void testJsonToFile();
  void testJsonFromFile();
  void testStartService();
  void testSetRegistry();
  void testGetValueFromRegistry();
  void testRemoveRegistry();
  void testIniciarDaemon();
  void testColetaSoftware();
  void testColetaHardware();
  void testConvertDouble();
  void testGetTest();
  void testColeta();
  void cleanupTestCase();
};

#endif // TESTINSTALLCACIC_H
