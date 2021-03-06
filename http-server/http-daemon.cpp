#include <http-daemon.h>

void HttpDaemon::incomingConnection(int socket) {
    if (disabled)
        return;

    // When a new client connects, the server constructs a QTcpSocket and all
    // communication with the client is done over this QTcpSocket. QTcpSocket
    // works asynchronously, this means that all the communication is done
    // in the two slots readClient() and discardClient().
    QTcpSocket* s = new QTcpSocket(this);
    connect(s, SIGNAL(readyRead()), this, SLOT(readClient()));
    connect(s, SIGNAL(disconnected()), this, SLOT(discardClient()));
    s->setSocketDescriptor(socket);

    //QtServiceBase::instance()->logMessage("New Connection");
}

void HttpDaemon::pause() {
     disabled = true;
}
void HttpDaemon::resume() {
    disabled = false;
}


void HttpDaemon::readClient()
{
    if (disabled)
        return;

    // This slot is called when the client sent data to the server. The
    // server looks if it was a get request and sends a very simple HTML
    // document back.
    QTcpSocket* socket = (QTcpSocket*)sender();

    if (socket->canReadLine()) {
        QStringList tokens = QString(socket->readLine()).split(QRegExp("[ \r\n][ \r\n]*"));

        qDebug() << tokens;

        if (tokens[0] == "GET" || tokens[0] == "POST") {
            QTextStream os(socket);
            os.setAutoDetectUnicode(true);

            // Retorna dados de acordo com a rota solicitada processamento de acordo com a rota
            QString rota;
            rota = tokens[1];

            os << this->processRoutes(rota);

//            os << "HTTP/1.0 200 Ok\r\n"
//                "Content-Type: text/html; charset=\"utf-8\"\r\n"
//                "\r\n"
//                "<h1>Nothing to see here</h1>\n"
//                << QDateTime::currentDateTime().toString() << "\n";
            socket->close();
//            socket->waitForDisconnected();
//            socket->disconnectFromHost();

            //QtServiceBase::instance()->logMessage("Wrote to client");

            if (socket->state() == QTcpSocket::UnconnectedState) {
                delete socket;
                //QtServiceBase::instance()->logMessage("Connection closed");
            }
        } else if (tokens[0] == "POST") {
            qDebug() << "POST request received.";
        }
    }
}

void HttpDaemon::discardClient()
{
    QTcpSocket* socket = (QTcpSocket*)sender();
    socket->deleteLater();

    //QtServiceBase::instance()->logMessage("Connection closed");
}

QString HttpDaemon::getInstalaHash()
{
    QString retorno;
    retorno = "HTTP/1.0 200 Ok\r\n"
        "Content-Type: application/json; charset=\"utf-8\"\r\n"
        "\r\n"
        "{\"valor\": \"CERTO\"}\n";

    return retorno;
}

QString HttpDaemon::getDefaultRoute(const QString &rota)
{
    QString retorno;
    retorno = "HTTP/1.0 200 Ok\r\n"
        "Content-Type: application/json; charset=\"utf-8\"\r\n"
        "\r\n"
        "{\"valor\":\"route "+rota+" not found\"}\n";

    retorno += QDateTime::currentDateTime().toString();
    retorno += "\n";

    return retorno;
}

QString HttpDaemon::getErro()
{
    QString retorno;

    retorno = "HTTP/1.0 200 Ok\r\n"
        "Content-Type: application/json; charset=\"utf-8\"\r\n"
        "\r\n"
        "{\"valor\": \"OK\"}\n";

    retorno += QDateTime::currentDateTime().toString();
    retorno += "\n";

    return retorno;
}

QString HttpDaemon::getDownload()
{
    QString retorno;

    retorno = "HTTP/1.0 200 Ok\r\n"
        "Content-Type: application/json; charset=\"utf-8\"\r\n"
        "\r\n"
        "{\"valor\": \"http://teste.cacic.cc/downloads/cacic/current/windows/cacic-service.exe\"}\n";

    retorno += "\n";

    return retorno;
}

QString HttpDaemon::getDownloadMsi()
{
    QString retorno;

    retorno = "HTTP/1.0 200 Ok\r\n"
        "Content-Type: application/json; charset=\"utf-8\"\r\n"
        "\r\n"
        "{\"valor\": \"http://teste.cacic.cc/downloads/cacic/current/windows/Cacic.msi\"}\n";

    retorno += "\n";

    return retorno;
}

QString HttpDaemon::getLdapInfo()
{
    QString retorno;

    retorno = "HTTP/1.0 200 Ok\r\n"
        "Content-Type: application/json; charset=\"utf-8\"\r\n"
        "\r\n"
        "{\"objectClass\": \"LDAP_info\",\n"
        "\"info\": {\n"
            "\"base\": \"ou=usuarios,dc=lightbase,dc=com,dc=br\",\n"
            "\"filter\": \"(&(objectClass=*)(uid=thiagop))\",\n"
            "\"login\": \"cn=System Administrator-gosa-admin,ou=usuarios,dc=lightbase,dc=com,dc=br\",\n"
            "\"pass\": \"brlight2012\",\n"
            "\"server\": \"ldap.lightbase\"}\n"
        "}\n";

    retorno += "\n";

    return retorno;
}

QString HttpDaemon::getMapa()
{
    QString retorno;

    retorno = "HTTP/1.0 200 Ok\r\n"
        "Content-Type: application/json; charset=\"utf-8\"\r\n"
        "\r\n"
        "{"
            "\"col_patr\":true,\n"
            "\"ldap\":{\n"
                "\"ldapBase\":\"ou=usuario,dc=dominio,dc=com,dc=br\",\n"
                "\"ldapFilter\":[\n"
                    "\"uid\"\n"
                "],\n"
                "\"ldapLogin\":\"cn=Admin,ou=usuarios,dc=dominio,dc=com,dc=br\",\n"
                "\"ldapPass\":\"pass\",\n"
                "\"ldapServer\":\"teste.ldap.ex\"\n"
            "}\n"
        "}\n";

    retorno += "\n";

    return retorno;
}

QString HttpDaemon::getMapaFormReply()
{
    QString retorno;

    retorno = "HTTP/1.0 200 Ok\r\n"
        "Content-Type: application/json; charset=\"utf-8\"\r\n"
        "\r\n"
        "{\"objectClass\": \"formReply\",\n"
        "\"status\": \"formReply\"\n"
        "}\n";

    retorno += "\n";

    return retorno;
}

QString HttpDaemon::getColeta()
{
    QString retorno;

    retorno = "HTTP/1.0 200 Ok\r\n"
        "Content-Type: application/json; charset=\"utf-8\"\r\n"
        "\r\n"
        "{\"valor\": \"OK\"}\n";

    retorno += QDateTime::currentDateTime().toString();
    retorno += "\n";

    return retorno;
}

QString HttpDaemon::getLog()
{
    QString retorno;

    retorno = "HTTP/1.0 200 Ok\r\n"
        "Content-Type: application/json; charset=\"utf-8\"\r\n"
        "\r\n"
        "{\"valor\": \"OK\"}\n";

    retorno += QDateTime::currentDateTime().toString();
    retorno += "\n";

    return retorno;
}

QString HttpDaemon::processRoutes(const QString &rota)
{
    // Processa a rota definida na variável executando um método para cada rota
    qDebug() << rota;
    if (rota == ROUTE_HASH) {
        return this->getInstalaHash();
    } else if (rota == ROUTE_ERRO) {
        return this->getErro();
    } else if (rota.contains(ROUTE_DOWNLOAD)) {
        return this->getDownload();
    } else if (rota == ROUTE_DOWNLOAD_MSI) {
        return this->getDownloadMsi();
    } else if(rota == ROUTE_MAPA_LDAP) {
        return this->getLdapInfo();
    } else if(rota == ROUTE_MAPA_FORM) {
        return this->getMapaFormReply();
    } else if(rota == ROUTE_MAPA_GETMAPA) {
        return this->getMapa();
    } else if(rota == ROUTE_LOG) {
        return this->getLog();
    } else if(rota == ROUTE_COLETA) {
        return this->getColeta();
    } else {
        return this->getDefaultRoute(rota);
    }
}
