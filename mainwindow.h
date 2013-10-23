#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QJsonObject>
#include <QJsonArray>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    QTreeWidgetItem* createItem(QString repr, QString type, QString value);
    QTreeWidgetItem* createItemForValue(QString key, QJsonValue value);

private slots:
    void on_actionOpen_triggered();
    void on_actionQuit_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
