/**
 * @class WindowHelper
 * @brief Helper class to get window or screen from a widget.
 */

#include "WindowHelper.h"

/**
 * @brief Gets the QWindow of a widget.
 * 
 * @param widget 
 * @return QWindow* 
 */
QWindow* WindowHelper::windowFromWidget(const QWidget *widget)
{
    QWindow *windowHandle = widget->windowHandle();
    if (windowHandle) {
        return windowHandle;
    }

    const QWidget *nativeParent = widget->nativeParentWidget();
    if (nativeParent) {
        return nativeParent->windowHandle();
    }

    return nullptr;
}

/**
 * @brief Gets the QScreen of a widget.
 * 
 * @param widget 
 * @return QScreen* 
 */
QScreen* WindowHelper::screenFromWidget(const QWidget *widget)
{
    const QWindow *windowHandle = windowFromWidget(widget);
    if (windowHandle && windowHandle->screen()) {
        return windowHandle->screen();
    }

    return QGuiApplication::primaryScreen();
}
