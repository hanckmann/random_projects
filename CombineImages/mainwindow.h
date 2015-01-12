#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QStringList>
#include <QString>
#include <QList>
#include <QImage>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void sortFiles(const QStringList&);


    void combine(const QStringList& files);
    void combineImages();
    QImage combineImage(const QString&, const QString&);


    void split(const QStringList& files);
    void splitImages();
    QImage splitImage(const QString& file1, const int& part);

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    QList<int> width_list;
    QList<QString> file_list;
};

#endif // MAINWINDOW_H
