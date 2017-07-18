#include "qstartdialog.h"
#include "ui_qstartdialog.h"

QStartDialog::QStartDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QStartDialog)
{
    ui->setupUi(this);
    QRegExp qRegExp("[A-z]+");
    QRegExpValidator* qRegExpValidator = new QRegExpValidator(qRegExp,this);
    ui->lineEdit->setValidator(qRegExpValidator);
    ui->lineEdit_2->setValidator(new QIntValidator(0, 200, this));
    ui->lineEdit_3->setValidator(qRegExpValidator);
    this->setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
}

QStartDialog::~QStartDialog()
{
    delete ui;
}

void QStartDialog::showEvent(QShowEvent* pEvent)
{
    QDialog::showEvent(pEvent);
}


void QStartDialog::on_pushButton_clicked()
{
    if (ui->lineEdit->text().isEmpty()){
        ui->lineEdit->setFocus();
        return;
    }

    if (ui->lineEdit_2->text().isEmpty()){
        ui->lineEdit_2->setFocus();
        return;
    }

    if (ui->lineEdit_3->text().isEmpty()){
        ui->lineEdit_3->setFocus();
        return;
    }

    emit FinishedSignal(ui->lineEdit->text(),ui->lineEdit_2->text().toInt(),ui->lineEdit_3->text());
    this->close();
}
