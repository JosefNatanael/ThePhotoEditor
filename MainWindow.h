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
#include "Palette/ColorControls.h"

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

private:
    void                        resizeGraphicsViewBoundaries(int newWidth, int newHeight);
    void                        reconnectConnection();
    void                        reconstructWorkspaceArea(int imageWidth, int imageHeight);

    void                        addRoot(QTreeWidgetItem* parent, QString name);
    void                        customAddChild(QTreeWidgetItem* parent, QWidget* widget);

    void                        createActions();
    void                        createMenus();
    bool                        maybeSave();
    bool                        saveAsFile(const QByteArray &fileFormat);

private:
    Ui::MainWindow*             ui;

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
    ColorControls*              colors;

    QImage                      loadedImage;
    QString                     fileName = "";
    QByteArray                  fileFormat;
    bool                        fileSaved = false;
};

inline void MainWindow::on_actionPrint_triggered()
{
    workspaceArea->print();
}

inline void MainWindow::on_actionExit_triggered()
{
    MainWindow::close();
}

inline void MainWindow::on_edit(QGraphicsPathItem* item)
{
    history.push_back(item);
}


#endif // MAINWINDOW_H
