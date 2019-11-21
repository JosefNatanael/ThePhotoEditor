#include "ColorControls.h"
#include "ui_ColorControls.h"

#include "../FilterTransform/NonKernelBased/GrayscaleFilter.h"
#include "../FilterTransform/NonKernelBased/InvertFilter.h"

#include "../FilterTransform/NonKernelBased/HueFilter.h"
#include "../FilterTransform/NonKernelBased/SaturationFilter.h"
#include "../FilterTransform/NonKernelBased/TemperatureFilter.h"
#include "../FilterTransform/NonKernelBased/TintFilter.h"

#include "../FilterTransform/NonKernelBased/BrightnessFilter.h"
#include "../FilterTransform/NonKernelBased/ContrastFilter.h"
#include "../FilterTransform/NonKernelBased/ExposureFilter.h"

ColorControls::ColorControls(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColorControls)
{
    ui->setupUi(this);
}

ColorControls::~ColorControls()
{
    delete ui;
}

void ColorControls::on_bnwButton_clicked()
{
    GrayscaleFilter* grayscaleFilter = new GrayscaleFilter();
    emit applyColorFilterClicked(grayscaleFilter, 1, 1);
}

void ColorControls::on_invertButton_clicked()
{
    InvertFilter* invertFilter = new InvertFilter();
    emit applyColorFilterClicked(invertFilter, 1, 1);
}

void ColorControls::on_applyColorButton_clicked()
{
    int hueStrength = ui->hueSlider->value();
    int saturationStrength = ui->saturationSlider->value();
    int tintStrength = ui->tintSlider->value();
    int temperatureStrength = ui->temperatureSlider->value();

    if (hueStrength != 0) {
        HueFilter* hueFilter = new HueFilter();
        emit applyColorFilterClicked(hueFilter, 1, hueStrength);
    }
    if (saturationStrength != 0) {
        SaturationFilter* saturationFilter = new SaturationFilter();
        emit applyColorFilterClicked(saturationFilter, 1, saturationStrength);
    }
    if (tintStrength != 0) {
        TintFilter* tintFilter = new TintFilter();
        emit applyColorFilterClicked(tintFilter, 1, tintStrength);
    }
    if (temperatureStrength != 0) {
        TemperatureFilter* temperatureFilter = new TemperatureFilter();
        emit applyColorFilterClicked(temperatureFilter, 1, temperatureStrength);
    }
}

void ColorControls::on_applyLightingButton_clicked()
{
    int brightnessStrength = ui->brightnessSlider->value();
    int exposureStrength = ui->exposureSlider->value();
    int contrastStrength = ui->contrastSlider->value();
    if (brightnessStrength != 0) {
        BrightnessFilter* brigthnessFilter = new BrightnessFilter();
        emit applyColorFilterClicked(brigthnessFilter, 1, brightnessStrength);
    }
    if (exposureStrength != 0) {
        ExposureFilter* exposureFilter = new ExposureFilter();
        emit applyColorFilterClicked(exposureFilter, 1, exposureStrength);
    }
    if (contrastStrength != 0) {
        ContrastFilter* contrastFilter = new ContrastFilter();
        emit applyColorFilterClicked(contrastFilter, 1, contrastStrength);
    }
}
