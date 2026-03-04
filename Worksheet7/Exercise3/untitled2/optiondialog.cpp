#include "optiondialog.h"
#include "ui_optiondialog.h"
#include <QColorDialog>

OptionDialog::OptionDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OptionDialog)
{
    ui->setupUi(this);
    selectedColour = Qt::white;
    ui->colourPreview->setStyleSheet("background-color: " + selectedColour.name());
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

void OptionDialog::setValues(const QString& name, const QColor& colour, bool visible)
{
    ui->lineEdit->setText(name);
    ui->checkBox->setChecked(visible);

    selectedColour = colour.isValid() ? colour : Qt::white;
    ui->colourPreview->setStyleSheet("background-color: " + selectedColour.name());
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
