#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ModelPartList.h"
#include "ModelPart.h"
#include <QModelIndex>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void handleButton1();
    void handleButton2();
    void handleTreeClicked(const QModelIndex &index);
signals:
    void statusUpdateMessage(const QString& message, int timeout);
private slots:
    void on_actionOpen_File_triggered();

    void on_actionItem_Options_triggered();

private:
    Ui::MainWindow *ui;
    ModelPartList* partList;
};
#endif // MAINWINDOW_H
