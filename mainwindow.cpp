#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(this, &MainWindow::statusUpdateMessage,
            ui->statusbar, &QStatusBar::showMessage);
    // Create / allocate the ModelPartList
    this->partList = new ModelPartList("Parts List");
    ui->treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    // Link it to the tree view in the GUI
    ui->treeView->setModel(this->partList);
    connect(ui->treeView, &QTreeView::clicked,
            this, &MainWindow::handleTreeClicked);
    // Build demo tree
    ModelPart* rootItem = this->partList->getRootItem();

    // Add 3 top-level items
    for (int i = 0; i < 3; i++) {
        QString name = QString("TopLevel %1").arg(i);
        QString visible("true");

        ModelPart* childItem = new ModelPart({ name, visible });
        rootItem->appendChild(childItem);

        // Add 5 sub-items
        for (int j = 0; j < 5; j++) {
            QString childName = QString("Item %1,%2").arg(i).arg(j);
            QString childVisible("true");

            ModelPart* childChildItem = new ModelPart({ childName, childVisible });
            childItem->appendChild(childChildItem);
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::handleButton1()
{
    emit statusUpdateMessage(QString("Button 1 was clicked"), 0);
}

void MainWindow::handleButton2()
{
    emit statusUpdateMessage(QString("Button 2 was clicked"), 0);
}

void MainWindow::handleTreeClicked(const QModelIndex &index)
{
    if (!index.isValid()) return;

    auto* selectedPart = static_cast<ModelPart*>(index.internalPointer());
    if (!selectedPart) return;

    QString text = selectedPart->data(0).toString();
    emit statusUpdateMessage(QString("The selected item is: ") + text, 0);
}

void MainWindow::on_actionOpen_File_triggered()
{
    emit statusUpdateMessage("Open File action triggered", 0);

}

