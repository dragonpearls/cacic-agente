#-----------------------------------------------------------------------------------------
#
# Project Cacic Agente, module cacic-service, created by Lightbase (http://www.softwarepublico.gov.br/ver-comunidade?community_id=3585)
#
# Developers: Eric Menezes Noronha (eric.m.noronha@lightbase.com.br); GitHub: ericmenezes
#             Thiago Rocha         (thiago.rocha@lightbase.com.br)  ;
#
#-----------------------------------------------------------------------------------------

##############################################################################
#SEMPRE MUDAR A VERSÃO DE BUILD (o terceiro número) AO REALIZAR QUALQUER BUILD.#
VERSION_MAJOR = 3
VERSION_MINOR = 2
VERSION_BUILD = 0
DEFINES += VERSION_MAJOR=$$VERSION_MAJOR \
           VERSION_MINOR=$$VERSION_MINOR \
           VERSION_BUILD=$$VERSION_BUILD

VERSION = $${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_BUILD}
##############################################################################

QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cacic-ui
TEMPLATE = app

SOURCES += main.cpp\
        ../src/identificadores.cpp \
        ../src/logcacic.cpp \
        ../src/QLogger.cpp \
        ../src/ccacic.cpp\
        ../src/servicecontroller.cpp\
        ../src/vregistry.cpp\
        ../src/vqtconvert.cpp \
        ../src/workerthread.cpp \
    cacicsystray.cpp \
    cacicwidget.cpp \
    uiserver.cpp \
    uiserverthread.cpp \
    uiclient.cpp \
    netdevtab.cpp \
    netadapterconfigtab.cpp \
    cddrivetab.cpp \
    logicaldisktab.cpp \
    pointdevicetab.cpp \
    printertab.cpp

HEADERS  +=\
            ../src/identificadores.h \
            ../src/logcacic.h \
            ../src/QLogger.h \
            ../src/ccacic.h \
            ../src/servicecontroller.h\
            ../src/console.h \
            ../src/vregistry.h\
            ../src/vqtconvert.h \
            ../src/workerthread.h \
    cacicsystray.h \
    cacicwidget.h \
    uiserver.h \
    uiserverthread.h \
    uiclient.h \
    netdevtab.h \
    netadapterconfigtab.h \
    cddrivetab.h \
    logicaldisktab.h \
    pointdevicetab.h \
    printertab.h

FORMS    += widget.ui \
    netdevtab.ui \
    netadapterconfigtab.ui \
    cddrivetab.ui \
    logicaldisktab.ui \
    pointdevicetab.ui \
    printertab.ui

RESOURCES += \
    resource.qrc
