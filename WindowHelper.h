#ifndef WINDOWHELPER_H
#define WINDOWHELPER_H

#include <QScreen>
#include <QWindow>
#include <QApplication>
#include <QWidget>

class WindowHelper
{
public:
    WindowHelper() = delete;
    static QWindow* windowFromWidget(const QWidget *widget);
    static QScreen* screenFromWidget(const QWidget *widget);
};

#endif // WINDOWHELPER_H
