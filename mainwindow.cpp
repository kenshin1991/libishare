#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtNetwork>
#include <QByteArray>

#define SERVER "https://www.google.com/youtube/accounts/ClientLogin"

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
    data.clear();
    data.append(QString("Email=" + ui->username->text() + "&Passwd="+ ui->password->text()+ "&service=youtube&source=Test"));

    //Work out stuff here to Upload video n stuff
    QNetworkAccessManager *networkManager =new QNetworkAccessManager();
    QUrl url(ui->url->text());

    QNetworkRequest request;
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");

    request.setUrl(url);
    QNetworkReply *r = networkManager->post(request, data);

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

    connect(r, SIGNAL(downloadProgress(qint64,qint64)),
            SLOT(slotSetProgress(qint64,qint64)));

}

void MainWindow::downloadFinished()
{
    QNetworkReply *reply =((QNetworkReply *)sender());
    QByteArray data = reply->readAll();
    ui->plainTextEdit->appendPlainText(data);
    ui->progressBar->setValue(0);
}

void MainWindow::slotSetProgress(qint64 received, qint64 total)
{
    if (total > 0 && received > 0) {
        ui->progressBar->setValue((int) total / received * 100);
    }
}
