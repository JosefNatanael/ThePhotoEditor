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
    int                         getImageWidth() const { return imageWidth; }                    //!< A Getter function.
    int                         getImageHeight() const { return imageHeight; }                  //!< A Getter function.
    bool                        getImageLoaded() const{ return isImageLoaded; }                 //!< A Getter function.
    const QImage&               getImage() const{ return image; }                               //!< A Getter function.
    bool                        isModified() const { return modified; }                         //!< A Getter function.

    QColor                      penColor() const { return myPenColor; }                         //!< A Getter function.
    int                         penWidth() const { return myPenWidth; }                         //!< A Getter function.

    void                        setPenColor(const QColor& newColor) { myPenColor = newColor; }                      //!< A Setter function.
    void                        setPenWidth(int newWidth) { myPenWidth = newWidth; }                                //!< A Setter function.
    void                        setModified(bool modified) { this->modified = modified; }                           //!< A Setter function.
    void                        setImageLoaded(bool isImageLoaded) { this->isImageLoaded = isImageLoaded ; }        //!< A Setter function.
    void                        setCursorMode(CursorMode cursorMode) { this->cursorMode = cursorMode; }             //!< A Setter function.
    void                        setMagicWandThreshold(int threshold) { this->magicWandThreshold = threshold; }      //!< A Setter function.
    QImage                      commitImage();
    void                        commitImageAndSet() { image = commitImage(); }                  //!< Set the workspaceImage to a committed version of it.
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
