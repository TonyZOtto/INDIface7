QT += core
QT += gui
QT += widgets
QT += xml
CONFIG += c++17
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
#DEFINES += ENABLE_AVGFACE
DEFINES += TODO0002
#DEFINES += EYEFIND
DEFINES += BUILD_OBJDET_EVAL

include(DESTDIR.pri)

SOURCES += \
    DetectorResult.cpp \
    DetectorResultList.cpp \
    IfSearchApplication.cpp \
    IfSearchEngine.cpp \
    IfSearchWindow.cpp \
    Objdet.cpp \
    ObjdetCatalog.cpp \
    ObjdetCatalogItem.cpp \
    ObjdetEyes.cpp \
    ObjdetFrontal.cpp \
    ObjdetParameters.cpp \
    ObjdetParametersEyes.cpp \
    ObjdetParametersFrontal.cpp \
    ObjdetRawArguments.cpp \
    ObjdetResource.cpp \
    ObjdetResult.cpp \
    ObjdetResultMap.cpp \
    SCRect.cpp \
    VersionInfo.cpp \
    main.cpp \

DISTFILES += \
    ../../../bin/DbgWin64-Qt6CV4-v7.91A/detectors/Aim8A001-32-NoSplit.xml \
    ../../../bin/DbgWin64-Qt6CV4-v7.91A/detectors/Detectors.xml \
    ../../../bin/DbgWin64-Qt6CV4-v7.91A/detectors/haarcascade_eye.xml \
    ../../../bin/detectors/detectors/Aim8A001-32-NoSplit.xml \
    ../../ottoZcode/doc/desktop/INDIface/Settings2.txt \
    ../../ottoZcode/doc/desktop/INDIface/detect2.txt \
    FrameSequencer.graphml \
    GradIcon.ico \
    INDIface24.ico \
    Installing.md \
    LIBDIR.pri \
    QualityScore.md \
    eIRonly24.ico \
    version.pri

HEADERS += \
    DetectorResult.h \
    DetectorResultList.h \
    IfSearchApplication.h \
    IfSearchEngine.h \
    IfSearchWindow.h \
    Objdet.h \
    ObjdetCatalog.h \
    ObjdetCatalogItem.h \
    ObjdetEyes.h \
    ObjdetFrontal.h \
    ObjdetParameters.h \
    ObjdetParametersEyes.h \
    ObjdetParametersFrontal.h \
    ObjdetRawArguments.h \
    ObjdetResource.h \
    ObjdetResult.h \
    ObjdetResultMap.h \
    SCRect.h \
    VersionInfo.h \
    version.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    If7Engine.qrc



win32: LIBS += -L$$PWD/../../../../Lang/OpenCV-MinGW-Build-OpenCV-4.5.5-x64/x64/mingw/lib/
win32: LIBS +=  -llibopencv_core455.dll -llibopencv_objdetect455.dll
INCLUDEPATH += $$PWD/../../../../Lang/OpenCV-MinGW-Build-OpenCV-4.5.5-x64/include
DEPENDPATH += $$PWD/../../../../Lang/OpenCV-MinGW-Build-OpenCV-4.5.5-x64/include
