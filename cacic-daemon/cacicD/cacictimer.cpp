#include "cacictimer.h"

CacicTimer::CacicTimer(QString dirpath)
{
    logManager = QLogger::QLoggerManager::getInstance();
    logManager->addDestination(dirpath + "/Logs/cacic.log",Identificadores::LOG_DAEMON_TIMER,QLogger::InfoLevel);
    logManager->addDestination(dirpath + "/Logs/cacic.log",Identificadores::LOG_DAEMON_TIMER,QLogger::ErrorLevel);
    setApplicationDirPath(dirpath);
    iniciarInstancias();
    connect(timer,SIGNAL(timeout()),this,SLOT(mslot()));
}

CacicTimer::~CacicTimer()
{
    logManager->closeLogger();
}

void CacicTimer::reiniciarTimer(){
    timer->stop();
    timer->start(getPeriodicidadeExecucao());
}


void CacicTimer::iniciarTimer()
{
    //    if(comunicarGerente()){
    //        checkModules->start();
    //        //TODO: FAZER O SERVIÇO SE MATAR APÓS A CHAMADA DO INSTALLCACIC CASO ELE PRECISE SER ATUALIZADO.
    //        verificarModulos();
    //        verificarPeriodicidade();r
    //        if (verificarEIniciarQMutex()) {
    //            iniciarThread();
    //        }
    //        timer->start(getPeriodicidadeExecucao());
    //    }else{
    //        timer->start(this->periodicidadeExecucaoPadrao * 60000);
    //        QLogger::QLog_Error(Identificadores::LOG_DAEMON_TIMER, QString("Problemas na comunicação com o gerente. Setando periodicidade padrão."));
    //    }

    //iniciar em 2 minutos devido à placa de rede que às vezes não sobe à tempo.
    timer->start(2 * 60000);
}

//Slot que será iniciado sempre der a contagem do timer.
void CacicTimer::mslot(){
    if(comunicarGerente()){

        if ( QFile(ccacic->getCacicMainFolder() + "/cacic280.exe" ).exists() ) {
            if( !removeArquivosEstrangeiros(QDir(ccacic->getCacicMainFolder())) ) {
                QLogger::QLog_Info(Identificadores::LOG_DAEMON_TIMER, QString("Problemas ao remover arquivos não pertencentes a esta versão do Cacic."));
            }
        }

        if (!checkModules->start()){
            QLogger::QLog_Info(Identificadores::LOG_DAEMON_TIMER, QString("Problemas ao checkar módulos."));
        }
        verificarModulos();
        if (verificarEIniciarQMutex()) {
            iniciarThread();
            if(verificarPeriodicidade()){
                reiniciarTimer();
            }
        }
    } else {
        QLogger::QLog_Info(Identificadores::LOG_DAEMON_TIMER, QString("Problemas ao comunicar com gerente."));
    }
}

/*************************************************************
 * Verifica se a thread ainda está rodando, se tiver mata ela.
 *************************************************************/
bool CacicTimer::verificarEIniciarQMutex(){
    /*
     * para o refactoring: Quando matar o gercols travado na memoria imediatamente ja inicia outra coleta
     *
     */
    if(!cacicthread->isRunning()){
        cMutex->lock();
        QLogger::QLog_Info(Identificadores::LOG_DAEMON_TIMER, "Semáforo fechado com sucesso.");
        return true;
    }else{
        QLogger::QLog_Info(Identificadores::LOG_DAEMON_TIMER, "Possivelmente o gercols travou e será finalizado.");
        try{
            cacicthread->terminate();
            QLogger::QLog_Info(Identificadores::LOG_DAEMON_TIMER, "Gercols finalizado com sucesso.");
        }catch (...){
            QLogger::QLog_Error(Identificadores::LOG_DAEMON_TIMER, "Falha ao finalizar gercols.");
            return false;
        }
        cMutex->lock();
        QLogger::QLog_Info(Identificadores::LOG_DAEMON_TIMER, "Semáforo fechado com sucesso.");
        return true;
    }
}
/**********************************************************************************************
 * Verifica se há algum módulo na pasta temporária, se tiver remove o antigo da pasta principal
 * e substitui pelo novo
 **********************************************************************************************/
bool CacicTimer::verificarModulos()
{
    QDir dir(ccacic->getCacicMainFolder() + "/temp");
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks | QDir::Executable);
    dir.setSorting(QDir::Size | QDir::Reversed);

    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i<list.size(); i++){
#ifdef Q_OS_WIN
        if(!(list.at(i).fileName() == "cacic-service.exe")){
#else
        if(!(list.at(i).fileName() == "cacic-service")){
#endif
            QLogger::QLog_Info(Identificadores::LOG_DAEMON_TIMER, "Módulo \"" + list.at(i).filePath() + "\" encontrado para atualização.");
            QFile novoModulo(list.at(i).filePath());
            if (QFile::exists(applicationDirPath + "/" + list.at(i).fileName())){
                QLogger::QLog_Info(Identificadores::LOG_DAEMON_TIMER, "Excluindo versão antiga de " + list.at(i).fileName());
                QFile::remove(applicationDirPath + "/" + list.at(i).fileName());
            }
            if (!QFile::exists(applicationDirPath + "/" + list.at(i).fileName())){
                novoModulo.copy(applicationDirPath + "/" + list.at(i).fileName());
                if (!novoModulo.remove())
                    QLogger::QLog_Info(Identificadores::LOG_DAEMON_TIMER, "Falha ao excluir "+list.at(i).fileName()+" da pasta temporária.");
            } else {
                QLogger::QLog_Info(Identificadores::LOG_DAEMON_TIMER, "Falha ao excluir módulo antigo"+list.at(i).fileName()+" da pasta temporária.");
            }

            novoModulo.close();
        } else {
            QLogger::QLog_Info(Identificadores::LOG_DAEMON_TIMER, "Atualização do serviço.");
            QStringList arg;
            arg << "-updateService";
            QProcess installCacicProc;
            installCacicProc.startDetached(ccacic->getCacicMainFolder() + "/install-cacic", arg);

            QLogger::QLog_Info(Identificadores::LOG_DAEMON_TIMER, "Matando serviço.");
#ifdef Q_OS_WIN
            system("sc stop cacicdaemon");
#else
            ConsoleObject console;
            console("/etc/init.d/cacic3 stop");
#endif
            logManager->closeLogger();
            emit finalizar();
            break;
        }
    }
    list.clear();
    return true;
}

bool CacicTimer::verificarseModuloJaFoiExecutado(QString nome, QString hash){
    QFile file("/etc/xdg/Lightbase/Cacic.conf");
    if(!file.open(QIODevice::ReadOnly)) {
        QLogger::QLog_Info(Identificadores::LOG_DAEMON_TIMER, QString("Erro ao tentar ler o /etc/xdg/Lightbase/Cacic.conf."));
    }
    QTextStream in(&file);
    QStringList fields;
    while(!in.atEnd()) {
        QString line = in.readLine();
        fields.append(line.split(","));
    }
    file.close();
    if(fields.contains(QString(nome.append("=").append(hash)))){
        return true;
    }else{
        return false;
    }
}

void CacicTimer::iniciarThread(){    
    QJsonObject agenteConfigJson;
    QJsonArray listaModulos;
    QJsonObject result = ccacic->getJsonFromFile(this->applicationDirPath + "/getConfig.json");
    if(!result.contains("error") && !result.isEmpty()){
        agenteConfigJson = result["agentcomputer"].toObject();
        listaModulos = agenteConfigJson["modulos"].toObject()["cacic"].toArray();
        QVariantMap modulosExecutados;
        for (int var = 0; var < listaModulos.size(); var++) {
            QString nome = listaModulos.at(var).toObject().value("nome").toString();
            QString hash = listaModulos.at(var).toObject().value("hash").toString();
            definirDirModulo(getApplicationDirPath(), nome);
            if(nome != "install-cacic" && nome != "cacic-service"){
//                if(!verificarseModuloJaFoiExecutado(nome,hash)){
                    if (QFile::exists(getDirProgram())){
                        cacicthread->setCcacic(ccacic);
                        cacicthread->setOCacicComm(OCacicComm);
                        cacicthread->setNomeModulo(nome);
                        cacicthread->setCMutex(cMutex);
                        cacicthread->setModuloDirPath(getDirProgram());
                        cacicthread->start(QThread::NormalPriority);
                        modulosExecutados[nome] = hash;
                        ccacic->setValueToRegistry("Lightbase", "Cacic", modulosExecutados);
                    }else{
                        QLogger::QLog_Info(Identificadores::LOG_DAEMON_TIMER, "Modulo \""+ nome + "\" não foi encontrado para execução.");
                    }
//                }else{
//                    QLogger::QLog_Info(Identificadores::LOG_DAEMON_TIMER, "O ("+ nome +") com o hash ("+ hash +") já foi executado antes.");
//                }
            }
        }
    }
}

QString CacicTimer::getApplicationDirPath() {
    return applicationDirPath;
}

bool CacicTimer::comunicarGerente(){
    bool ok;
    OCacic_Computer.coletaDados();
    //Sempre recuperar as informações aqui caso mude.
    OCacicComm->setUrlGerente(ccacic->getValueFromRegistry("Lightbase", "Cacic", "applicationUrl").toString());
    QLogger::QLog_Info(Identificadores::LOG_DAEMON_TIMER, "Realizando comunicação em: " + OCacicComm->getUrlGerente());
    OCacicComm->setUsuario(ccacic->getValueFromRegistry("Lightbase", "Cacic", "usuario").toString());
    OCacicComm->setPassword(ccacic->getValueFromRegistry("Lightbase", "Cacic", "password").toString());
    ccacic->setChaveCrypt(ccacic->getValueFromRegistry("Lightbase", "Cacic", "key").toString());

    QJsonObject resposta = OCacicComm->login(&ok);
    if(resposta.isEmpty() || resposta.contains("error")){
        //de vez enquando a conexão da erro, é bom tentar 2 vezes pra garantir.
        resposta = OCacicComm->login(&ok);
        if(resposta.isEmpty() || resposta.contains("error")){
            QLogger::QLog_Error(Identificadores::LOG_DAEMON_TIMER, "Erro no login: " + resposta["error"].toString());
            return false;
        }
    }

    QLogger::QLog_Info(Identificadores::LOG_DAEMON_TIMER, "getLogin() success.");
    resposta = getTest();
    if(!resposta.contains("error")){
        QLogger::QLog_Info(Identificadores::LOG_DAEMON_TIMER, QString("getTeste() success."));
        resposta = getConfig();
        if(!resposta.contains("error")){
            QLogger::QLog_Info(Identificadores::LOG_DAEMON_TIMER, QString("getConfig() success."));
            return true;
        } else{
            QLogger::QLog_Error(Identificadores::LOG_DAEMON_TIMER, " Erro ao pegar informações do gerente: " + resposta["error"].toString());
            return false;
        }
    }else{
        return false;
    }
}

QJsonObject CacicTimer::getTest(){
    bool ok;
    QJsonObject as;
    as["computador"] = OCacic_Computer.toJsonObject();
    QJsonObject jsonresult = OCacicComm->comm(Identificadores::ROTA_GETTEST, &ok, as, true);
    if(!ok){
        jsonresult = OCacicComm->comm(Identificadores::ROTA_GETTEST, &ok, as, true); // mais uma vez pra garantir.
    }
    if(jsonresult.contains("error")){
        QLogger::QLog_Error(Identificadores::LOG_DAEMON_TIMER, "Falha na execução do getTest(). " + jsonresult["error"].toString());
        return jsonresult;
    }
    try{
        ccacic->setJsonToFile(jsonresult.contains("reply") ? jsonresult["reply"].toObject() : jsonresult,
                              ccacic->getCacicMainFolder() + "/getTest.json");
        return jsonresult;
    } catch (...) {
        QLogger::QLog_Error(Identificadores::LOG_DAEMON_TIMER,"Erro ao salvar o arquivo de configurações.");
        return jsonresult;
    }
}

QJsonObject CacicTimer::getConfig(){
    bool ok;
    QJsonObject as;
    as["computador"] = OCacic_Computer.toJsonObject();
    QJsonObject jsonresult = OCacicComm->comm(Identificadores::ROTA_GETCONFIG, &ok, as, true);

    if(jsonresult.contains("error")){
        QLogger::QLog_Error(Identificadores::LOG_DAEMON_TIMER, "Falha na execução do getConfig()." + jsonresult["error"].toString());
        return jsonresult;
    }
    try{
        ccacic->setJsonToFile(jsonresult.contains("reply") ? jsonresult["reply"].toObject() : jsonresult,
                              ccacic->getCacicMainFolder() + "/getConfig.json");
        if (!jsonresult["reply"].toObject()["applicationUrl"].toString().isEmpty()){
            QVariantMap registro;
            registro["applicationUrl"] = jsonresult["reply"].toObject()["applicationUrl"].toString();
            ccacic->setValueToRegistry("Lightbase", "Cacic", registro);
        }
        return jsonresult;
    } catch (...) {
        QLogger::QLog_Error(Identificadores::LOG_DAEMON_TIMER,"Erro ao salvar o arquivo de configurações.");
        return jsonresult;
    }
}

QString CacicTimer::getDirProgram() const
{
    return dirProgram;
}

void CacicTimer::setDirProgram(const QString &value)
{
    dirProgram = value;
}

void CacicTimer::setApplicationDirPath(const QString &value)
{
    this->applicationDirPath = value;
}

void CacicTimer::iniciarInstancias(){

    ccacic = new CCacic();
    ccacic->setCacicMainFolder(this->applicationDirPath);
    timer = new QTimer(this);
    cMutex = new QMutex(QMutex::Recursive);
    cacicthread = new CacicThread(this->applicationDirPath);
    OCacicComm = new CacicComm();
    OCacicComm->setUrlGerente(ccacic->getValueFromRegistry("Lightbase", "Cacic", "applicationUrl").toString());
    OCacicComm->setUsuario(ccacic->getValueFromRegistry("Lightbase", "Cacic", "usuario").toString());
    OCacicComm->setPassword(ccacic->getValueFromRegistry("Lightbase", "Cacic", "password").toString());
    ccacic->setChaveCrypt(ccacic->getValueFromRegistry("Lightbase", "Cacic", "key").toString());
    checkModules = new CheckModules(this->applicationDirPath, Identificadores::LOG_DAEMON_TIMER);
}

bool CacicTimer::verificarPeriodicidade()
{
    QJsonObject agenteConfigJson;
    QJsonObject configuracoes;
    QJsonObject result = ccacic->getJsonFromFile(this->applicationDirPath + "/getConfig.json");
    if(!result.contains("error") && !result.isEmpty()){
        agenteConfigJson = result["agentcomputer"].toObject();
        configuracoes = agenteConfigJson["configuracoes"].toObject();
        if(!configuracoes["nu_intervalo_exec"].isNull() &&
                getPeriodicidadeExecucao() != configuracoes["nu_intervalo_exec"].toString().toInt()){
            setPeriodicidadeExecucao(configuracoes["nu_intervalo_exec"].toString().toInt() * 60000);
            return true;
        } else {
            setPeriodicidadeExecucao(this->periodicidadeExecucaoPadrao * 60000);
            QLogger::QLog_Error(Identificadores::LOG_DAEMON_TIMER, QString("valor do timer com erro ou vazio"));
            return false;
        }
    }else{
        setPeriodicidadeExecucao(this->periodicidadeExecucaoPadrao * 60000);
        QLogger::QLog_Error(Identificadores::LOG_DAEMON_TIMER, QString("getConfig.json com erro ou vazio"));
        return false;
    }
}


bool CacicTimer::removeArquivosEstrangeiros(const QDir &diretorio)
{
    bool retorno = true;
    //    QDir dir(ccacic->getCacicMainFolder());
    QDir dir = diretorio;
    dir.setFilter(QDir::AllEntries | QDir::Hidden | QDir::NoDotAndDotDot );
    dir.setSorting(QDir::Size | QDir::Reversed);

    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i<list.size(); i++){

        // Lista arquivos a não serem excluídos
#if defined(Q_OS_WIN)
        if( !list.at(i).fileName().contains("cacic-service.exe") &&
        #else
        if( !list.at(i).fileName().contains("cacic-service") &&
        #endif
                !list.at(i).fileName().contains("cacic.log") &&
                !list.at(i).fileName().contains("install-cacic") &&
                !list.at(i).fileName().contains("gercols") &&
                !list.at(i).fileName().contains("getTest.json") &&
                !list.at(i).fileName().contains("getConfig.json") ) {

            if ( list.at(i).isDir()) {

                // Lista diretorios a não serem excluidos
                if( list.at(i).absoluteFilePath() == "/usr/share/cacic/Logs" ||
                    list.at(i).absoluteFilePath() == "/usr/share/cacic/temp" ) {

                    if( removeArquivosEstrangeiros(QDir(list.at(i).absoluteFilePath())) )
                        retorno = true;
                    else
                        retorno = false;
                } else {
                    retorno = ccacic->deleteFolder(list.at(i).absoluteFilePath());
                    QLogger::QLog_Info(Identificadores::LOG_DAEMON_TIMER, "Excluindo diretorio: " + list.at(i).fileName());
                }
            } else {
                retorno = ccacic->deleteFile(list.at(i).absoluteFilePath());
                QLogger::QLog_Info(Identificadores::LOG_DAEMON_TIMER, "Excluindo arquivo: " + list.at(i).fileName());
            }

        }
    }

    return retorno;
}

void CacicTimer::definirDirModulo(QString appDirPath, QString nome){
#if defined (Q_OS_WIN)
    setDirProgram(appDirPath + "\\" + nome + ".exe");
#elif defined (Q_OS_LINUX)
    setDirProgram(appDirPath + "/"+ nome);
#endif
}

int CacicTimer::getPeriodicidadeExecucao() const
{
    return periodicidadeExecucao;
}

void CacicTimer::setPeriodicidadeExecucao(int value)
{
    periodicidadeExecucao = value;
}
