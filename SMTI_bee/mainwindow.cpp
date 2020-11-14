#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include "klasa.h"

MainWindow::MainWindow(QWidget *parent) :   QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    NN=0; nn=0; mm=0; nnsp=0; nnep=0; ee=0; pp1=0; pp2=0;
    max=""; single="";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_lineEdit_textEdited(const QString &arg1)
{
    nn=arg1.toInt();
}

void MainWindow::on_lineEdit_2_textEdited(const QString &arg1)
{
    mm=arg1.toInt();
}

void MainWindow::on_lineEdit_3_textEdited(const QString &arg1)
{
    ee=arg1.toInt();
}

void MainWindow::on_lineEdit_4_textEdited(const QString &arg1)
{
    nnsp=arg1.toInt();
}

void MainWindow::on_lineEdit_5_textEdited(const QString &arg1)
{
    nnep=arg1.toInt();
}

void MainWindow::on_pushButton_released()
{

    smti SMTI (ui->comboBox->currentText().toInt(), nn, mm, ee, nnsp, nnep, ui->comboBox_2->currentText().toDouble(), ui->comboBox_3->currentText().toDouble());
    SMTI.find_marriage( max, single );
    ui->lineEdit_6->setText(max);
    ui->lineEdit_7->setText(single);

}
