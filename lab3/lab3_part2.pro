QT += core gui datavisualization widgets
CONFIG += c++17
INCLUDEPATH += C:/Users/icezg/Downloads/boost_1_83_0
LIBS += "-LC:/Users/icezg/Downloads/boost_1_83_0/stage/lib/"
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    solver.cpp

HEADERS += \
    mainwindow.h \
    solver.h

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#unix|win32: LIBS += -lgmp -lgmpxx -lmpfr
