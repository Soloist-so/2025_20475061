#include "optiondialog.h"
#include "ui_optiondialog.h"
#include <QColorDialog>

OptionDialog::OptionDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OptionDialog)
{
    ui->setupUi(this);
    connect(ui->pickColourButton, &QPushButton::clicked,
            this, &OptionDialog::onPickColour);
}

OptionDialog::~OptionDialog()
{
    delete ui;
}
void OptionDialog::onPickColour()
{
    QColor colour = QColorDialog::getColor(selectedColour, this, "Select Colour");

    if (colour.isValid()) {
        selectedColour = colour;

        ui->colourPreview->setStyleSheet(
            "background-color: " + selectedColour.name()
            );
    }
}
QString OptionDialog::getName() const
{
    return ui->lineEdit->text();
}

QColor OptionDialog::getColour() const
{
    return selectedColour;
}

bool OptionDialog::getVisible() const
{
    return ui->checkBox->isChecked();
}
