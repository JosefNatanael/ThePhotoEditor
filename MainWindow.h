#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QTreeWidgetItem>
#include <QToolBar>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QPrinter>
#include <QPrintDialog>
#include <QVector>

#include "WindowHelper.h"

#include "WorkspaceArea.h"

#include "FilterTransform/AbstractImageFilterTransform.h"
#include "Palette/Histogram.h"
#include "Palette/Brush.h"
#include "Palette/ColorControls.h"
#include "Palette/BasicControls.h"
#include "Palette/Effects.h"
#include "AboutUs.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    void                        closeEvent(QCloseEvent* event) override;
    virtual bool                eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void                        on_actionNew_triggered();
    void                        on_actionOpen_triggered();
    void                        on_actionPrint_triggered();
    void                        on_actionAbout_Us_triggered();
    void                        on_actionExit_triggered();
    void                        on_actionUndo_triggered();
    void                        on_actionSave_triggered();

    void                        saveAs();
    void                        on_edit(QGraphicsPathItem*);
    void                        clearImage();
    void                        onZoom(const QString&);
    void                        onCrossCursorChanged(WorkspaceArea::CursorMode, int data);
    void                        onImageCropped(QImage, int width, int height);
    void                        applyFilterTransform(AbstractImageFilterTransform* filterTransform, int size, double strength);

private:
    void                        resizeGraphicsViewBoundaries(int newWidth, int newHeight);
    void                        reconnectConnection();
    void                        reconstructWorkspaceArea(int imageWidth, int imageHeight);
    void                        handleWheelEvent(QGraphicsSceneWheelEvent* event);
    void                        resetGraphicsViewScale();      // Revert changes caused by wheelEvent zoom

    void                        addRoot(QTreeWidgetItem* parent, QString name);
    void                        customAddChild(QTreeWidgetItem* parent, QWidget* widget);

    void                        createActions();
    void                        createMenus();
    bool                        maybeSave();
    bool                        saveAsFile(const QByteArray &fileFormat);
    void                        fitImageToScreen(int, int);

private:
    Ui::MainWindow*             ui;

    WorkspaceArea*              workspaceArea;
	WorkspaceArea*				temporaryArea = nullptr;
    QGraphicsView*              graphicsView;
    QVector<QGraphicsPathItem*> history;                // Saves the strokes created when drawing on the workspaceArea

    QMenu*                      optionMenu;             // optionMenu is generated during runtime
    QList<QAction *>            saveAsActs;             // all possible image format that can be used to save the image
    QAction*                    clearScreenAct;         // an action to clear the workspaceArea

    QTreeWidgetItem*            histogram;              // The parent wrapper of our histogram widget
    QTreeWidgetItem*            basicControls;          // The parent wrapper of our basicControls widget
    QTreeWidgetItem*            colorControls;          // The parent wrapper of our colorControls widget
    QTreeWidgetItem*            brushControls;          // The parent wrapper of our brushControls widget
    QTreeWidgetItem*            effects;                // The parent wrapper of our effects widget

    Histogram*                  histo;                  // Our Histogram widget
    Brush*                      brush;                  // Our Brush widget
    ColorControls*              colors;                 // Our ColorControls widget
    BasicControls*              basics;                 // Our BasicControls widget
    Effects*                    effect;                 // Our Effects widget

    QString                     fileName = "";          // filename of the loaded image
    QByteArray                  fileFormat;             // the file format of our loaded image
    bool                        fileSaved = false;      // the state that saves whether our current project been saved
    double                      currentZoom = 1.0;      // Saves our current zoom level
    double                      totalScaleX = 1.0;
    double                      totalScaleY = 1.0;
    QComboBox*                  comboBox;
    int                         resizedImageHeight = WorkspaceArea::SCENE_HEIGHT;
    int                         resizedImageWidth = WorkspaceArea::SCENE_WIDTH;
};

// Close the application
inline void MainWindow::on_actionExit_triggered()
{
    MainWindow::close();
}

// Saves the stroke history to the history
inline void MainWindow::on_edit(QGraphicsPathItem* item)
{
    history.push_back(item);
}


#endif // MAINWINDOW_H
