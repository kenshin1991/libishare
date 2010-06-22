#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "YouTube/YouTubeService.h"

class QByteArray;

namespace Ui {
    class MainWindow;
}

class VideoData;
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    YouTubeService* y;
    QString devKey;

    void shareOnInternet();
    void uploadVideo( QString& username, QString& password,
                      QString& fileName, VideoData& videoData );

private slots:
    void on_abortButton_clicked();
    void on_pushButton_clicked();

    void authFinished();
    void uploadFinished(QString);
    void error(QString);
    void videoUploadProgress(qint64 received, qint64 total);
};

#endif // MAINWINDOW_H
