#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QStringList>
#include <QString>
#include <QList>
#include <QImage>
#include <QDir>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QList<QString> sortFiles(const QStringList&);
    void combineImages(QList<QString>&);
    QImage mergeImages(const QString&, const QString&);



private slots:
    void on_buttonSetTargetFolder_clicked();
    void on_buttonSelectPictures_clicked();

private:
    Ui::MainWindow *ui;

    QDir target_folder;
};

#endif // MAINWINDOW_H
