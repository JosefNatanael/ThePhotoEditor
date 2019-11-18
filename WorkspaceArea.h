#ifndef WORKSPACEAREA_H
#define WORKSPACEAREA_H

#include <QWidget>
#include <QColor>
#include <QImage>
#include <QPoint>
#include <QGraphicsScene>

namespace Ui {
class WorkspaceArea;
}

class WorkspaceArea : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit WorkspaceArea(QObject* parent = nullptr);
    WorkspaceArea(int width, int height, QObject* parent = nullptr);
    virtual ~WorkspaceArea() override = default;

public:
    bool                    openImage(const QImage&, int width, int height);
    bool                    saveImage(const QString& fileName, const char* fileFormat);
    int                     getImageWidth() const { return imageWidth; }
    int                     getImageHeight() const { return imageHeight; }
    bool                    getImageLoaded() const{ return isImageLoaded; }
    const QImage&           getImage() const{ return image; }
    bool                    isModified() const { return modified; }

    QColor                  penColor() const { return myPenColor; }
    int                     penWidth() const { return myPenWidth; }

    void                    setPenColor(const QColor& newColor);
    void                    setPenWidth(int newWidth);
    void                    setModified(bool);
    void                    setImageLoaded(bool);
    void                    resize(int, int);

    QImage                  commitImage();

public slots:
    void                    print();

signals:
    void                    imageLoaded(const QImage& image);     // Signals the mainwindow to update the histogram on image load
    void                    edited(QGraphicsPathItem*);               // Signals the on_edit slot that a stroke has been drawn

protected:
    virtual void            mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void            mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void            mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    bool                    modified;                   // Workspace was modified
    bool                    isImageLoaded = false;        // If true, loaded image can be drawn without new canvas

    QPen                    pen;                        // Pen for drawing paths
    int                     myPenWidth;                 // stores the width of the current pen
    QColor                  myPenColor;                 // stores what color is our pen

    QImage                  image;                      // Saves the current image
    int                     imageWidth;                 // Saves the width of our current image
    int                     imageHeight;                // Saves the height of our current image
    QGraphicsPixmapItem*    pixmapGraphics = nullptr;   // The pointer to foreground image item
    QGraphicsPathItem*      pathItem = nullptr;         // Pointer to the strokes created when drawing

};

// Used to change the pen color
inline void WorkspaceArea::setPenColor(const QColor &newColor)
{
    myPenColor = newColor;
}

// Used to change the pen width
inline void WorkspaceArea::setPenWidth(int newWidth)
{
    myPenWidth = newWidth;
}

// Used to change the modified state of the WorkspaceArea
inline void WorkspaceArea::setModified(bool modif)
{
    modified = modif;
}

// Used to change the loaded state of the WorkspaceArea
inline void WorkspaceArea::setImageLoaded(bool isLoaded)
{
    isImageLoaded = isLoaded;
}

#endif // WORKSPACEAREA_H
