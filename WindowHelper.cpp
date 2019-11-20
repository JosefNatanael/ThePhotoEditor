#include "WindowHelper.h"

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

QScreen* WindowHelper::screenFromWidget(const QWidget *widget)
{
    const QWindow *windowHandle = windowFromWidget(widget);
    if (windowHandle && windowHandle->screen()) {
        return windowHandle->screen();
    }

    return QGuiApplication::primaryScreen();
}
