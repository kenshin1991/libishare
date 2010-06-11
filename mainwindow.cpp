#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtNetwork>

#define SERVER "http://doc.trolltech.com/qtjambi-4.4/html/com/trolltech/qt/network/QNetworkAccessManager.html"

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
    //Work out stuff here to Upload video n stuff
    QNetworkAccessManager *networkManager =new QNetworkAccessManager();
    QUrl url(SERVER);

    QNetworkRequest request(url);
    QNetworkReply *r = networkManager->get(request);

    //QFile certFile(":/secure.example.com.crt");
    //Q_ASSERT(certFile.open(QIODevice::ReadOnly));
    //QSslCertificate cert(&certFile, QSsl::Pem);
    //QSslSocket * sslSocket = new QSslSocket(this);
    //sslSocket->addCaCertificate(cert);
    //QSslConfiguration config = sslSocket->sslConfiguration();
    //config.setProtocol(QSsl::TlsV1);
    //sslSocket->setSslConfiguration(config);
    //r->setSslConfiguration(config);
    connect(r, SIGNAL(finished()),this,SLOT(downloadFinished()));
}

void MainWindow::downloadFinished()
{
    QNetworkReply *reply =((QNetworkReply *)sender());
    QByteArray data = reply->readAll();
    ui->plainTextEdit->appendPlainText(data);
}
