#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    devKey = "AI39si7FOtp165Vq644xVkuka84TVQNbztQmQ1dC9stheBfh3-33RZaTu7eJkYJzvxp6XNbvlr4M6-ULjXDERFl62WIo6AQIEQ";
    y.setDeveloperKey(devKey);
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
    y.authenticate();

    connect(&y, SIGNAL(authenticationFinished()), this, SLOT(finished()));
    connect(&y, SIGNAL(error(QString)), this, SLOT(error(QString)) );
}

void MainWindow::finished()
{
    Q_ASSERT_X( y.isAuthenticated() == true, "YT_AUTH...",
                    "err" );
    //ui->plainTextEdit->appendPlainText(y.getYouTubeAuthString());
    //ui->plainTextEdit->appendPlainText(y.getYouTubeUser());

    /*On Finish, extract out the auth token and upload a test video */
    y.upload("test.mp4"); /* YouTubeService will check if the auth token is expired 
                             or it's not authenticated yet... Then if it's true, it will upload*/
}

void MainWindow::error(QString)
{
    ui->status->setText("U-ERR=" + y.getYouTubeError() + ", NET= " + y.getNetworkError());
}
