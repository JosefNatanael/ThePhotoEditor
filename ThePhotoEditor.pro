#-------------------------------------------------
#
# Project created by QtCreator 2019-11-08T14:05:07
#
#-------------------------------------------------

QT       += core gui printsupport

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
        FilterTransform/KernelBased/GaussianBlurFilter.cpp \
        FilterTransform/NonKernelBased/ClockwiseRotationTransform.cpp \
        FilterTransform/NonKernelBased/CounterClockwiseRotationTransform.cpp \
        FilterTransform/NonKernelBased/FlipHorizontalTransform.cpp \
        FilterTransform/NonKernelBased/FlipVerticalTransform.cpp \
        FilterTransform/NonKernelBased/GrayscaleFilter.cpp \
        FilterTransform/NonKernelBased/HueFilter.cpp \
        FilterTransform/NonKernelBased/InvertFilter.cpp \
        FilterTransform/NonKernelBased/SaturationFilter.cpp \
        Palette/BasicControls.cpp \
        Palette/Brush.cpp \
        Palette/ColorControls.cpp \
        Palette/Histogram.cpp \
        PixelHelper.cpp \
        WorkspaceArea.cpp \
        main.cpp \
        MainWindow.cpp \
        qcustomplot.cpp

HEADERS += \
        AboutUs.h \
        FilterTransform/AbstractImageFilterTransform.h \
        FilterTransform/AbstractKernelBasedImageFilterTransform.h \
        FilterTransform/AbstractNonKernelBasedImageFilterTransform.h \
        FilterTransform/KernelBased/GaussianBlurFilter.h \
        FilterTransform/NonKernelBased/ClockwiseRotationTransform.h \
        FilterTransform/NonKernelBased/CounterClockwiseRotationTransform.h \
        FilterTransform/NonKernelBased/FlipHorizontalTransform.h \
        FilterTransform/NonKernelBased/FlipVerticalTransform.h \
        FilterTransform/NonKernelBased/GrayscaleFilter.h \
        FilterTransform/NonKernelBased/HueFilter.h \
        FilterTransform/NonKernelBased/InvertFilter.h \
        FilterTransform/NonKernelBased/SaturationFilter.h \
        MainWindow.h \
        Palette/BasicControls.h \
        Palette/Brush.h \
        Palette/ColorControls.h \
        Palette/Histogram.h \
        PixelHelper.h \
        WorkspaceArea.h \
        qcustomplot.h

FORMS += \
        AboutUs.ui \
        MainWindow.ui \
        Palette/BasicControls.ui \
        Palette/Brush.ui \
        Palette/ColorControls.ui \
        Palette/Histogram.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc

DISTFILES +=
