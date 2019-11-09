#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "ScribbleWindow.h"
#include "Palette/Brush.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /*
     * Add a new toolbar dynamically, with specified fixed height,
     * since we need to add an expanding spacer, and widgets
    */
    ui->toolBar->setIconSize(QSize(55, 55));
    ui->toolBar->setFixedHeight(60);
    ui->toolBar->setContentsMargins(30, 0, 30, 0);
    ui->toolBar->setStyleSheet("QToolBar{spacing:30px;}");

    QWidget* spacerWidget = new QWidget(this);
    spacerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    spacerWidget->setVisible(true);

    ui->toolBar->addWidget(spacerWidget);
    ui->toolBar->addAction(ui->actionNew);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(ui->actionOpen);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(ui->actionSave);

    /*
     * Setup the treeWidget, which will be in our palette
     * TODO: fill each root
     */
    ui->palette->setColumnCount(1);
    QTreeWidgetItem* histogram = new QTreeWidgetItem(ui->palette);
    QTreeWidgetItem* basicControls = new QTreeWidgetItem(ui->palette);
    QTreeWidgetItem* colorControls = new QTreeWidgetItem(ui->palette);
    QTreeWidgetItem* brushControls = new QTreeWidgetItem(ui->palette);
    QTreeWidgetItem* effects = new QTreeWidgetItem(ui->palette);

    // TODO
    addRoot(histogram, "Histogram");

    // TODO
    addRoot(basicControls, "Basic Controls");

    // TODO
    addRoot(colorControls, "Color");

    addRoot(brushControls, "Brush");
    Brush* brush = new Brush();
    customAddChild(brushControls, brush);

    // TODO
    addRoot(effects, "Effects");

    // Sets the initial dimensions for the palette view and the workspace
    ui->splitter->setStretchFactor(0, 10);
    ui->splitter->setStretchFactor(1, 1);

    // Spawns a Scribble Window
    ScribbleWindow* scribbleWindow = new ScribbleWindow;
    ui->workspaceView->addWidget(scribbleWindow);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAbout_Us_triggered()
{
    AboutUs aboutUs;
    aboutUs.setModal(true);
    aboutUs.exec();
}

void MainWindow::addRoot(QTreeWidgetItem* parent, QString name)
{
    parent->setText(0, name);
    ui->palette->addTopLevelItem(parent);
}

void MainWindow::customAddChild(QTreeWidgetItem* parent, QWidget* widget)
{
    QTreeWidgetItem* item = new QTreeWidgetItem();
    parent->addChild(item);
    ui->palette->setItemWidget(item, 0, widget);
}

void MainWindow::on_actionPrint_triggered()
{
    QPrinter printer;
    printer.setPrinterName("Printer Name");
    QPrintDialog pDialog(&printer, this);
    if (pDialog.exec() == QDialog::Rejected) {
        QMessageBox::warning(this, "Warning", "Cannot Access printer");
        return;
    }
    // TODO: implement the printer, to print the current image file
    // Here is the sample, if the file is a text file
    // ui->textEdit->print(&printer);

}
