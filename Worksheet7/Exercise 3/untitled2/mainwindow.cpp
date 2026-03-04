#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include "optiondialog.h"
#include "ModelPart.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(this, &MainWindow::statusUpdateMessage,
            ui->statusbar, &QStatusBar::showMessage);
    ui->treeView->addAction(ui->actionItem_Options);
    // Create / allocate the ModelPartList
    this->partList = new ModelPartList("Parts List");
    ui->treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    // Link it to the tree view in the GUI
    ui->treeView->setModel(this->partList);
    connect(ui->treeView, &QTreeView::clicked,
            this, &MainWindow::handleTreeClicked);

    connect(ui->pushButton_2, &QPushButton::released,
            this, &MainWindow::handleButton2);// Build demo tree
    ModelPart* rootItem = this->partList->getRootItem();

    // Add 3 top-level items
    for (int i = 0; i < 3; i++) {
        QString name = QString("TopLevel %1").arg(i);
        QString visible("true");

        ModelPart* childItem = new ModelPart({ name, visible, QVariant() });
        rootItem->appendChild(childItem);

        // Add 5 sub-items
        for (int j = 0; j < 5; j++) {
            QString childName = QString("Item %1,%2").arg(i).arg(j);
            QString childVisible("true");

            ModelPart* childChildItem = new ModelPart({ childName, childVisible, QVariant() });
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
    QModelIndex index = ui->treeView->currentIndex();
    if (!index.isValid()) {
        emit statusUpdateMessage("No item selected", 0);
        return;
    }

    ModelPart* selectedPart =
        static_cast<ModelPart*>(index.internalPointer());
    if (!selectedPart) return;

    // Read current values from the selected item
    const QString currentName = selectedPart->data(0).toString();

    const QString visStr = selectedPart->data(1).toString().toLower();
    const bool currentVisible = (visStr == "true");

    QColor currentColour = selectedPart->getColour();   // <-- HERE

    //  Create dialog and prefill it
    OptionDialog dialog(this);
    dialog.setValues(currentName, currentColour, currentVisible);   // <-- HERE

    //Show dialog
    if (dialog.exec() == QDialog::Accepted)
    {
        selectedPart->set(0, dialog.getName());
        selectedPart->set(1, dialog.getVisible() ? "true" : "false");
        selectedPart->setVisible(dialog.getVisible());

        QColor c = dialog.getColour();
        selectedPart->setColour(
            (unsigned char)c.red(),
            (unsigned char)c.green(),
            (unsigned char)c.blue()
            );

        emit statusUpdateMessage("ModelPart updated", 0);

        QModelIndex nameIdx = index.sibling(index.row(), 0);
        QModelIndex visIdx  = index.sibling(index.row(), 1);
        QModelIndex colIdx  = index.sibling(index.row(), 2);

        emit partList->dataChanged(nameIdx, nameIdx, {Qt::DisplayRole});
        emit partList->dataChanged(visIdx, visIdx, {Qt::CheckStateRole});
        emit partList->dataChanged(colIdx, colIdx, {Qt::DecorationRole});
    }
    else
    {
        emit statusUpdateMessage("Dialog rejected", 0);
    }
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
    // must have something selected
    QModelIndex idx = ui->treeView->currentIndex();
    if (!idx.isValid()) {
        emit statusUpdateMessage("Select an item in the tree first.", 0);
        return;
    }

    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        "C:\\",
        tr("STL Files (*.stl);;Text Files (*.txt);;All Files (*.*)")
        );
    if (fileName.isEmpty())
        return;

    // choose what to show in the tree:
    QFileInfo fi(fileName);
    QString displayName = fi.fileName();

    // rename the selected item
    QModelIndex nameIndex = idx.sibling(idx.row(), 0);


    if (!ui->treeView->model()->setData(nameIndex, displayName, Qt::EditRole)) {
        emit statusUpdateMessage("Failed to rename selected item.", 0);
        return;
    }

    emit statusUpdateMessage("Selected file: " + fileName, 0);
}


void MainWindow::on_actionItem_Options_triggered()
{
    handleButton2();

}
