#-------------------------------------------------
#
# Project created by QtCreator 2019-07-07T11:22:30
#
#-------------------------------------------------

QT       += core gui network sql
# need command line iwgetid

# Insert data picture to SQLiteDatabase

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = kensagaikan
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

LIBS += -ludev -lcurl

SOURCES += \
    cAB1CodeParserUtils.cpp \
    cAB2CodeParserUtils.cpp \
        cAutoSync.cpp \
        cCancelButton.cpp \
        cClockWidget.cpp \
        cConfigureUtils.cpp \
        cCustomLineEdit.cpp \
        cDataSession.cpp \
        cKanbanParserUtils.cpp \
        cNumberPad.cpp \
        cRowErrorWidget.cpp \
        cScannerUtils.cpp \
        cServerUtils.cpp \
        cSettingButton.cpp \
        cSqliteDatabase.cpp \
        cStaffIDParserUtils.cpp \
        cTitleBarWidget.cpp \
        cWifiWidget.cpp \
        cdatamh.cpp \
        libse660/src/se660.c \
        libse660/src/se660_detector_helper.c \
        main.cpp \
        mainwindow.cpp \
        wErrorTable.cpp \
        wScanKanbanCode.cpp \
        wScanStaffID.cpp \
        wSelectErrorTable.cpp \
        cSyncingWidget.cpp \
        wCheckingKanban.cpp \
        cSettingDialog.cpp \
    cFreeMemWidget.cpp \
    cNumberPad2.cpp \
    cDataSessionActivating.cpp \
    cCameraWorker.cpp \
    wCameraStream.cpp \
    cCameraViewer.cpp \
    cPicturesData.cpp \
    cPicturesUtils.cpp \
    cConfirmYesButton.cpp \
    cConfirmNoButton.cpp \
    cMessageBox.cpp \
    cPasswordDialog.cpp \
    cSaveButton.cpp \
    cTrashButton.cpp \
    cCameraControllerWidg.cpp \
    wSelectLine.cpp

HEADERS += \
    cAB1CodeParserUtils.h \
    cAB2CodeParserUtils.h \
        cAutoSync.h \
        cCancelButton.h \
        cClockWidget.h \
        cConfigureUtils.h \
        cCustomLineEdit.h \
        cDataSession.h \
        cKanbanParserUtils.h \
        cNumberPad.h \
        cRowErrorWidget.h \
        cScannerUtils.h \
        cServerUtils.h \
        cSettingButton.h \
        cSqliteDatabase.h \
        cStaffIDParserUtils.h \
        cTitleBarWidget.h \
        cWifiWidget.h \
    cdatamh.h \
        libse660/inc/se660.h \
        libse660/inc/se660_detector_helper.h \
        mainwindow.h \
        wErrorTable.h \
        wScanKanbanCode.h \
        wScanStaffID.h \
        wSelectErrorTable.h \
        cSyncingWidget.h \
        wCheckingKanban.h \
        cSettingDialog.h \
    cFreeMemWidget.h \
    cNumberPad2.h \
    cDataSessionActivating.h \
    cCameraWorker.h \
    wCameraStream.h \
    cCameraViewer.h \
    cPicturesData.h \
    cPicturesUtils.h \
    cConfirmYesButton.h \
    cConfirmNoButton.h \
    cMessageBox.h \
    cPasswordDialog.h \
    cSaveButton.h \
    cTrashButton.h \
    cCameraControllerWidg.h \
    wSelectLine.h

FORMS += \
        mainwindow.ui \
        wErrorTable.ui \
        wScanKanbanCode.ui \
        wScanStaffid.ui \
        wSelectErrorTable.ui \
    wCheckingKanban.ui \
    cSettingDialog.ui \
    wCameraStream.ui \
    cMessageBox.ui \
    cPasswordDialog.ui \
    wSelectLine.ui

CONFIG(raspberry) {
    message(Building with Raspberry support.)
    LIBS += -lopencv_core        \
            -lopencv_highgui     \
            -lopencv_imgproc     \
            -lopencv_features2d  \
            -lopencv_calib3d    \
            -lopencv_videoio \
} else {
    message(Build for PC.)
    LIBS += -lopencv_core        \
            -lopencv_highgui     \
            -lopencv_imgcodecs   \
            -lopencv_imgproc     \
            -lopencv_features2d  \
            -lopencv_calib3d    \
            -lopencv_videoio
    DEFINES += "BUILD_PC"
}

# Tren Raspberry remove -lopencv_imgproc -lopencv_videoio
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
