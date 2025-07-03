QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# Путь до папки с opencv2 (там лежит opencv2/opencv.hpp)
INCLUDEPATH += D:\openCV\opencv\build\include

# — для Release: opencv_world4110.lib
# — для Debug:   opencv_world4110d.lib
win32:CONFIG(release, debug|release): LIBS += -LD:\openCV\opencv\build\x64\vc16\lib -lopencv_world4110
win32:CONFIG(debug, debug|release):   LIBS += -LD:\openCV\opencv\build\x64\vc16\lib -lopencv_world4110d

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    imageanalyzer.cpp


HEADERS += \
    mainwindow.h \
    imageanalyzer.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    resources.qrc

qnx:     target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    images/cat1.jpg  \
    images/cat2.jpg  \
    images/cat3.jpg  \
    images/cat4.jpg  \
    images/cat5.jpg  \
    images/cat6.jpg  \
    images/cat7.jpg  \
    images/cat8.jpg  \
    images/cat9.jpg  \
    images/cat10.jpg \
    images/cat11.jpg \
    images/cat12.jpg \
    images/cat13.jpg \
    images/cat14.jpg \
    images/cat15.jpg \
    images/cat16.jpg \
    images/cat17.jpg \
    images/cat18.jpg \
    images/cat19.jpg \
    images/cat20.jpg \
    images/cat21.jpg \
    images/cat22.jpg \
    images/cat23.jpg \
    images/cat24.jpg \
    images/cat25.jpg
