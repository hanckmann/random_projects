#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <cmath>

#include <QStringListIterator>
#include <QFileDialog>
#include <QFileInfo>
#include <QString>
#include <QSize>
#include <QMatrix>
#include <QPainter>

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

void MainWindow::on_btnLoad_clicked()
{
    files = QFileDialog::getOpenFileNames(this,
                                          "Open images",
                                          "./",
                                          "Image Files (*.png *.jpg *.bmp *.JPG)");
    if(files.size() > 0) {
        ui->btnDo->setEnabled(true);
        ui->progressBar->setValue(0);
    }
}


void MainWindow::on_btnDo_clicked()
{
    if((ui->sbScale->value() == 100) && (ui->rbScale->isChecked())) {
        ui->progressBar->setMaximum(1);
        ui->progressBar->setValue(1);
        return;
    }

    float scale = (float)ui->sbScale->value() / 100.0;

    ui->progressBar->setMaximum(files.size());
    //QStringListIterator iter(files);
    //while (iter.hasNext()) {
    for (int i = 0; i < files.size(); ++i) {
        ui->progressBar->setValue(ui->progressBar->value()+1);
        //QString file = iter.next();
        QString file = files.at(i);
        QImage img;
        QImage img2;
        img.load(file);

        // Change scale on constant area
        if (ui->rbHeight->isChecked()) {
            scale = (float)ui->sbHeight->value() / img.size().height();
        }
        if (ui->rbWidth->isChecked()) {
            scale = (float)ui->sbWidth->value() / img.size().width();
        }

        // Resize
        int width  = qRound((float)(img.size().width() * scale));
        int height = qRound((float)(img.size().height() * scale));
        QSize newSize(width, height);
        img2 = img.scaled(newSize);

        QFileInfo info(file);
        QString newname;
        newname = info.dir().absolutePath() + "/scaled_" + info.fileName();
        ui->listWidget->insertItem(0, newname);
        img2.save(newname);
    }
}
