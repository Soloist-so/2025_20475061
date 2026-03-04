#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H
#include <QColor>
#include <QDialog>

namespace Ui {
class OptionDialog;
}

class OptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionDialog(QWidget *parent = nullptr);
    ~OptionDialog();
    void setValues(const QString& name, const QColor& colour, bool visible);
    QString getName() const;
    QColor getColour() const;
    bool getVisible() const;
private slots:
    void onPickColour();

private:
    Ui::OptionDialog *ui;
    QColor selectedColour;
};

#endif // OPTIONDIALOG_H
