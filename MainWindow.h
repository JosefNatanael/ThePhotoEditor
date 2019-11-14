#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "AboutUs.h"

#include <QTreeWidgetItem>
#include <QToolBar>

#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QPrinter>
#include <QPrintDialog>
#include <QVector>

#include "WorkspaceArea.h"
#include "Palette/Histogram.h"
#include "Palette/Brush.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void                        resizeGraphicsViewBoundaries(int newWidth, int newHeight);
    void                        reconnectConnection();
    void                        reconstructWorkspaceArea(int imageWidth, int imageHeight);

protected:
    void                        closeEvent(QCloseEvent* event) override;

private slots:
    void                        on_actionNew_triggered();
    void                        on_actionOpen_triggered();
    void                        on_actionPrint_triggered();
    void                        on_actionAbout_Us_triggered();
    void                        on_actionExit_triggered();
    void                        on_actionUndo_triggered();

    void                        open();
    void                        saveAs();
    void                        on_edit(QGraphicsPathItem*);
    void                        clearImage();


private:
    Ui::MainWindow*             ui;

    void                        addRoot(QTreeWidgetItem* parent, QString name);
    void                        customAddChild(QTreeWidgetItem* parent, QWidget* widget);

    void                        createActions();
    void                        createMenus();
    bool                        maybeSave();
    bool                        saveFile(const QByteArray &fileFormat);

    WorkspaceArea*              workspaceArea;
    QGraphicsView*              graphicsView;
    QVector<QGraphicsPathItem*> history;

    QMenu*                      optionMenu;
    QList<QAction *>            saveAsActs;
    QAction*                    clearScreenAct;

    QTreeWidgetItem*            histogram;
    QTreeWidgetItem*            basicControls;
    QTreeWidgetItem*            colorControls;
    QTreeWidgetItem*            brushControls;
    QTreeWidgetItem*            effects;

    Histogram*                  histo;
    Brush*                      brush;

    QImage                      loadedImage;
};

#endif // MAINWINDOW_H
