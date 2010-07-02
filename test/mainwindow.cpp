#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Gui/ShareOnInternet.h"

#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
//    QString fileName = QFileDialog::getOpenFileName( this,
//                                                 tr( "Choose Video File to upload" ), "",
//                                                 tr( "Video files (*.avi *.flv *.mov *.mp4 *.mpg)" ) );

    /* Add code to transcode/export video */
//    ui->filePath->setText( fileName );
    shareOnInternet();
}

void MainWindow::shareOnInternet()
{
    QString filePath = ui->filePath->text();
    ShareOnInternet *exportToInternet = new ShareOnInternet();


    if ( exportToInternet->exec() == QDialog::Rejected )
    {
        qDebug() << "Dialog cancelled...";
        delete exportToInternet;
        return ;
    }
    qDebug() << "All right... stuck here";

    //delete exportToInternet;
}

void MainWindow::on_abortButton_clicked()
{
    ui->log->appendPlainText("[UPLOAD ABORTED]");
}
