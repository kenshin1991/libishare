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

private:
    Ui::MainWindow *ui;
    QString fileName;
    void shareOnInternet();

private slots:
    void on_proxy_clicked();
    void on_pushButton_clicked();
    void updateProxy();
};

#endif // MAINWINDOW_H
