#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QWidget>
#include <QSettings>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QCloseEvent>
#include <QJsonDocument>
#include <QString>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QTreeView>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSettings settings;
    // TODO: Calculate middle of the screen on first run and save coordinates

    restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
    restoreState(settings.value("mainWindowState").toByteArray());
}

MainWindow::~MainWindow()
{
    QSettings settings;
    settings.setValue("mainWindowGeometry", saveGeometry());
    settings.setValue("mainWindowState", saveState());

    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(), tr("JSON Files (*.json);;All files (*)"));

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
            return;
        }

        QTextStream in(&file);
        QString docStr = in.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(docStr.toUtf8());
    } else {
        return;
    }

    QStandardItemModel model;
    QStandardItem *parentItem = model.invisibleRootItem();
    for (int i = 0; i < 4; ++i) {
        QStandardItem *item = new QStandardItem(QString("item %0").arg(i));
        parentItem->appendRow(item);
        parentItem = item;
    }

    QTreeView *treeView = new QTreeView(this);
    treeView->setModel(&model);
//    connect(treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(clicked(QModelIndex)));
}

void MainWindow::on_actionQuit_triggered()
{
    qApp->quit();
}
