#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <cmath>

#include <QFileDialog>
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
    sortFiles(files);
    combineImages();
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
            str = "Combined " + file1 + " and " + file2 + " to " + file3;
        } else {
            str = "There was 1 file unprocessed: " + file_list.first();
        }
        ui->label->setText(str);
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
