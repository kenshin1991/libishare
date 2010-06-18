#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    y.setCredentials(ui->username->text(), ui->password->text());
    y.setDeveloperKey(devKey);
    y.authenticate();

    connect(&y, SIGNAL(authOK()), this, SLOT(finished()));
    //connect(&y, SIGNAL(error(QString)), this, SLOT(error(QString)) );
}

void MainWindow::finished()
{
    //ui->plainTextEdit->appendPlainText(y.getYouTubeAuthString());
    //ui->plainTextEdit->appendPlainText(y.getYouTubeUser());

    /*On Finish, extract out the auth token and upload a test video */
    disconnect(&y, SIGNAL(authOK()), this, SLOT(finished()));
    qDebug() << y.upload("test.mp4"); /* YouTubeService will check if the auth token is expired
                             or it's not authenticated yet... Then if it's true, it will upload*/
}

void MainWindow::error(QString e)
{
    ui->status->setText("Error: " + e);
}
