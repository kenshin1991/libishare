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
    devKey = "AI39si7FOtp165Vq644xVkuka84TVQNbztQmQ1dC9stheBfh3-33RZaTu7eJkYJzvxp6XNbvlr4M6-ULjXDERFl62WIo6AQIEQ";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::on_pushButton_clicked()
{
    QString path = QFileDialog::getOpenFileName( this,
                                                 tr( "Choose Video File to upload" ),
                                                 "",
                                                 tr( "Video files (*.avi *.mov *.mp4 *.ogg *.*)" ) );

    ui->filePath->setText(path);
    uploadVideo( path );
}

void MainWindow::on_actionShare_on_Internet_triggered()
{
    /* No path checking is done, so handle it carefully */
    QString path = ui->filePath->text();
    uploadVideo( path );
}

void MainWindow::uploadVideo( QString& fileName )
{
    ShareOnInternet *exportToInternet = new ShareOnInternet;

    if ( exportToInternet->exec() == QDialog::Rejected )
    {
        delete exportToInternet;
        return ;
    }

    QString username     = exportToInternet->username();
    QString password     = exportToInternet->password();
    QString title        = exportToInternet->title();
    QString category     = exportToInternet->category();
    QString description  = exportToInternet->description();
    QString keywords     = exportToInternet->keywords();
    quint32 width        = exportToInternet->width();
    quint32 height       = exportToInternet->height();
    bool    videoPrivacy = exportToInternet->videoPrivacy();

    delete exportToInternet;

    //qDebug() << username << password
    //         << title << category << description << keywords;

    /* Add code to transcode/export video */

    y = new YouTubeService( devKey, username, password );
    y->setVideoParameters( fileName, title, description, category, keywords, videoPrivacy );
    y->authenticate();

    connect( y, SIGNAL(authOK()), this, SLOT(authFinished()) );
    connect( y, SIGNAL(uploadOK(QString)), this, SLOT(uploadFinished(QString)));
    connect( y, SIGNAL(uploadProgress(qint64,qint64)),
             this, SLOT(videoUploadProgress(qint64,qint64)) );
    connect( y, SIGNAL(serviceError(QString)), this, SLOT(error(QString)) );
}


void MainWindow::authFinished()
{
    /*On Finish, extract out the auth token and upload a test video */
    disconnect( y, SIGNAL(authOK()), this, SLOT(authFinished()) );

    qDebug() << "[UPLOAD SERVICE]: START";
    y->upload();

    /* YouTubeService will check if the auth token is expired
                   or it's not authenticated yet... Then if it's true, it will upload*/
}

void MainWindow::uploadFinished( QString result )
{
    qDebug() << "[TEST APP]: Upload Finished";

    //disconnect( y, SIGNAL(uploadOK(QString)), this, SLOT(uploadFinished(QString)));
    //disconnect( y, SIGNAL(uploadProgress(qint64,qint64)),
    //            this, SLOT(videoUploadProgress(qint64,qint64)) );
    //disconnect( y, SIGNAL(serviceError(QString)), this, SLOT(error(QString)) );

    ui->log->appendPlainText(result);

    if(y)
        delete y;
}

void MainWindow::videoUploadProgress(qint64 received, qint64 total)
{
    qint64 progress = received * 100 / total;
    ui->progressBar->setValue( progress );
}

void MainWindow::error(QString e)
{
    ui->log->appendPlainText("[Error]: " + e);
}
