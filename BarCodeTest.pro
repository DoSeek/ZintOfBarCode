#-------------------------------------------------
#
# Project created by QtCreator 2017-07-24T11:23:15
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BarCodeTest
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        barcodemainwnd.cpp \

HEADERS  += barcodemainwnd.h \
    zint.h \


FORMS    += barcodemainwnd.ui

RESOURCES += \

INCLUDEPATH += ./Zint/include \


#make 或 make debug     -------生成debug版
#make release           -------生成release版
#make all               -------两个版本都生成
#屏蔽release版本中所有QDebug()提示信息，可以在pro文件中添加DEFINES+=QT_NO_DEBUG_OUTPUT
CONFIG(debug, debug|release) {
    #LIBS += -L./Zint/bin/Win32/Debug -lzintd
    LIBS += -LD:/Qt/WorkSpace/BarCode/BarCodeTest/Zint/bin/Win32/Debug -lzintd
}
else {
    LIBS += -L./Zint/bin/Win32/Release -lzint
}
