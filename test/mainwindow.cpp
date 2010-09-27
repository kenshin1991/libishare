#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Gui/ShareOnInternet.h"

#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QNetworkProxy>

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
    fileName = QFileDialog::getOpenFileName( this,
                                                tr( "Choose Video File to upload" ), "",
                                                tr( "Video files (*.avi *.flv *.mov *.mp4 *.mpg *.*)" ) );

    /* Add code to transcode/export video */
    shareOnInternet();
}

void MainWindow::shareOnInternet()
{
    ShareOnInternet *exportToInternet = new ShareOnInternet();

    exportToInternet->setVideoFile(fileName);

    if ( exportToInternet->exec() == QDialog::Rejected )
    {
        qDebug() << "[Main Window] Dialog cancelled...";
        delete exportToInternet;
        return ;
    }
    qDebug() << "[Main Window] Done!";
    delete exportToInternet;
}

void MainWindow::updateProxy()
{
    if( ui->proxy->checkState() == Qt::Checked )
    {
        /* Updates Global Proxy for VLMC */
        qDebug() << "[Main Window] Setting up Global proxy";
        QNetworkProxy proxy;
        proxy.setType( QNetworkProxy::HttpProxy );
        proxy.setHostName( ui->hostName->text() );
        proxy.setPort( ui->port->text().toInt() );
        proxy.setUser( ui->username->text() );
        proxy.setPassword( ui->password->text() );
        QNetworkProxy::setApplicationProxy( proxy );
        return;
    }

    QNetworkProxy::setApplicationProxy( QNetworkProxy::NoProxy );
}

void MainWindow::on_proxy_clicked()
{
    updateProxy();
}
