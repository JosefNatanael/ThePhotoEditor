/**
 * @class ColorControls
 * @brief Image filters to apply.
 */
#include "ColorControls.h"
#include "ui_ColorControls.h"
#include <QSignalMapper>

#include "../FilterTransform/NonKernelBased/GrayscaleFilter.h"
#include "../FilterTransform/NonKernelBased/InvertFilter.h"

#include "../FilterTransform/NonKernelBased/HueFilter.h"
#include "../FilterTransform/NonKernelBased/SaturationFilter.h"
#include "../FilterTransform/NonKernelBased/TemperatureFilter.h"
#include "../FilterTransform/NonKernelBased/TintFilter.h"

#include "../FilterTransform/NonKernelBased/BrightnessFilter.h"
#include "../FilterTransform/NonKernelBased/ContrastFilter.h"
#include "../FilterTransform/NonKernelBased/ExposureFilter.h"

/**
 * @brief Construct a new Color Controls:: Color Controls object
 * 
 * @param parent Passed to QWidget() constructor.
 */
ColorControls::ColorControls(QWidget *parent) : QWidget(parent),
                                                ui(new Ui::ColorControls)
{
    ui->setupUi(this);
    imagePreview.load(":/icons/resources/noimageavail.png");
    imagePreview = imagePreview.scaled(200, 200, Qt::IgnoreAspectRatio);
    ui->imagePreviewLabel->setPixmap(imagePreview);

    // Setup connection, for each slider, make a connection to main window. This is used to do image previewing.
    connect(ui->hueSlider, &QSlider::valueChanged, this, [=](int value) {
        emit onSliderValueChanged(0, value);
    });
    connect(ui->saturationSlider, &QSlider::valueChanged, this, [=](int value) {
        emit onSliderValueChanged(1, value);
    });
    connect(ui->tintSlider, &QSlider::valueChanged, this, [=](int value) {
        emit onSliderValueChanged(2, value);
    });
    connect(ui->temperatureSlider, &QSlider::valueChanged, this, [=](int value) {
        emit onSliderValueChanged(3, value);
    });
    connect(ui->brightnessSlider, &QSlider::valueChanged, this, [=](int value) {
        emit onSliderValueChanged(4, value);
    });
    connect(ui->exposureSlider, &QSlider::valueChanged, this, [=](int value) {
        emit onSliderValueChanged(5, value);
    });
    connect(ui->contrastSlider, &QSlider::valueChanged, this, [=](int value) {
        emit onSliderValueChanged(6, value);
    });
}

/**
 * @brief Destroy the Color Controls:: Color Controls object
 */
ColorControls::~ColorControls()
{
    delete ui;
}

/**
 * @brief Emits grayscale filter signal to main window.
 * 
 */
void ColorControls::on_bnwButton_clicked()
{
    GrayscaleFilter *grayscaleFilter = new GrayscaleFilter();
    emit applyColorFilterClicked(grayscaleFilter, 1, 1);
}

/**
 * @brief Emits invert filter signal to main window.
 * 
 */
void ColorControls::on_invertButton_clicked()
{
    InvertFilter *invertFilter = new InvertFilter();
    emit applyColorFilterClicked(invertFilter, 1, 1);
}

/**
 * @brief Emits color filters signal to main window.
 * 
 */
void ColorControls::on_applyColorButton_clicked()
{
    int hueStrength = ui->hueSlider->value();
    int saturationStrength = ui->saturationSlider->value();
    int tintStrength = ui->tintSlider->value();
    int temperatureStrength = ui->temperatureSlider->value();

    if (hueStrength != 0)
    {
        HueFilter *hueFilter = new HueFilter();
        emit applyColorFilterClicked(hueFilter, 1, hueStrength);
    }
    if (saturationStrength != 0)
    {
        SaturationFilter *saturationFilter = new SaturationFilter();
        emit applyColorFilterClicked(saturationFilter, 1, saturationStrength);
    }
    if (tintStrength != 0)
    {
        TintFilter *tintFilter = new TintFilter();
        emit applyColorFilterClicked(tintFilter, 1, tintStrength);
    }
    if (temperatureStrength != 0)
    {
        TemperatureFilter *temperatureFilter = new TemperatureFilter();
        emit applyColorFilterClicked(temperatureFilter, 1, temperatureStrength);
    }
}

/**
 * @brief Emits lighting filter signal to main window.
 */
void ColorControls::on_applyLightingButton_clicked()
{
    int brightnessStrength = ui->brightnessSlider->value();
    int exposureStrength = ui->exposureSlider->value();
    int contrastStrength = ui->contrastSlider->value();
    if (brightnessStrength != 0)
    {
        BrightnessFilter *brigthnessFilter = new BrightnessFilter();
        emit applyColorFilterClicked(brigthnessFilter, 1, brightnessStrength);
    }
    if (exposureStrength != 0)
    {
        ExposureFilter *exposureFilter = new ExposureFilter();
        emit applyColorFilterClicked(exposureFilter, 1, exposureStrength);
    }
    if (contrastStrength != 0)
    {
        ContrastFilter *contrastFilter = new ContrastFilter();
        emit applyColorFilterClicked(contrastFilter, 1, contrastStrength);
    }
}

/**
 * @brief Sets image previewer image to image.
 * 
 * @param image Image to be in the image previewer.
 */
void ColorControls::setImagePreview(const QImage& image) {

    ui->imagePreviewLabel->setPixmap(QPixmap::fromImage(image));
}

/**
 * @brief Emits signal, based on id, to main window.
 * @details To update image previewer based on slider changes.
 * 
 * @param id Filter whose slider changed.
 * @param value Slider change value.
 */
void ColorControls::onSliderValueChanged(int id, int value) {
    switch (id) {
        case 0: {
            HueFilter *hueFilter = new HueFilter();
            emit applyColorFilterOnPreview(hueFilter, 1, value);
            break;
        }
        case 1: {
            SaturationFilter *saturationFilter = new SaturationFilter();
            emit applyColorFilterOnPreview(saturationFilter, 1, value);
            break;
        }
        case 2: {
            TintFilter *tintFilter = new TintFilter();
            emit applyColorFilterOnPreview(tintFilter, 1, value);
            break;
        }
        case 3: {
            TemperatureFilter *temperatureFilter = new TemperatureFilter();
            emit applyColorFilterOnPreview(temperatureFilter, 1, value);
            break;
        }
        case 4: {
            BrightnessFilter *brightnessFilter = new BrightnessFilter();
            emit applyColorFilterOnPreview(brightnessFilter, 1, value);
            break;
        }
        case 5: {
            ExposureFilter *exposureFilter = new ExposureFilter();
            emit applyColorFilterOnPreview(exposureFilter, 1, value);
            break;
        }
        case 6: {
            ContrastFilter *contrastFilter = new ContrastFilter();
            emit applyColorFilterOnPreview(contrastFilter, 1, value);
            break;
        }
    }
}

/**
 * @brief Resets sliders.
 */
void ColorControls::resetSliders() {
    ui->hueSlider->setValue(0);
    ui->saturationSlider->setValue(0);
    ui->tintSlider->setValue(0);
    ui->temperatureSlider->setValue(0);
    ui->brightnessSlider->setValue(0);
    ui->exposureSlider->setValue(0);
    ui->contrastSlider->setValue(0);
}
