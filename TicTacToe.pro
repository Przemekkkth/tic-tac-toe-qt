greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    src/fontmanager.cpp \
    src/gamescene.cpp \
    src/main.cpp \
    src/tictactoe.cpp \
    src/view.cpp

HEADERS += \
    src/fontmanager.h \
    src/gamescene.h \
    src/tictactoe.h \
    src/utils.h \
    src/view.h

RESOURCES += \
    resource.qrc
