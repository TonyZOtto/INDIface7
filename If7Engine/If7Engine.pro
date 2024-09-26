QT += core
QT += gui
QT += gui
QT += xml
CONFIG += c++17
CONFIG += cmdline
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
DEFINES += TODO0002

SOURCES += \
    AnyColor.cpp \
    AnyColorMatrix.cpp \
    AttributeSet.cpp \
    AverageFace.cpp \
    CharacteristicColor.cpp \
    ClothesMatchProperties.cpp \
    ClothesMatcher.cpp \
    ColorModel.cpp \
    DetectorResult.cpp \
    DomValuesElement.cpp \
    EigenFace.cpp \
    EigenFaceBitMask.cpp \
    EigenFaceData.cpp \
    EigenFaceFace.cpp \
    EigenFaceGenerator.cpp \
    EigenFaceGenerator2.cpp \
    EigenFaceMaskedArray-IO.cpp \
    EigenFaceMaskedArray-Image.cpp \
    EigenFaceMaskedArray.cpp \
    EigenFaceParameters.cpp \
    EigenFaceSearchPerson.cpp \
    EigenFaceSearchResult.cpp \
    EigenFaceSearchResultList.cpp \
    EigenFaceSearchSettings.cpp \
    EigenFaceSearchTier.cpp \
    EigenFaceSearcher.cpp \
    EigenFaceSimilarity.cpp \
    EigenFaceSimilarityEntry.cpp \
    EigenFaceSimilarityResult.cpp \
    EigenFaceSimilarityResultList.cpp \
    EigenFaceStatus.cpp \
    EigenFaceTemplate.cpp \
    EigenFaceVector.cpp \
    Enumeration.cpp \
    EyeDetector.cpp \
    FaceBase-Access.cpp \
    FaceBase-File.cpp \
    FaceBase.cpp \
    FeatureInfo.cpp \
    FeatureSet.cpp \
    FileWriteProfile.cpp \
    FileWriter.cpp \
    FrontalFaceDetector.cpp \
    GreyImage.cpp \
    GroupObjects.cpp \
    HaarDetector.cpp \
    HeightGrid.cpp \
    IdString.cpp \
    IfSearch-AvgFace.cpp \
    IfSearch-ProcessFace.cpp \
    IfSearch-ProcessGrab.cpp \
    IfSearch.cpp \
    IfSearchAuthenticate.cpp \
    IfSearchEnroll.cpp \
    IfSearchEnroll2.cpp \
    IfSearchInit.cpp \
    IfSearchRetrieve.cpp \
    IfSearchSearch.cpp \
    IfSearchSimilarity.cpp \
    IfSearchSlots.cpp \
    ImageCache.cpp \
    ImageCacheCleaner.cpp \
    ImageCacheEntry.cpp \
    ImageCacheWriter.cpp \
    ImageInfo.cpp \
    ImageMarker.cpp \
    ImageSource.cpp \
    Info.cpp \
    InfoFunction.cpp \
    InfoItem.cpp \
    InfoOutputFile.cpp \
    InfoOutputSetting.cpp \
    InfoOutputs.cpp \
    InfoThread.cpp \
    InputHotdir.cpp \
    Key.cpp \
    QQRect.cpp \
    Resolver.cpp \
    Return.cpp \
    Setting.cpp \
    SettingItem.cpp \
    SettingProperty.cpp \
    Settings.cpp \
    SettingsScanner.cpp \
    SkinDetector.cpp \
    SkinMatchProperties.cpp \
    SkinMatcher.cpp \
    TransformProperties.cpp \
    VersionInfo.cpp \
    WD-IfSearch.cpp \
    WD-IfSearchInit.cpp \
    WD-IfSearchSlots.cpp \
    WindowsProcess.cpp \
    ddtcore.cpp \
    fbPerson.cpp \
    fbValue.cpp \
    main.cpp \

DISTFILES += \
    GradIcon.ico \
    INDIface24.ico \
    eIRonly24.ico

HEADERS += \
    AnyColor.h \
    AnyColorMatrix.h \
    AttributeSet.h \
    AverageFace.h \
    CharacteristicColor.h \
    ClothesMatchProperties.h \
    ClothesMatcher.h \
    ColorModel.h \
    ColorRangeWeightVector.inc \
    DDTcore.h \
    Detector.h \
    DetectorResult.h \
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
    Eyes.h \
    Eyes.hh \
    FaceBase.h \
    FeatureInfo.h \
    FeatureInfoData.h \
    FeatureSet.h \
    FileWriteProfile.h \
    FileWriter.h \
    GreyImage.h \
    HeightGrid.h \
    Histogram.h \
    INDIef.h \
    INDIffd.h \
    IdString.h \
    IfSearch.h \
    ImageCache.h \
    ImageInfo.h \
    ImageMarker.h \
    ImageSource.h \
    Info.h \
    InfoFunction.h \
    InfoItem.h \
    InfoMacros.h \
    InfoOutputBase.h \
    InfoOutputFile.h \
    InfoOutputQDebug.h \
    InfoOutputSetting.h \
    InfoOutputs.h \
    InfoThread.h \
    InputHotdir.h \
    Key.h \
    Key.hh \
    Matrix.h \
    Named.h \
    NamedArray.h \
    NamedEnum.h \
    NamedEnum2.h \
    Property.h \
    QProperty.h \
    QQRect.h \
    Rectangle.h \
    Rectangle.hh \
    Resolver.h \
    Return.h \
    Settings.h \
    SkinColorTables.inc \
    SkinDetector.h \
    SkinMatchProperties.h \
    SkinMatcher.h \
    TransformProperties.h \
    VersionInfo.h \
    WD-IfSearch.h \
    WindowsProcess.h \
    fbPerson.h \
    fbValue.h \
    iImageCache.h \
    iSettings.h \
    version.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
