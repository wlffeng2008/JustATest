QT       += core gui mqtt
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += console
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += /home/levi/Desktop/Work/vcpkg/packages/ffmpeg_x64-linux/include
LIBS += /home/levi/Desktop/Work/vcpkg/installed/x64-linux/lib/libavdevice.a    # 依赖 avformat、avcodec
LIBS += /home/levi/Desktop/Work/vcpkg/installed/x64-linux/lib/libavfilter.a    # 依赖 avformat、avcodec 等
LIBS += /home/levi/Desktop/Work/vcpkg/installed/x64-linux/lib/libavformat.a    # 依赖 avcodec、avutil
LIBS += /home/levi/Desktop/Work/vcpkg/installed/x64-linux/lib/libavcodec.a     # 依赖 avutil
LIBS += /home/levi/Desktop/Work/vcpkg/installed/x64-linux/lib/libswscale.a     # 依赖 avutil
LIBS += /home/levi/Desktop/Work/vcpkg/installed/x64-linux/lib/libswresample.a  # 依赖 avutil
LIBS += /home/levi/Desktop/Work/vcpkg/installed/x64-linux/lib/libavutil.a      # 最基础库，无依赖

LIBS += -lz -lpthread -lm -ldl

SOURCES += \
    LogToFile.cpp \
    custumradiobutton.cpp \
    ffmpegplayer.cpp \
    gencomport.cpp \
    main.cpp \
    mainwindow.cpp \
    DialogToast.cpp \
    ringloop.cpp

HEADERS += \
    DialogToast.h \
    LogToFile.h \
    custumradiobutton.h \
    ffmpegplayer.h \
    gencomport.h \
    mainwindow.h \
    ringloop.h

FORMS += \
    DialogToast.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
