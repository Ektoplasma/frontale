######################################################################
# Automatically generated by qmake (3.0) dim. janv. 3 13:17:01 2016
######################################################################

TEMPLATE = app
TARGET = Frontale2
INCLUDEPATH += .

# Input
HEADERS += aesbdd.h \
           bdd_tcp.h \
           ../leBeauReseau/Client/client.h \
           clientFront.h \
           engine_event.h \
           message.h \
           network_interface.h \
           noeudthor.h \
           reception.h \
           requete.h \
           rsacrypt.h \
           traitement.h \
           ../leBeauReseau/Trame/trame.h \
           Tslot.h \
           utils.h 
SOURCES += aesbdd.cpp \
           bdd_tcp.cpp \
           client.cpp \
           main.cpp \
           message.cpp \
           network_interface.cpp \
           noeudthor.cpp \
           reception.cpp \
           requete.cpp \
           rsacrypt.cpp \
           traitement.cpp \
           ../leBeauReseau/Trame/trame.cpp \
           Tslot.cpp \
           utils.cpp 
QT += network
