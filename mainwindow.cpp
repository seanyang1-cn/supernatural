#include "mainwindow.h"
#include "ui_mainwindow.h"
int screenWidth = 512;
int screenHeight= 512;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    paraInit();
    dither = new Dither;
}

void MainWindow::paraInit()
{
    screenWidth = ui->lineEdit_screenWidth->text().toInt();
    screenHeight = ui->lineEdit_screenHeight->text().toInt();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete dither;
}

void MainWindow::on_pushButton_play_clicked()
{
    paraInit();
    dither->getDitherMatrix();
    if(ui->pushButton_play->text()=="PLAY"){
        ui->pushButton_play->setText("HIDE");
        dither->ditherRun();
    }
    else{
        dither->hide();
        ui->pushButton_play->setText("PLAY");
    }

}




void MainWindow::on_spinBox_R_valueChanged(int arg1)
{
    ui->horizontalSlider_R->setValue(arg1);
    double R = ui->horizontalSlider_R->value();
    double G = ui->horizontalSlider_G->value();
    double B = ui->horizontalSlider_B->value();
    double R_ratio = ui->spinBox_ratioR->value();
    double G_ratio = ui->spinBox_ratioG->value();
    double B_ratio = ui->spinBox_ratioB->value();
    double R_out = dither->valueCal(R,R_ratio);
    double G_out = dither->valueCal(G,G_ratio);
    double B_out = dither->valueCal(B,B_ratio);
    dither->autoDither(R_out,G_out,B_out);
}


void MainWindow::on_spinBox_G_valueChanged(int arg1)
{
    ui->horizontalSlider_G->setValue(arg1);
    double R = ui->horizontalSlider_R->value();
    double G = ui->horizontalSlider_G->value();
    double B = ui->horizontalSlider_B->value();
    double R_ratio = ui->spinBox_ratioR->value();
    double G_ratio = ui->spinBox_ratioG->value();
    double B_ratio = ui->spinBox_ratioB->value();
    double R_out = dither->valueCal(R,R_ratio);
    double G_out = dither->valueCal(G,G_ratio);
    double B_out = dither->valueCal(B,B_ratio);
    dither->autoDither(R_out,G_out,B_out);

}


void MainWindow::on_spinBox_B_valueChanged(int arg1)
{
    ui->horizontalSlider_B->setValue(arg1);
    double R = ui->horizontalSlider_R->value();
    double G = ui->horizontalSlider_G->value();
    double B = ui->horizontalSlider_B->value();
    double R_ratio = ui->spinBox_ratioR->value();
    double G_ratio = ui->spinBox_ratioG->value();
    double B_ratio = ui->spinBox_ratioB->value();
    double R_out = dither->valueCal(R,R_ratio);
    double G_out = dither->valueCal(G,G_ratio);
    double B_out = dither->valueCal(B,B_ratio);
    dither->autoDither(R_out,G_out,B_out);

}


void MainWindow::on_horizontalSlider_R_valueChanged(int value)
{
    ui->spinBox_R->setValue(value);
}


void MainWindow::on_horizontalSlider_G_valueChanged(int value)
{
    ui->spinBox_G->setValue(value);
}


void MainWindow::on_horizontalSlider_B_valueChanged(int value)
{
    ui->spinBox_B->setValue(value);
}


void MainWindow::on_spinBox_ratioR_valueChanged(int arg1)
{
    double R = ui->horizontalSlider_R->value();
    double G = ui->horizontalSlider_G->value();
    double B = ui->horizontalSlider_B->value();
    double R_ratio = ui->spinBox_ratioR->value();
    double G_ratio = ui->spinBox_ratioG->value();
    double B_ratio = ui->spinBox_ratioB->value();
    double R_out = dither->valueCal(R,R_ratio);
    double G_out = dither->valueCal(G,G_ratio);
    double B_out = dither->valueCal(B,B_ratio);
    dither->autoDither(R_out,G_out,B_out);
}


void MainWindow::on_spinBox_ratioG_valueChanged(int arg1)
{
    double R = ui->horizontalSlider_R->value();
    double G = ui->horizontalSlider_G->value();
    double B = ui->horizontalSlider_B->value();
    double R_ratio = ui->spinBox_ratioR->value();
    double G_ratio = ui->spinBox_ratioG->value();
    double B_ratio = ui->spinBox_ratioB->value();
    double R_out = dither->valueCal(R,R_ratio);
    double G_out = dither->valueCal(G,G_ratio);
    double B_out = dither->valueCal(B,B_ratio);
    dither->autoDither(R_out,G_out,B_out);
}


void MainWindow::on_spinBox_ratioB_valueChanged(int arg1)
{
    double R = ui->horizontalSlider_R->value();
    double G = ui->horizontalSlider_G->value();
    double B = ui->horizontalSlider_B->value();
    double R_ratio = ui->spinBox_ratioR->value();
    double G_ratio = ui->spinBox_ratioG->value();
    double B_ratio = ui->spinBox_ratioB->value();
    double R_out = dither->valueCal(R,R_ratio);
    double G_out = dither->valueCal(G,G_ratio);
    double B_out = dither->valueCal(B,B_ratio);
    dither->autoDither(R_out,G_out,B_out);
}


void MainWindow::on_horizontalSlider_ratioR_valueChanged(int value)
{
    ui->spinBox_ratioR->setValue(value);
}


void MainWindow::on_horizontalSlider_ratioG_valueChanged(int value)
{
    ui->spinBox_ratioG->setValue(value);
}


void MainWindow::on_horizontalSlider_ratioB_valueChanged(int value)
{
    ui->spinBox_ratioB->setValue(value);
}


void MainWindow::on_lineEdit_screenWidth_textChanged(const QString &arg1)
{
    int width = ui->lineEdit_screenWidth->text().toInt();
    if(width>=3840){
        width =3840;
    }
    int height = ui->lineEdit_screenHeight->text().toInt();
    if(height>=2160){
        height =2160;
    }
    screenWidth = width;
    screenHeight = height;
}


void MainWindow::on_pushButton_fullscreen_clicked()
{
    dither->fullscreen();
}


void MainWindow::on_pushButton_fullscreen_2_clicked()
{
    dither->fullscreenEnd();
}

