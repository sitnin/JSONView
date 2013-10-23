#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iomanip>

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
#include <QTreeWidget>
#include <QJsonArray>
#include <QJsonObject>
#include <QList>
#include <QTreeWidgetItem>

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

QTreeWidgetItem* MainWindow::createItem(QString repr, QString type, QString value)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setData(0,Qt::DisplayRole, repr);
    item->setData(1,Qt::DisplayRole, type);
    item->setData(2,Qt::DisplayRole, value);

    return item;
}

QTreeWidgetItem* MainWindow::createItemForValue(QString key, QJsonValue value)
{
    QTreeWidgetItem *item;

    if (value.isNull()) {
        item = this->createItem(key, "Null", "null");
    } else if (value.isBool()) {
        bool val = value.toBool();
        item = this->createItem(key, "Bool", val == true ? "true": "false");
    } else if (value.isDouble()) {
        item = this->createItem(key, "Double", QVariant(value.toDouble()).toString());
    } else if (value.isString()) {
        item = this->createItem(key, "String", value.toString());
    } else if (value.isUndefined()) {
        item = this->createItem(key, "Undefined", "undefined");
    } else if (value.isObject()) {
        QJsonObject obj = value.toObject();
        QStringList keys = obj.keys();

        item = this->createItem(key, "Object", "("+QVariant(keys.count()).toString()+" keys)");

        QList<QTreeWidgetItem*> items;
        foreach(QString key, keys) {
            items.append(this->createItemForValue(key, obj.value(key)));
        }
        item->addChildren(items);
    } else if (value.isArray()) {
        QJsonArray arr = value.toArray();

        item = this->createItem(key, "Array", "("+QVariant(arr.count()).toString()+" items)");

        QList<QTreeWidgetItem*> items;
        int i=-1;
        foreach(QJsonValue val, arr) {
            i++;
            items.append(this->createItemForValue(QString(i), val));
        }
        item->addChildren(items);
    }

    return item;
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(), tr("JSON Files (*.json);;All files (*)"));

    QJsonDocument doc;

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
            return;
        }

        QTextStream in(&file);
        QString docStr = in.readAll();
        file.close();

        doc = QJsonDocument::fromJson(docStr.toUtf8());
    } else {
        return;
    }

    QTreeWidgetItem *rootElement;

    if (doc.isEmpty()) {
        rootElement = this->createItem("Document","(empty)","");
    } else if (doc.isNull()) {
        rootElement = this->createItem("Document","Null","");
    } else if (doc.isObject()) {
        rootElement = this->createItemForValue("Document",doc.object());
    } else if (doc.isArray()) {
        rootElement = this->createItemForValue("Document",doc.array());
    }

    ui->treeWidget->clear();
    ui->treeWidget->addTopLevelItem(rootElement);
}

void MainWindow::on_actionQuit_triggered()
{
    qApp->quit();
}
