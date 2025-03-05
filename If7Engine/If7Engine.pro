QT += core
QT += gui
QT += widgets
QT += xml
CONFIG += c++17
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
#DEFINES += ENABLE_AVGFACE
DEFINES += TODO0002
DEFINES += BUILD_OBJDET_EVAL

#include(useOCV4.pri)
#message(TODO: Specify DESTDIR)
include(DESTDIR.pri)

SOURCES += \
    AbstractIdString.cpp \
    AnyColor.cpp \
#    AnyColorMatrix.cpp \
#    AttributeSet.cpp \
#    AverageFace.cpp \
    BaseIdBehavior.cpp \
    BaseObjdetEngine.cpp \
    BaseObjdetMarker.cpp \
    BasicId.cpp \
    CharacteristicColor.cpp \
    ClothesMatchProperties.cpp \
    ClothesMatcher.cpp \
#    ColorModel.cpp \
#    CsvWriter.cpp \
    DetectorResult.cpp \
    DetectorResultList.cpp \
    DomValuesElement.cpp \
#    EigenFace.cpp \
    EigenFaceBitMask.cpp \
    EigenFaceData.cpp \
#    EigenFaceFace.cpp \
    EigenFaceGenerator.cpp \
    EigenFaceGenerator2.cpp \
    EigenFaceMaskedArray-IO.cpp \
    EigenFaceMaskedArray-Image.cpp \
    EigenFaceMaskedArray.cpp \
    EigenFaceParameters.cpp \
#    EigenFaceSearchPerson.cpp \
#    EigenFaceSearchResult.cpp \
#    EigenFaceSearchResultList.cpp \
#    EigenFaceSearchSettings.cpp \
#    EigenFaceSearchTier.cpp \
#    EigenFaceSearcher.cpp \
#    EigenFaceSimilarity.cpp \
#    EigenFaceSimilarityEntry.cpp \
#    EigenFaceSimilarityResult.cpp \
#    EigenFaceSimilarityResultList.cpp \
#    EigenFaceStatus.cpp \
    EigenFaceTemplate.cpp \
    EigenFaceVector.cpp \
#    Enumeration.cpp \
#    EyeDetector.cpp \
#    EyesMarker.cpp \
#    FaceBase-Access.cpp \
#    FaceBase-File.cpp \
#    FaceBase.cpp \
    EyesMarker.cpp \
    FeatureInfo.cpp \
    FileWriteProfile.cpp \
    FileWriter.cpp \
    FrameObjdet.cpp \
#    FramePerformance.cpp \
#    FrameStatistics.cpp \
#    FrontalFaceDetector.cpp \
    FrontalMarker.cpp \
    FrontalObjdet.cpp \
#    GreyImage.cpp \
#    GroupObjects.cpp \
#    HaarDetector.cpp \
    HeightGrid.cpp \
    IdGenerator.cpp \
#    IdString.cpp \
#    IfSearch-AvgFace.cpp \
#    IfSearch-ProcessFace.cpp \
#    IfSearch-ProcessGrab.cpp \
#    IfSearchAuthenticate.cpp \
#    IfSearchEnroll.cpp \
#    IfSearchEnroll2.cpp \
    IfSearchApplication.cpp \
    IfSearchEngine-Init.cpp \
    IfSearchEngine-Slots.cpp \
    IfSearchEngine.cpp \
#    IfSearchRetrieve.cpp \
#    IfSearchSearch.cpp \
#    IfSearchSimilarity.cpp \
    IfSearchWindow.cpp \
    ImageCache.cpp \
    ImageCacheCleaner.cpp \
    ImageCacheEntry.cpp \
    ImageCacheWriter.cpp \
    ImageInfo.cpp \
    ImageMarker.cpp \
    ImageSource.cpp \
#    InputHotdir.cpp \
#    Key.cpp \
    LogOutput.cpp \
    MillisecondDelta.cpp \
    MillisecondTime.cpp \
    Objdet.cpp \
    ObjdetCatalog.cpp \
    ObjdetCatalogItem.cpp \
    ObjdetFrontal.cpp \
    ObjdetParameters.cpp \
    ObjdetParametersEyes.cpp \
    ObjdetParametersFrontal.cpp \
    ObjdetRawArguments.cpp \
    ObjdetResource.cpp \
    ObjdetResult.cpp \
    ObjdetResultMap.cpp \
    ObjectHelper.cpp \
#    QQRect.cpp \
#    Resolver.cpp \
    Return.cpp \
    Setting.cpp \
    SettingProperty.cpp \
    Settings.cpp \
    SettingsScanner.cpp \
#    SkinDetector.cpp \
#    SkinMatchProperties.cpp \
#    SkinMatcher.cpp \
#    TransformProperties.cpp \
    Variable.cpp \
    VariableGroup.cpp \
    VariableId.cpp \
    VariableIdBehavior.cpp \
    VariableIdList.cpp \
    VariableSet.cpp \
    VariableType.cpp \
    VersionInfo.cpp \
#    WindowsProcess.cpp \
#    ddtcore.cpp \
#    fbPerson.cpp \
#    fbValue.cpp \
    main.cpp \

DISTFILES += \
    ../../../bin/detectors/detectors/Aim8A001-32-NoSplit.xml \
    ../../ottoZcode/doc/desktop/INDIface/Settings2.txt \
    ../../ottoZcode/doc/desktop/INDIface/detect2.txt \
    FrameSequencer.graphml \
    GradIcon.ico \
    INDIface24.ico \
    LIBDIR.pri \
    QualityScore.md \
    eIRonly24.ico \
    version.pri

HEADERS += \
    AbstractIdString.h \
    AnyColor.h \
    AnyColorMatrix.h \
    AttributeSet.h \
    AverageFace.h \
    BaseIdBehavior.h \
    BaseObjdetEngine.h \
    BaseObjdetMarker.h \
    BasicId.h \
    CharacteristicColor.h \
    ClothesMatchProperties.h \
    ClothesMatcher.h \
    ColorModel.h \
    ColorRangeWeightVector.inc \
    CsvWriter.h \
    DDTcore.h \
    Detector.hhh \
    DetectorResult.h \
    DetectorResultList.h \
    DomValuesElement.h \
    DualMap.h \
    EigenFace.h \
    EigenFaceBitMask.h \
    EigenFaceData.h \
    EigenFaceFace.h \
    EigenFaceGenerator.h \
    EigenFaceMaskedArray.h \
    EigenFaceParameters.h \
    EigenFaceSearchPerson.h \
    EigenFaceSearchResult.h \
    EigenFaceSearchResultList.h \
    EigenFaceSearchSettings.h \
    EigenFaceSearchTier.h \
    EigenFaceSearcher.h \
    EigenFaceSimilarity.h \
    EigenFaceSimilarityEntry.h \
    EigenFaceSimilarityResult.h \
    EigenFaceSimilarityResultList.h \
    EigenFaceStatus.h \
    EigenFaceTemplate.h \
    EigenFaceVector.h \
    Enumeration.h \
    EyesMarker.h \
    FaceBase.h \
    FeatureInfo.h \
    FeatureInfoData.h \
    FileWriteProfile.h \
    FileWriter.h \
    FrameObjdet.h \
    FramePerformance.h \
    FrameStatistics.h \
    FrontalMarker.h \
    FrontalObjdet.h \
    GreyImage.h \
    HeightGrid.h \
    Histogram.h \
    INDIef.h \
    INDIffd.h \
    IdGenerator.h \
    IdString.h \
    IfSearchApplication.h \
    IfSearchEngine.h \
    IfSearchWindow.h \
    ImageCache.h \
    ImageInfo.h \
    ImageMarker.h \
    ImageSource.h \
    InputHotdir.h \
    Key.h \
    Key.hh \
    LogOutput.h \
    Matrix.h \
    MillisecondDelta.h \
    MillisecondTime.h \
    Named.h \
    NamedArray.h \
    NamedEnum.h \
    NamedEnum2.h \
    Objdet.h \
    ObjdetCatalog.h \
    ObjdetCatalogItem.h \
    ObjdetFrontal.h \
    ObjdetParameters.h \
    ObjdetParametersEyes.h \
    ObjdetParametersFrontal.h \
    ObjdetRawArguments.h \
    ObjdetResource.h \
    ObjdetResult.h \
    ObjdetResultMap.h \
    ObjectHelper.h \
    Property.h \
    QProperty.h \
    QQRect.h \
    Rectangle.h \
    Rectangle.hh \
    Resolver.h \
    Return.h \
    Setting.h \
    SettingProperty.h \
    Settings.h \
    SettingsScanner.h \
    SkinColorTables.inc \
    SkinDetector.h \
    SkinMatchProperties.h \
    SkinMatcher.h \
    TransformProperties.h \
    Variable.h \
    VariableGroup.h \
    VariableId.h \
    VariableIdBehavior.h \
    VariableIdList.h \
    VariableSet.h \
    VariableType.h \
    VersionInfo.h \
    WindowsProcess.h \
    fbPerson.h \
    fbValue.h \
    iImageCache.h \
    version.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    If7Engine.qrc


win32: LIBS += -L$$PWD/../../../../Lang/OpenCV-MinGW-Build-OpenCV-4.5.5-x64/x64/mingw/lib/ -llibopencv_objdetect455.dll

INCLUDEPATH += $$PWD/../../../../Lang/OpenCV-MinGW-Build-OpenCV-4.5.5-x64/include
DEPENDPATH += $$PWD/../../../../Lang/OpenCV-MinGW-Build-OpenCV-4.5.5-x64/include

win32: LIBS += -L$$PWD/../../../../Lang/OpenCV-MinGW-Build-OpenCV-4.5.5-x64/x64/mingw/lib/ -llibopencv_core455.dll

INCLUDEPATH += $$PWD/../../../../Lang/OpenCV-MinGW-Build-OpenCV-4.5.5-x64/include
DEPENDPATH += $$PWD/../../../../Lang/OpenCV-MinGW-Build-OpenCV-4.5.5-x64/include
