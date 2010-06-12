#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    QByteArray data;

private slots:
    void on_pushButton_clicked();
    void downloadFinished();
    void slotSetProgress(qint64 received, qint64 total);
};

#endif // MAINWINDOW_H
