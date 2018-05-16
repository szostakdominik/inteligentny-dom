QT += widgets serialport printsupport

TARGET = slave2
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    settingsdialog.cpp \
    watek_temperatura.cpp \
    watek_ruch.cpp

HEADERS += \
    mainwindow.h \
    settingsdialog.h \
    watek_temperatura.h \
    watek_ruch.h

FORMS += \
    mainwindow.ui \
    settingsdialog.ui

RESOURCES += \
    slave2.qrc

target.path = $$[QT_INSTALL_EXAMPLES]/serialport/terminal
INSTALLS += target
