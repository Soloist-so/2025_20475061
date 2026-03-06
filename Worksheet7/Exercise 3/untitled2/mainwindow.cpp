#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include "optiondialog.h"
#include "ModelPart.h"
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkCamera.h>
#include <vtkNew.h>
#include <vtkSTLReader.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Link VTK render window with Qt widget
    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    ui->widget->setRenderWindow(renderWindow);

    // Create renderer
    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderWindow->AddRenderer(renderer);

    // Render
    renderWindow->Render();

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
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        "C:\\",
        tr("STL Files (*.stl)")
    );

    if (fileName.isEmpty())
        return;

    QModelIndex parentIndex = ui->treeView->currentIndex();
    if (!parentIndex.isValid())
        parentIndex = QModelIndex();

    QString displayName = QFileInfo(fileName).fileName();

    QList<QVariant> rowData;
    rowData << displayName << "true" << QVariant();

    QModelIndex newIndex = partList->appendChild(parentIndex, rowData);

    ModelPart* newItem = static_cast<ModelPart*>(newIndex.internalPointer());

    if (newItem)
    {
        newItem->loadSTL(fileName);
        updateRender();

    }

    emit statusUpdateMessage("Loaded: " + fileName, 0);
}


void MainWindow::on_actionItem_Options_triggered()
{
    handleButton2();

}
void MainWindow::updateRender()
{
    renderer->RemoveAllViewProps();

    // start from the root level
    int rows = partList->rowCount(QModelIndex());
    for (int i = 0; i < rows; ++i) {
        updateRenderFromTree(partList->index(i, 0, QModelIndex()));
    }

    renderer->ResetCamera();
    renderWindow->Render();
}

void MainWindow::updateRenderFromTree(const QModelIndex& index)
{
    if (!index.isValid())
        return;

    ModelPart* selectedPart =
        static_cast<ModelPart*>(index.internalPointer());

    if (selectedPart) {
        // only add actor if it exists and the item is visible
        vtkSmartPointer<vtkActor> actor = selectedPart->getActor();
        if (actor && selectedPart->visible()) {
            renderer->AddActor(actor);
        }
    }

    // recurse through children
    if (!partList->hasChildren(index) || (index.flags() & Qt::ItemNeverHasChildren))
        return;

    int rows = partList->rowCount(index);
    for (int i = 0; i < rows; ++i) {
        updateRenderFromTree(partList->index(i, 0, index));
    }
}