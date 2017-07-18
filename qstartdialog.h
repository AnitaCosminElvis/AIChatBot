#ifndef QSTARTDIALOG_H
#define QSTARTDIALOG_H

#include <QDialog>
#include <QIntValidator>
#include <QString>

namespace Ui {
class QStartDialog;
}

class QStartDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QStartDialog(QWidget *parent = 0);
    ~QStartDialog();

protected:
    void showEvent(QShowEvent*pEvent);

signals:
    void CloseApplication();
    void FinishedSignal(QString qsName, qint32 nAge, QString qsOccupation);

private slots:
    void on_pushButton_clicked();

private:
    Ui::QStartDialog *ui;
};

#endif // QSTARTDIALOG_H
