#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "YouTube/YouTubeService.h"
class QByteArray;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;
    YouTubeService* y;
    QString devKey;

private slots:
    void on_actionShare_on_Internet_triggered();
    void on_pushButton_clicked();

    void authFinished();
    void uploadVideo( QString& fileName );
    void uploadFinished(QString);
    void error(QString);
    void videoUploadProgress(qint64 received, qint64 total);
};

#endif // MAINWINDOW_H
