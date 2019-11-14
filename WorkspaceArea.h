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
    bool    openImage(const QImage&, int width, int height);
    bool    saveImage(const QString& fileName, const char* fileFormat);
    int     getImageWidth() const { return imageWidth; }
    int     getImageHeight() const { return imageHeight; }

    QColor  penColor() const { return myPenColor; }
    int     penWidth() const { return myPenWidth; }
    void    setPenColor(const QColor& newColor);
    void    setPenWidth(int newWidth);

    const   QImage& getImage() const{ return image; }
    bool    isModified() const { return modified; }
    QImage  commitImage();

public slots:
    void    clearImage();
    void    print();

signals:
    void    onImageLoaded(const QImage& image);
    void    edit(QGraphicsPathItem*);

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    bool                    modified; // Workspace was modified

    QPen                    pen; // Pen for drawing paths
    int                     myPenWidth;
    QColor                  myPenColor;

    QImage                  image;
    int                     imageWidth;
    int                     imageHeight;
    QGraphicsPixmapItem*    pixmapGraphics = nullptr; // The pointer to foreground image item
    QGraphicsPathItem*      pathItem = nullptr;

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

#endif // WORKSPACEAREA_H
