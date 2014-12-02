#include "cacicd.h"

cacicD::cacicD(int argc, char **argv) : QtService<QCoreApplication>(argc, argv, "CacicDaemon")
{
    try{
        this->createApplication(argc, argv);
        ccacic = new CCacic();
        QString folder = ccacic->getValueFromRegistry("Lightbase", "Cacic", "mainFolder").toString();
#ifdef Q_OS_WIN
        ccacic->setCacicMainFolder(!folder.isEmpty() && !folder.isNull() ? folder : "c:/cacic");
#elif defined(Q_OS_LINUX)
        ccacic->setCacicMainFolder(!folder.isEmpty() && !folder.isNull() ? folder : "/usr/share/cacic");
#endif
        Ocacictimer = new CacicTimer(ccacic->getCacicMainFolder());

        setServiceFlags(QtService::Default);

        QObject::connect(Ocacictimer, SIGNAL(finalizar()), this->application(), SLOT(quit()));

        logManager = QLogger::QLoggerManager::getInstance();
        logManager->addDestination(ccacic->getCacicMainFolder() + "/Logs/cacic.log", Identificadores::LOG_DAEMON, QLogger::InfoLevel);
        logManager->addDestination(ccacic->getCacicMainFolder() + "/Logs/cacic.log",Identificadores::LOG_DAEMON ,QLogger::ErrorLevel);
    } catch (...){
        qCritical() << "Error desconhecido no construtor.";
        QLogger::QLog_Error("Cacic Daemon", QString("Erro desconhecido no construtor."));
    }
}

cacicD::~cacicD()
{
    try{
        logManager->closeLogger();
        application()->exit();
    } catch (...){
        QLogger::QLog_Error("Cacic Daemon", QString("Erro desconhecido no destrutor."));
    }
}

void cacicD::start() {
    try{
        QLogger::QLog_Info("Cacic Daemon", QString("Servico iniciado em " + ccacic->getCacicMainFolder() + "."));
        Ocacictimer->iniciarTimer();
    }catch (...){
        QLogger::QLog_Info("Cacic Daemon", QString("Erro desconhecido ao iniciar o serviço."));
    }
}

void cacicD::pause()
{
    try{
        QLogger::QLog_Info("Cacic Daemon", QString("Serviço pausado."));
    } catch (...){
        QLogger::QLog_Error("Cacic Daemon", QString("Erro desconhecido ao pausar o serviço."));
    }
}

void cacicD::resume()
{
    try{
        QLogger::QLog_Info("Cacic Daemon", QString("Serviço resumido."));
    } catch (...){
        QLogger::QLog_Error("Cacic Daemon", QString("Erro desconhecido ao resumir o serviço."));
    }
}

void cacicD::stop()
{
    try{
        QLogger::QLog_Info("Cacic Daemon", QString("Serviço parado."));
        this->application()->quit();
    } catch (...){
        QLogger::QLog_Error("Cacic Daemon", QString("Erro desconhecido ao parar o serviço."));
    }
}

