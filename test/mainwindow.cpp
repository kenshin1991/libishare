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
    y = NULL;
    devKey = "AI39si7FOtp165Vq644xVkuka84TVQNbztQmQ1dC9stheBfh3-33RZaTu7eJkYJzvxp6XNbvlr4M6-ULjXDERFl62WIo6AQIEQ";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName( this,
                                                 tr( "Choose Video File to upload" ), "",
                                                 tr( "Video files (*.avi *.flv *.mov *.mp4 *.mpg)" ) );

    /* Add code to transcode/export video */
    ui->filePath->setText( fileName );
    shareOnInternet();
}

void MainWindow::shareOnInternet()
{
    QString filePath = ui->filePath->text();
    ShareOnInternet *exportToInternet = new ShareOnInternet();

    if( y )
        exportToInternet->setData( y->getVideoData() );

    if ( exportToInternet->exec() == QDialog::Rejected )
    {
        if( y )
        {
            qDebug() << "Dialog cancelled, deleting y";
            y->deleteLater();
            y = NULL;
        }

        delete exportToInternet;
        return ;
    }

    QString username     = exportToInternet->username();
    QString password     = exportToInternet->password();
    quint32 width        = exportToInternet->width();
    quint32 height       = exportToInternet->height();
    VideoData videoData  = exportToInternet->videoData();

    delete exportToInternet;

    uploadVideo( username, password, filePath, videoData );
}

void MainWindow::uploadVideo( QString& username, QString& password,
                              QString& fileName, VideoData& videoData )
{
    qDebug() << username << password
            << fileName << videoData.title;

    if( !y )
    {
        qDebug() << "creating y object";
        y = new YouTubeService( devKey, username, password );
    }
    else
        y->setCredentials( username, password );

    y->setVideoParameters( fileName, videoData );
    y->authenticate();

    connect( y, SIGNAL(authOver()), this, SLOT(authFinished()) );
    connect( y, SIGNAL(uploadOver(QString)), this, SLOT(uploadFinished(QString)));
    connect( y, SIGNAL(uploadProgress(qint64,qint64)),
             this, SLOT(videoUploadProgress(qint64,qint64)) );
    connect( y, SIGNAL(error(QString)), this, SLOT(error(QString)) );
}


void MainWindow::authFinished()
{
    qDebug() << "[AUTH FINISHED]";

    /*On Finish, extract out the auth token and upload a test video */
    //disconnect( y, SIGNAL(authOver()), this, SLOT(authFinished()) );

    if( !y->upload() )
    {
        qDebug() << "[AUTH FAILED]";
        shareOnInternet();
        /* Add code here to work on fallback... */

    }
    qDebug() << "[UPLOAD STARTED]";
    ui->abortButton->setEnabled(true);
    /* YouTubeService will check if the auth token is expired
                   or it's not authenticated yet... Then if it's true, it will upload*/
}

void MainWindow::uploadFinished( QString result )
{
    ui->abortButton->setEnabled(false);
    qDebug() << "[Upload Finished]";;
    ui->log->appendPlainText(result);
    y->deleteLater();
}

void MainWindow::videoUploadProgress(qint64 received, qint64 total)
{
    if( total != 0 )
    {
        qint64 progress = received * 100 / total;
        ui->progressBar->setValue( progress );
        ui->progress->setText( QString("Bytes Uploaded: %1").arg( received ) );
    }
}

void MainWindow::error(QString e)
{
    ui->log->appendPlainText("[Error]: " + e);
}

void MainWindow::on_abortButton_clicked()
{
    y->abort();
    y->deleteLater();
    ui->log->appendPlainText("[UPLOAD ABORTED]");
}
