#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Gui/ShareOnInternet.h"

#include <QDebug>

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
}

void MainWindow::on_actionShare_on_Internet_triggered()
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

    /* Add code to transcode/export video */

    y = new YouTubeService( devKey, username, password );
    y->setVideoParameters( fileName, title, description, category, keywords, videoPrivacy );
    y->authenticate();

    connect( y, SIGNAL(authOK()), this, SLOT(authFinished()) );
    connect( y, SIGNAL(uploadOK(QString)), this, SLOT(uploadFinished(QString)));
    connect( y, SIGNAL(uploadProgress(qint64,qint64)),
             this, SLOT(videoUploadProgress(qint64,qint64)) );
    connect( y, SIGNAL(error(QString)), this, SLOT(error(QString)) );
}


void MainWindow::authFinished()
{
    /*On Finish, extract out the auth token and upload a test video */
    disconnect( y, SIGNAL(authOK()), this, SLOT(uploadVideo()) );
    y->upload(); /* YouTubeService will check if the auth token is expired
                   or it's not authenticated yet... Then if it's true, it will upload*/
}

void MainWindow::uploadFinished( QString result )
{
    disconnect( y, SIGNAL(uploadOK(QString)), this, SLOT(uploadFinished(QString)));
    disconnect( y, SIGNAL(uploadProgress(qint64,qint64)),
                this, SLOT(videoUploadProgress(qint64,qint64)) );
    disconnect( y, SIGNAL(error(QString)), this, SLOT(error(QString)) );

    ui->log->appendPlainText(result);

}

void MainWindow::videoUploadProgress(qint64 received, qint64 total)
{
    ui->progressBar->setValue( received * 100 / total );
}

void MainWindow::error(QString e)
{
    ui->log->appendPlainText("Error: " + e);
}
