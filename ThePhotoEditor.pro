#-------------------------------------------------
#
# Project created by QtCreator 2019-11-08T14:05:07
#
#-------------------------------------------------

QT       += core gui printsupport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ThePhotoEditor
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

CONFIG += c++14

SOURCES += \
        AboutUs.cpp \
        FilterTransform/AbstractImageFilterTransform.cpp \
        FilterTransform/AbstractKernelBasedImageFilterTransform.cpp \
        FilterTransform/AbstractNonKernelBasedImageFilterTransform.cpp \
        FilterTransform/KernelBased/EdgeDetectionFilter.cpp \
        FilterTransform/KernelBased/EmbossFilter.cpp \
        FilterTransform/KernelBased/GaussianBlurFilter.cpp \
        FilterTransform/KernelBased/MeanBlurFilter.cpp \
        FilterTransform/NonKernelBased/BrightnessFilter.cpp \
        FilterTransform/NonKernelBased/ClockwiseRotationTransform.cpp \
        FilterTransform/NonKernelBased/ContrastFilter.cpp \
        FilterTransform/NonKernelBased/CounterClockwiseRotationTransform.cpp \
        FilterTransform/NonKernelBased/ExposureFilter.cpp \
        FilterTransform/NonKernelBased/FlipHorizontalTransform.cpp \
        FilterTransform/NonKernelBased/FlipVerticalTransform.cpp \
        FilterTransform/NonKernelBased/GrayscaleFilter.cpp \
        FilterTransform/NonKernelBased/HueFilter.cpp \
        FilterTransform/NonKernelBased/InvertFilter.cpp \
        FilterTransform/NonKernelBased/MagicWand.cpp \
        FilterTransform/NonKernelBased/SaturationFilter.cpp \
        FilterTransform/NonKernelBased/TemperatureFilter.cpp \
        FilterTransform/NonKernelBased/TintFilter.cpp \
        Palette/BasicControls.cpp \
        Palette/Brush.cpp \
        Palette/ColorControls.cpp \
        Palette/Effects.cpp \
        Palette/Histogram.cpp \
        Utilities/PixelHelper.cpp \
        Utilities/VersionControl.cpp \
        Utilities/WindowHelper.cpp \
        WorkspaceArea.cpp \
        main.cpp \
        MainWindow.cpp \
        qcustomplot.cpp \
        serverroom.cpp

HEADERS += \
        AboutUs.h \
        FilterTransform/AbstractImageFilterTransform.h \
        FilterTransform/AbstractKernelBasedImageFilterTransform.h \
        FilterTransform/AbstractNonKernelBasedImageFilterTransform.h \
        FilterTransform/KernelBased/EdgeDetectionFilter.h \
        FilterTransform/KernelBased/EmbossFilter.h \
        FilterTransform/KernelBased/GaussianBlurFilter.h \
        FilterTransform/KernelBased/MeanBlurFilter.h \
        FilterTransform/NonKernelBased/BrightnessFilter.h \
        FilterTransform/NonKernelBased/ClockwiseRotationTransform.h \
        FilterTransform/NonKernelBased/ContrastFilter.h \
        FilterTransform/NonKernelBased/CounterClockwiseRotationTransform.h \
        FilterTransform/NonKernelBased/ExposureFilter.h \
        FilterTransform/NonKernelBased/FlipHorizontalTransform.h \
        FilterTransform/NonKernelBased/FlipVerticalTransform.h \
        FilterTransform/NonKernelBased/GrayscaleFilter.h \
        FilterTransform/NonKernelBased/HueFilter.h \
        FilterTransform/NonKernelBased/InvertFilter.h \
        FilterTransform/NonKernelBased/MagicWand.h \
        FilterTransform/NonKernelBased/SaturationFilter.h \
        FilterTransform/NonKernelBased/TemperatureFilter.h \
        FilterTransform/NonKernelBased/TintFilter.h \
        MainWindow.h \
        Palette/BasicControls.h \
        Palette/Brush.h \
        Palette/ColorControls.h \
        Palette/Effects.h \
        Palette/Histogram.h \
        Utilities/PixelHelper.h \
        Utilities/VersionControl.h \
        Server/Client.h \
        Server/Server.h \
        Server/ServerWorker.h \
        Utilities/WindowHelper.h \
        WorkspaceArea.h \
        qcustomplot.h \
        serverroom.h

FORMS += \
        AboutUs.ui \
        MainWindow.ui \
        Palette/BasicControls.ui \
        Palette/Brush.ui \
        Palette/ColorControls.ui \
        Palette/Effects.ui \
        Palette/Histogram.ui \
        serverroom.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc

DISTFILES += \

