#include "cacic_computer.h"

CACIC_Computer::CACIC_Computer()
{
    this->coletaDados();
}

/*NetworkInterfacesRunning
 * @return: QList<QList<std::string>>.
 *      Retorna todas as interfaces com seus IP's, nome e MAC.
 *      Cada uma é salva em uma lista que também será salva em
 *      outra lista que conterá todas as interfaces.
 *
 */
QList<QVariantMap> CACIC_Computer::networkInterfacesRunning(){
    QNetworkInterface netInterface;
    QVariantMap mapInterface;
    QList<QVariantMap> listaMap;

    foreach (QNetworkInterface in, netInterface.allInterfaces()) {
        if (!(bool)(in.flags() & QNetworkInterface::IsLoopBack) &&
            !(bool)(in.flags() & QNetworkInterface::IsPointToPoint) &&
            (bool)(in.flags() & QNetworkInterface::IsRunning)){
            mapInterface["nome"] = in.humanReadableName();
            mapInterface["mac"]  = in.hardwareAddress();
            foreach (QNetworkAddressEntry ae, in.addressEntries()){
                if (ae.ip().scopeId() == ""){
                    mapInterface["ipv4"] = ae.ip().toString();
                    mapInterface["netmask_ipv4"] = ae.netmask().toString();
                } else {
                    mapInterface["ipv6"] = ae.ip().toString();
                    mapInterface["netmask_ipv6"] = ae.netmask().toString();
                }
            }
            listaMap.append(mapInterface);
            mapInterface.clear();
        }
    }
    return listaMap;
}

/*********************************************
 * Transforma todos os dados da classe em json
 *********************************************/
QJsonObject CACIC_Computer::toJsonObject()
{
    this->coletaDados();
    QJsonObject json;
    QJsonArray network;

    json["usuario"] = QJsonValue::fromVariant(QString::fromStdString(this->usuario));
    json["operatingSystem"] = this->oOperatingSystem.toJsonObject();
    if (json["operatingSystem"].isNull()){
        return QJsonObject();
    }
    foreach(QVariantMap auxMap, this->getNetworkInterface()){
        network.append(QJsonObject::fromVariantMap(auxMap));
    }
    if (network.isEmpty()){
        return QJsonObject();
    }
    json["networkDevices"] = QJsonValue(network);
    json["nmComputador"] = QJsonValue::fromVariant(QString::fromStdString(this->computerName));
    json["versaoAgente"] = QJsonValue::fromVariant(Identificadores::AGENTE_VERSAO);
    json["versaoGercols"] = QJsonValue::fromVariant(CCacic::getValueFromRegistry("Lightbase", "Cacic", "versao_gercols"));
//    qDebug() << json;
    return json;
}


/*
 * PegarUsu()
 * @return: std::string;
 *      retorna o usuario logado no sistema. (erro, retorna o usuário que está executando.)
*/
std::string CACIC_Computer::pegarUsu(){
    QString user;
#if defined(Q_OS_WIN)
    user = wmi::wmiSearch("Win32_ComputerSystem",QStringList("UserName")).toObject()["UserName"].toString();
    user = user.split("\\").last();

#elif defined(Q_OS_LINUX)
    user = console("who").split(" ")[0];
#endif
    if (user.isEmpty() || user.isNull()) {

        QJsonObject oldColeta = CCacic::getJsonFromFile(CCacic::getValueFromRegistry("Lightbase", "Cacic", "mainFolder").toString() + "/coleta.json");
        QString oldUser = oldColeta["computador"].toObject()["usuario"].toString();

        if (!oldUser.isEmpty() && !oldUser.isNull()) {
            user = oldUser;
        }
    }
    return user.toStdString();
}

void CACIC_Computer::coletaDados() {
#ifdef Q_OS_LINUX
    struct utsname sysName;
    uname(&sysName);
    computerName = sysName.nodename;
#elif defined(Q_OS_WIN)
    QStringList param;
    param << "Caption";
    computerName = wmi::wmiSearch("Win32_ComputerSystem", param).toObject()["Caption"].toString().toStdString();
    param.clear();
#endif
    networkInterface = networkInterfacesRunning();
    usuario = pegarUsu();
}

/*
 * getters/setters
*/
OperatingSystem CACIC_Computer::getOs() const
{
    return oOperatingSystem;
}

std::string CACIC_Computer::getUser() const {
    return usuario;
}

QList<QVariantMap> CACIC_Computer::getNetworkInterface() const
{
    return networkInterface;
}
