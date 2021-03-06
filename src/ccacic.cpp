#include "ccacic.h"

/* getValueFromFile
 * Pega valor específico dentro de um arquivo
 * @parameter QString sectionName: nome da seção onde estará a key)
 * @parameter QString keyName: nome da key que está o valor)
 * @parameter QString filePath: caminho do arquivo que será aberto;
 *
 * @return QString: "" (vazio) Caso não encontre,
 *                  "0" caso não seja possível abrir o arquivo;
 */
//QString CCacic::getValueFromFile(QString sectionName, QString keyName, QString filePath)
//{
//    QFile file(filePath);
//    QString line = "";
//    sectionName = "[" + sectionName + "]";
//    keyName     = keyName + "=";
//    int sizeKeyName = keyName.size();

//    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
//        return "0";

//    while (!file.atEnd()){
//        line = file.readLine();
//        if (line.contains(sectionName, Qt::CaseInsensitive)) {
//            do {
//                line = file.readLine();
//                if (line.contains(keyName, Qt::CaseInsensitive)) {
//                    file.close();
//                    return line.mid(sizeKeyName).trimmed();
//                }
//            } while (!file.atEnd());
//        }
//    }
//    file.close();
//    return "";
//}
//void CCacic::setValueToFile(QString sectionName, QString keyName, QString value, QString filePath)
//{
//    QSettings settings(filePath, QSettings::IniFormat);
//    settings.beginGroup(sectionName);
//    settings.setValue(keyName, value);
//    settings.endGroup();
//}

/*getValueFromTags
 * @parameter QString fullString: string onde será pesquisado o valor desejado;
 * @parameter QString tag: nome da tag onde estará o valor;
 * @parameter QString tagType: padrão "[]", tipo da tag;
 *
 * @return: QString: valor entre as tags.
 */
//QString CCacic::getValueFromTags(QString fullString, QString tag, QString tagType) {
//    QString tagFim = tagType.mid(0,1) + "/" + tag + tagType.mid(1);
//    int tagSize;
//    tag = tagType.mid(0,1) + tag + tagType.mid(1);
//    tagSize = tag.size();
//    return fullString.mid(fullString.indexOf(tag) + tagSize,
//                          fullString.indexOf(tagFim) -
//                          (fullString.indexOf(tag) + tagSize)).trimmed();
//}
/*createFolder
 * @parameter QString path: caminho onde será criado o diretório, sendo criado toda a árvore se necessário.
 * @return bool: true se conseguir ou já existir, false se não.
 */
bool CCacic::createFolder(QString path)
{
    QDir folder(path);
    return folder.mkpath(path);
}
/*deleteFolder
 * @parameter QString path: caminho do diretório;
 * @return bool: true se conseguir ou já não existir, false se não conseguir.
 */
bool CCacic::deleteFolder(QString path)
{
    QDir folder(path);
    if (folder.exists())
        return folder.removeRecursively();
    else
        return true;
}
/*deleteFile
 * @parameter QString path: caminho do arquivo;
 * @return bool: true se conseguir ou já não existir, false se não conseguir.
 */
bool CCacic::deleteFile(QString path)
{
    QFile file(path);
    if (file.exists())
        return file.remove(path);
    else
        return true;
}
/*jsonValueFromJsonString
 * Pega valor da chave desejada em um json em string;
 * @parameter QString json: json onde será feita a busca;
 * @parameter QString key: chave onde será pego o json;
 * @return iter: valor solicitado;
 */
//QJsonValue CCacic::jsonValueFromJsonString(QString json, QString key)
//{
//    return QJsonDocument::fromJson(json.toUtf8()).object()[key];
//}
/*setJsonToFile
 * Grava json em um arquivo.
 * IMPORTANTE: não parei pra olhar a fundo, mas a princípio é necessário ler o arquivo
 * para pegar o json das informações anteriores, senão informações serão duplicadas ou
 * excluidas;
 * @parameter QJsonObject json: json que será gravado
 * @parameter QString filepath: caminho do arquivo onde será gravado
 *
 */
bool CCacic::setJsonToFile(QJsonObject json, QString filepath)
{
    if (json.isEmpty()){
        return false;
    }
    QFile configFile(filepath);
    if (!configFile.open(QIODevice::WriteOnly)){
        return false;
    }
    QJsonDocument docJson(json);

    return (configFile.write(docJson.toJson()) != -1);
}

/*getJsonFromFile
 * Carrega json de um arquivo.
 * @return QJsonObject: json que será recuperado
 * @parameter QString filepath: caminho do arquivo onde será recuperado
 */
QJsonObject CCacic::getJsonFromFile(QString filepath)
{
    QFile configFile(filepath);
    QJsonDocument jsonDoc;
    QJsonObject json;
    QJsonParseError error;

    if (!configFile.open(QIODevice::ReadOnly)){
        return json;
    }

    jsonDoc = QJsonDocument::fromJson(configFile.readAll(),&error);
    if( error.error == QJsonParseError::NoError){
        json = jsonDoc.object();
    } else {
        qDebug() << "Erro no parsing do arquivo json:" + filepath;
        qDebug() << error.errorString();

        exit(1);
    }
    return json;
}

/*enCrypt
 * @parameter std::string str_in: string que será encriptada (url).
 *            std::string key: chave utilizada na encriptação (32 caracteres) 32*8 = 256 bits
 *              *exemplo: qwertyuiopasdfghjklzxcvbnmqwerty
 *            std::string iv: IV (Vetor de Inicialização) deve ser aleatório.
 *              (http://pt.wikipedia.org/wiki/Modo_de_opera%C3%A7%C3%A3o_%28criptografia%29#Vetor_de_inicializa.C3.A7.C3.A3o_.28IV.29)
 *              exemplo de iv: 0123456789123456
 * @return std:string: retorna a string encriptada convertida em base64.
 * */
//QString CCacic::enCrypt(std::string str_in, std::string iv) {
//    std::string str_out;
//    if ((!this->getChaveCrypt().isNull())){
//        std::string key = (!this->getChaveCrypt().isNull()) ? this->getChaveCrypt().toStdString() : "";
//        CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption encryption((byte*)key.c_str(), key.length(), (byte*)iv.c_str());
//        CryptoPP::StringSource encryptor(str_in, true,
//                                         new CryptoPP::StreamTransformationFilter(encryption,
//                                            new CryptoPP::Base64Encoder(new CryptoPP::StringSink(str_out),
//                                                false // do not append a newline
//                                                )
//                                            )
//                                        );
//    }

//    return QString::fromStdString(str_out);
//}

/*deCrypt
// * @parameter std::string str_in: string encriptada convertida em base64.
// *            std::string key: chave utilizada na encriptação (32 caracteres) 32*8 = 256 bits
// *              *exemplo: qwertyuiopasdfghjklzxcvbnmqwerty
// *            std::string iv: IV (Vetor de Inicialização) deve ser aleatório.
// *              *Um IV jamais deve ser utilizado mais de uma vez com a mesma chave.
// *              *(http://pt.wikipedia.org/wiki/Modo_de_opera%C3%A7%C3%A3o_%28criptografia%29#Vetor_de_inicializa.C3.A7.C3.A3o_.28IV.29)
// *              *exemplo de iv: 0123456789123456
// * @return QString: retorna a string desencriptada convertida em base64.
// * */
//QString CCacic::deCrypt(std::string str_in, std::string iv) {
//    std::string str_out;
//    if ((!this->getChaveCrypt().isNull())){
//        std::string key = this->getChaveCrypt().toStdString();
//        CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption decryption((byte*)key.c_str(), key.length(), (byte*)iv.c_str());

//        CryptoPP::StringSource decryptor(str_in, true,
//                                        new CryptoPP::Base64Decoder(
//                                            new CryptoPP::StreamTransformationFilter(decryption,
//                                                new CryptoPP::StringSink(str_out))
//                                           )
//                                         );
//    }
//    return QString::fromStdString(str_out);
//}

/********************************
 * Compara o MD5 de dois arquivos
 ********************************/
bool CCacic::Md5IsEqual(QVariant document01,QVariant document02){
    QString file1 = QString(QCryptographicHash::hash(
                                (document01.toByteArray()),QCryptographicHash::Md5).toHex());
    QString file2 = QString(QCryptographicHash::hash(
                                (document02.toByteArray()),QCryptographicHash::Md5).toHex());
    return file1 == file2;
}
/***********************************************************
 * Compara o MD5 de um arquivo com uma string (o MD5 em si)
 ***********************************************************/
bool CCacic::Md5IsEqual(QByteArray document01,QString document02){
    QString file1 = QString(QCryptographicHash::hash(
                                document01,QCryptographicHash::Md5).toHex());
    QString file2 = document02;

    return file1 == file2;
}

/******************************************
 * Método com funcionamento duvidoso...
 * TO DO: Ou exclui ou deixá-lo funcional.
 ******************************************/
QString CCacic::startProcess(QString pathprogram, bool wait, bool *ok, QStringList arguments)
{
    QProcess process;
    arguments.empty() ? process.start(pathprogram) : process.start(pathprogram, arguments);
    *ok = wait ? process.waitForFinished() : process.waitForStarted();
    return process.errorString();
}

/***********************************************************
 * Salva valores em um registro local do sistema operacional.
 * Mais detalhes verifique QSettings
 ***********************************************************/
void CCacic::setValueToRegistry(QString organization, QString application, QVariantMap values)
{
    QSettings registry(QSettings::SystemScope, organization, application);
    for (QVariantMap::const_iterator i = values.constBegin(); i != values.constEnd(); i++)
        registry.setValue(i.key(), i.value());
    registry.sync();
}

/***********************************************************
 * Recupera valores em um registro local do sistema operacional.
 * Mais detalhes verifique QSettings
 ***********************************************************/
QVariant CCacic::getValueFromRegistry(QString organization, QString application, QString key)
{
    QSettings registry(QSettings::SystemScope, organization, application);
    return registry.value(key);
}

/***********************************************************
 * Remove valores de um registro local do sistema operacional.
 * Mais detalhes verifique QSettings
 ***********************************************************/
void CCacic::removeRegistry(QString organization, QString application)
{
    QSettings registry(QSettings::SystemScope, organization, application);
    registry.clear();
    registry.sync();
}

QString CCacic::toMD5(QString string){
    return QString(QCryptographicHash::hash((string.toLatin1()),QCryptographicHash::Md5).toHex());
}

QString CCacic::convertDouble(const double &number, const int &precision)
{
    std::ostringstream ss;
    ss.precision(precision);
    ss << std::fixed << number;

    return QString::fromStdString(ss.str()).trimmed();
}

std::string CCacic::genRandomString(const int &len)
{
    char* s;

    s = (char *) malloc(sizeof(char) * len);

    static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    s[len] = 0;
    return std::string(s);
}

/**************************************************************
 * Verifica se esta execultando com permissoes de admnistrador.
 *************************************************************/
bool CCacic::verificarRoot(){
#ifdef Q_OS_WIN
    QFile myFile("C:\\Windows\\System32\\cacic.tmp");
#elif defined(Q_OS_LINUX)
    QFile myFile("/etc/cacic.tmp");
#endif
    if(myFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
        if(myFile.exists()){
            myFile.remove();
        }
        return true;
    } else {
        return false;
    }
    return false;
}

/***********************************************************
 * Verifica se o cacic-agente ja esta instalado.
 ***********************************************************/
bool CCacic::verificarCacicInstalado() {    
#ifdef Q_OS_WIN
    ServiceController service(QString(CACIC_SERVICE_NAME).toStdWString());
    if(service.isInstalled() || service.isRunning()){
        return true;
    }else{
        return false;
    }
#elif defined (Q_OS_LINUX)
    //    QStringList packageInfo = console(QString("dpkg --get-selections | grep -v '\^lib\\|\^fonts' | grep cacic-agente")).split("\n", QString::SkipEmptyParts);
    //    if(!packageInfo.isEmpty()){
    //        QStringList novaLista = packageInfo.takeFirst().trimmed().split("\t");
    //        if(novaLista.takeFirst() == "cacic-agente" && novaLista.takeLast() == "install"){
    ConsoleObject console;
    QStringList status = console(QString("service cacic3 status")).trimmed().split(" ", QString::SkipEmptyParts);
    if(status.isEmpty()){
        return false;
    }else{
        if((QString::compare(status.at(status.size() - 2), "unrecognized", Qt::CaseSensitive) == 0)
                || (QString::compare(status.at(status.size() - 2), "not", Qt::CaseSensitive) == 0)){
            return false;
        }
    }
    return true;
#endif
}

QString CCacic::padronizarData(QString data){
#ifdef Q_OS_WIN
    if (!data.isEmpty() && !data.isNull())
        return QString(data.mid(6,2) +"/"+ data.mid(4,2)+"/"+data.mid(0,4));
#elif defined (Q_OS_LINUX)
    if (!data.isEmpty() && !data.isNull()){
        QString dia, mes, ano;
        QStringList aux = data.split(" ");
        if (aux.size() >= 10){
            if (aux.at(10).isEmpty() && aux.size() >= 11)
                dia = aux.at(11);
            else
                dia = aux.at(10);
            mes = getMesFromString(aux.at(9));
            ano = aux.takeLast();
        }
        return QString(dia+"/" + mes + "/" + ano);
    }
#endif
    return "";
}

QString CCacic::getMesFromString(QString mes){
    if(mes==(QString("Jan"))){return "01";}
    if(mes==(QString("Fev")) || mes==(QString("Feb"))){return "02";}
    if(mes==(QString("Mar"))) return "03";
    if(mes==(QString("Abr")) || mes==(QString("Apr"))){return "04";}
    if(mes==(QString("Mai")) || mes==(QString("May"))){return "05";}
    if(mes==(QString("Jun"))){return "06";}
    if(mes==(QString("Jul"))){return "07";}
    if(mes==(QString("Ago")) || mes==(QString("Aug"))){return "08";}
    if(mes==(QString("Set")) || mes==(QString("Sep"))){return "09";}
    if(mes==(QString("Out")) || mes==(QString("Oct"))){return "10";}
    if(mes==(QString("Nov"))){return "11";}
    if(mes==(QString("Dez")) || mes==(QString("Dec"))){return "12";}
    return "00";
}

void CCacic::salvarVersao(QString modulo){
    QVariantMap ver;
    ver[QString("versao_").append(modulo)] = Identificadores::AGENTE_VERSAO;
    setValueToRegistry("Lightbase", "Cacic", ver);
}

bool CCacic::findProc(const char *name)
{
    QDir dir;
    char buf[512];

    long  pid;
    char pname[100] = {0,};
    char state;
    FILE *fp=NULL;

    dir.setPath("/proc");
    if (!dir.isReadable()) {
        return false;
    }

    QFileInfoList list = dir.entryInfoList(QDir::Dirs);
    for (int i = 0; i<list.size(); i++){
        long lpid = atol(list.at(i).baseName().toStdString().c_str());
        if(lpid < 0)
            continue;
        snprintf(buf, sizeof(buf), "/proc/%ld/stat", lpid);
        fp = fopen(buf, "r");

        if (fp) {
            if ( (fscanf(fp, "%ld (%[^)]) %c", &pid, pname, &state)) != 3 ){
                fclose(fp);
                return false;
            }
            if (!strcmp(pname, name)) {
                fclose(fp);
                return true;
            }
            fclose(fp);
        }
    }


    return false;
}

#ifdef Q_OS_WIN
/**
 * @brief CCacic::changeCacicVersion
 * Altera o valor do da versão do cacic no regedit para a versão que está rodando.
 * @return verdadeiro caso tenha sido encontrado o software cacic encontrado e o valor seja alterado.
 */
bool CCacic::changeCacicVersion()
{
    bool found = false;
    using namespace voidrealms::win32;
    QStringList regedit;
    //No windows, ele armazena os dados em 2 locais diferentes se for x64. Um para programas x86 e outro pra x64.
    regedit.append("SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\");
    regedit.append("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\");
    foreach(QString registry, regedit){
        VRegistry reg;
        reg.OpenKey(HKEY_LOCAL_MACHINE, registry);
        QStringList keys = reg.enum_Keys();
        foreach(QString key, keys){
            VRegistry subReg;
            subReg.OpenKey(HKEY_LOCAL_MACHINE, registry + key);
            if (subReg.get_REG_SZ("DisplayName") == "Cacic"){
                qDebug() << "REGISTRO DO CACIC ENCONTRADO!!!! <<<<<<";
                subReg.set_REG_SZ("DisplayVersion", Identificadores::AGENTE_VERSAO);
                found = true;
            }
        }
        if (found) break;
    }
    return found;
}
#endif
