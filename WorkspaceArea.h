#ifndef WORKSPACEAREA_H
#define WORKSPACEAREA_H

#include <QWidget>
#include <QColor>
#include <QImage>
#include <QPoint>
#include <QGraphicsScene>
#include <QRubberBand>

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
/**
 * @enum CursorMode.
 * 
 * @brief Enum class to differentiate different cursors.
 */
    enum class CursorMode {
        SCRIBBLE,
        RECTANGLECROP,
        MAGICWAND,
        LASSO
    };

public:
    void                        openImage(const QImage&, int width, int height);
    bool                        saveImage(const QString& fileName, const char* fileFormat);
    int                         getImageWidth() const { return imageWidth; }                    //!< Get image width.
    int                         getImageHeight() const { return imageHeight; }                  //!< Get image height.
    bool                        getImageLoaded() const{ return isImageLoaded; }                 //!< Is image loaded.
    const QImage&               getImage() const{ return image; }                               //!< Returns workspace area image.
    bool                        isModified() const { return modified; }                         //!< Returns if workspace area is modified.

    QColor                      penColor() const { return myPenColor; }                         //!< Get pen color.
    int                         penWidth() const { return myPenWidth; }                         //!< Get pen width.

    void                        setPenColor(const QColor& newColor) { myPenColor = newColor; }                      //!< Sets pen color to newColor.
    void                        setPenWidth(int newWidth) { myPenWidth = newWidth; }                                //!< Sets pen width to newWidth.
    void                        setModified(bool modified) { this->modified = modified; }                           //!< Sets if workspace area is modified.
    void                        setImageLoaded(bool isImageLoaded) { this->isImageLoaded = isImageLoaded ; }        //!< Sets if workspace area is loaded with image.
    void                        setCursorMode(CursorMode cursorMode) { this->cursorMode = cursorMode; }             //!< Sets cursor mode of workspace area.
    void                        setMagicWandThreshold(int threshold) { this->magicWandThreshold = threshold; }      //!< Sets magic wand threshold.
    QImage                      commitImage();
    void                        commitImageAndSet() { image = commitImage(); }                  //!< Sets the workspaceImage to a committed version of it.
    QImage                      commitImageForPreview();

public slots:
    void                        print();
    void                        resizeImage(int, int);

public:
    static const int SCENE_WIDTH = 720;    //!< The default width of the workspace
    static const int SCENE_HEIGHT = 480;   //!< The default height of the workspace

signals:
    void                        imageLoaded(const QImage& image);                       //!< Signals the mainwindow to update the histogram on image load.
    void                        imageDrawn();                                           //!< Signals the mainwindow, go to slot &MainWindow::onImageDrawn.
    void                        imageCropped(const QImage&, int width, int height);     //!< Signals the mainwindow to rerender workspaceArea with these arguments
    void                        imageResized(const QImage&, int width, int height);     //!< Signals the mainwindow to rerender workspaceArea with these arguments
    void                        commitChanges(QString changes);                         //!< Signals the mainwindow, go to slot &Mainwindow::onCommitChanges
    void                        updateImagePreview();                                   //!< Signals the mainwindow, go to slot &Mainwindow::onUpdateImagePreview

protected:
    virtual void                mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void                mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void                mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    bool                        modified;                           //!< Workspace was modified
    bool                        isImageLoaded = false;              //!< If true, loaded image can be drawn without new canvas
    CursorMode                  cursorMode = CursorMode::SCRIBBLE;  //!< Mouse events will be used to scribble the workspacearea
    QRubberBand*                rubberBand = nullptr;               //!< Rubber band is rectangular crop box, that resizes according to mouse movement.
    QPoint                      rubberBandOrigin;                   //!< Origin of the rubber band.

    QPen                        pen;                                //!< Pen for drawing paths
    int                         myPenWidth;                         //!< stores the width of the current pen
    QColor                      myPenColor;                         //!< stores what color is our pen

    QImage                      image;                              //!< Saves the current image
    int                         imageWidth;                         //!< Saves the width of our current image
    int                         imageHeight;                        //!< Saves the height of our current image
    QGraphicsPixmapItem*        pixmapGraphics = nullptr;           //!< The pointer to foreground image item
    QGraphicsPathItem*          pathItem = nullptr;                 //!< Pointer to the strokes created when drawing

    QPoint                      cropOriginScreen;                   //!< Point to start cropping, relative to screen
    QPoint                      cropOrigin;                         //!< Point to start cropping, relative to scene
    int                         cropX;                              //!< Stores the crop length in x direction
    int                         cropY;                              //!< Stores the crop length in y direction
    double                      dx;                                 //!< x difference betwwen screen position and scene position
    double                      dy;                                 //!< y difference between screen position and scene position
    QRgb                        thisColor;                          //!< Magic wand color to remove
    int                         magicWandThreshold = 5;             //!< Threshold for magic wand tolerence.
};

#endif // WORKSPACEAREA_H
