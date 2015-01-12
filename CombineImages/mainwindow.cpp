#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <cmath>

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

void MainWindow::on_pushButton_clicked()
{
    QStringList files = QFileDialog::getOpenFileNames(this,
                                                      "Open images",
                                                      "./",
                                                      "Image Files (*.png *.jpg *.bmp)");
    if(ui->rbCombine->isChecked())
        combine(files);
    else if(ui->rbSplit->isChecked())
        split(files);
    else
        ui->lineEditTargetFolder->setText("ERROR IN FUNCTION SELECTION!");

}

void MainWindow::combine(const QStringList& files)
{
    if(files.count() > 0)
    {
        // Show the target folder
        QFileInfo fileinfo(files.at(0));
        ui->lineEditTargetFolder->setText(fileinfo.absolutePath());
        int maxValue = files.count() + (int)floor(files.count()/2) - 1;
        ui->progressBar->setRange( 0, maxValue );
        ui->progressBar->setValue(0);

        sortFiles(files);
        combineImages();
    }
    else
    {
        ui->lineEditTargetFolder->setText("No valid files selected");
        ui->label->setText("No valid files selected");
    }
}

void MainWindow::sortFiles(const QStringList& files)
{
    QStringList lof = files;
    ui->label->setText("Checking file sizes.");
    while(lof.size() > 0) {
        QImage img(lof.first());
        int w = std::min(img.width(),img.height());
        int index = 0;
        while(index < width_list.size()) {
            if(w > width_list.at(index))
                break;
            ++index;
        }

        file_list.insert(index,lof.first());
        width_list.insert(index,w);
        lof.removeFirst();
        ui->progressBar->setValue(ui->progressBar->value()+1);
    }
    int i = 0;
}

void MainWindow::combineImages()
{
    int count = 1;
    while(file_list.size() > 2) {
        QString str;
        if(file_list.size() > 1) {
            QString file1 = file_list.first();
            file_list.removeFirst();
            width_list.removeFirst();
            QString file2 = file_list.first();
            file_list.removeFirst();
            width_list.removeFirst();
            QString file3;
            file3 = "combined_" + QString::number(count) + ".jpg" ;
            QImage img;
            img = combineImage(file1, file2);
            img.save(file3);
            QFileInfo fi1(file1);
            QFileInfo fi2(file2);
            QFileInfo fi3(file3);
            str = "Combined " + fi1.baseName() + " and " + fi2.baseName() + " to " + fi3.baseName();
        } else {
            str = "There was 1 file unprocessed: " + file_list.first();
        }
        ui->label->setText(str);
        ui->progressBar->setValue(ui->progressBar->value()+1);
        ++count;
    }
}

QImage MainWindow::combineImage(const QString& file1, const QString& file2)
{
    // Load images
    QImage img1(file1);
    QImage img2(file2);

    // Determine size
    QSize size1 = img1.size();
    QSize size2 = img2.size();
    // Determine if rotation is needed (and rotate)
    QMatrix matrix;
    matrix.scale(1,1);
    matrix.rotate(90);
    if (size1.width() > size1.height())
        img1 = img1.transformed(matrix);
    if (size2.width() > size2.height())
        img2 = img2.transformed(matrix);

    size1 = img1.size();
    size2 = img2.size();
    // Combine side-by-side
    QSize size3(size1.width()+size2.width(), std::max(size1.height(),size2.height()));
    QImage img3(size3,QImage::Format_ARGB32);

    QPainter paint(&img3);
    QRect rect(0,0,size3.width(),size3.height());
    paint.fillRect(rect,QColor("white"));

    paint.drawImage(0,0,img1);
    paint.drawImage(size1.width(),0,img2);

    paint.end();

    // return new image
    return img3;
}

void MainWindow::split(const QStringList& files)
{
    if(files.count() > 0)
    {
        // Show the target folder
        QFileInfo fileinfo(files.at(0));
        ui->lineEditTargetFolder->setText(fileinfo.absolutePath());
        int maxValue = files.count() + (int)floor(files.count()/2) - 1;
        ui->progressBar->setRange( 0, maxValue );
        ui->progressBar->setValue(0);


        QStringList lof = files;
        while(lof.size() > 0) {
            file_list.append(lof.first());
            lof.removeFirst();
            ui->progressBar->setValue(ui->progressBar->value()+1);
        }


        splitImages();
    }
    else
    {
        ui->lineEditTargetFolder->setText("No valid files selected");
        ui->label->setText("No valid files selected");
    }
}

void MainWindow::splitImages()
{
    int count = 1;
    while(file_list.size() > 0) {
        QString str;

        QString file1 = file_list.first();
        QFileInfo fileinfo(file1);
        file_list.removeFirst();
        QString file2;
        file2 = "split_1_" + fileinfo.baseName() + ".jpg" ;
        QString file3;
        file3 = "split_2_" + fileinfo.baseName() + ".jpg" ;
        QImage img1;
        QImage img2;
        img1 = splitImage(file1, 0);
        img2 = splitImage(file1, 1);
        img1.save(file2);
        img2.save(file3);
        QFileInfo fi1(file1);
        QFileInfo fi2(file2);
        QFileInfo fi3(file3);
        str = "Splitted " + fi1.baseName() + " into " + fi2.baseName() + " and " + fi3.baseName();

        ui->label->setText(str);
        ui->progressBar->setValue(ui->progressBar->value()+1);
        ++count;
    }
}

QImage MainWindow::splitImage(const QString& file1, const int& part)
{
    // Load images
    QImage img1(file1);

    // Determine size
    QSize size1 = img1.size();

    // Determine if rotation is needed (and rotate)
    QMatrix matrix;
    matrix.scale(1,1);
    matrix.rotate(90);
    if (size1.width() > size1.height())
        img1 = img1.transformed(matrix);
    size1 = img1.size();

    // Split
    int y = part * (int)std::ceil(img1.height() / 2);
    QImage img2;
    img2 = img1.copy(0,y,img1.width(),(int)std::ceil(size1.height() / 2));

    // Prepare new image and copy relevant part
    QSize size23(size1.width(), (int)std::ceil(size1.height() / 2));
    QImage img3(size23,QImage::Format_ARGB32);

    QPainter paint(&img3);
    QRect rect(0,0,size23.width(),size23.height());
    paint.fillRect(rect,QColor("white"));

    paint.drawImage(0,0,img2);

    paint.end();

    // return new image
    return img3;
}
