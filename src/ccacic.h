#ifndef CCACIC_H
#define CCACIC_H

#include <QCoreApplication>
#include <QObject>
#include <QFile>
#include <aes.h>
#include <base64.h>
#include <modes.h>
#include <filters.h>
#include <hex.h>
#include <sha.h>
#include <md5.h>
#include <QSettings>
#include <QDir>
#include <QDebug>
//#include <jvson.h>
class CCacic : public QObject
{
    Q_OBJECT
public:
    explicit CCacic(QObject *parent = 0);
    QString getValueFromFile(QString sectionName, QString keyName, QString filePath);
    void setValueToFile(QString sectionName, QString keyName, QString value, QString filePath);
    QString getValueFromTags(QString fullString, QString tag, QString tagType = "[]");
    std::string enCrypt(std::string str_in, std::string key, std::string iv);
    std::string deCrypt(std::string str_in, std::string key, std::string iv);
    bool createFolder(QString path);
    bool deleteFolder(QString path);
    bool deleteFile(QString path);

//Geters/seters:

    QString getCacicMainFolder() const;
    void setCacicMainFolder(const QString &value);

    QString getMainModuleName() const;
    void setMainModuleName(const QString &value);

    QString getUrlGerente() const;
    void setUrlGerente(const QString &value);

    QString getGerColsInfFilePath() const;
    void setGerColsInfFilePath(const QString &value);

    QString getChksisInfFilePath() const;
    void setChksisInfFilePath(const QString &value);

private:
    QString cacicMainFolder;
    QString mainModuleName;
    QString urlGerente;
    QString gerColsInfFilePath;
    QString chksisInfFilePath;

signals:

public slots:

};

#endif // CCACIC_H
