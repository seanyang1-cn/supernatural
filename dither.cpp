#include "dither.h"
#include <glut.h>
#include <mainwindow.h>
#include <freeglut.h>
#include<iostream>
#include<QMessageBox>
#include <QTextStream>
#include<QDebug>


#define HEIGHT 512
#define WIDTH 512
#define COLOR_RGB 3
#define CYCLE_COUNT 8
#define SCREEN_H 1024
#define SCREEN_W 1024
#define SCREEN_POS_X 0
#define SCREEN_POS_Y 0
#define FileName "White Tracking Tuning"
#define Dither_Matrix_COUNT 32

static unsigned char *dither_RGB ;
static unsigned char *outputBuffer;
static double *Refresh;
extern int screenWidth;
extern int screenHeight;
const int frame[8][2] = {{0, 15}, {1, 14}, {2, 13}, {3, 12}, {4, 11}, {5, 10}, {6, 9}, {7, 8}};
Dither::Dither(QWidget *parent) :
    QWidget(parent,Qt::Widget)
{
    R_index = (int*)calloc(HEIGHT*WIDTH, sizeof(int));
    G_index = (int*)calloc(HEIGHT*WIDTH, sizeof(int));
    B_index = (int*)calloc(HEIGHT*WIDTH, sizeof(int));
    R_out = (int*)calloc(HEIGHT*WIDTH, sizeof(int));
    G_out = (int*)calloc(HEIGHT*WIDTH, sizeof(int));
    B_out = (int*)calloc(HEIGHT*WIDTH, sizeof(int));
    Refresh = (double*)calloc(1, sizeof(double));
    Refresh[0] = 1000.0/60.0;//获取Refresh的值 默认60帧
    dither_RGB = (unsigned char*)calloc(HEIGHT*WIDTH*COLOR_RGB*CYCLE_COUNT, sizeof(unsigned char));
    outputBuffer = (unsigned char*)calloc(HEIGHT*WIDTH*COLOR_RGB, sizeof(unsigned char));
}

Dither::~Dither()
{
    free(R_index);
    free(G_index);
    free(B_index);
    free(R_out);
    free(G_out);
    free(B_out);
    free(outputBuffer);
    free(dither_RGB);
    free(Refresh);
}

void Dither::autoDither(double R_12bit, double G_12bit, double B_12bit)
{


    double startTime=clock();
    //    mutex.lock();
    int R_cycle[2][2] = {{0, 1}, {3, 2}};
    int G_cycle[2][2] = {{1, 3}, {0, 2}};
    int B_cycle[2][2] = {{2, 0}, {3, 1}};

    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            R_index[i*WIDTH+j] = R_12bit - int(R_12bit/16)*16;      // 读取数据的低四位,double
            G_index[i*WIDTH+j] = G_12bit - int(G_12bit/16)*16;
            B_index[i*WIDTH+j] = B_12bit - int(B_12bit/16)*16;

            R_out[i*WIDTH+j] = floor(R_12bit - R_index[i*WIDTH+j]);      // 读取数据的高八位,向下取整
            G_out[i*WIDTH+j] = floor(G_12bit - G_index[i*WIDTH+j]);
            B_out[i*WIDTH+j] = floor(B_12bit - B_index[i*WIDTH+j]);

            //            R_out[i*WIDTH+j] = floor(fLUTCoff_12bit/16);      // 读取数据的高八位,向下取整
            //            G_out[i*WIDTH+j] = floor(fLUTCoff_12bit/16);
            //            B_out[i*WIDTH+j] = floor(fLUTCoff_12bit/16);

            R_index[i*WIDTH+j] = floor(R_index[i*WIDTH+j]);     // 读取数据的低四位,向下取整
            G_index[i*WIDTH+j] = floor(G_index[i*WIDTH+j]);
            B_index[i*WIDTH+j] = floor(B_index[i*WIDTH+j]);
        }
    }
    double endTime=clock();
    double time=(endTime-startTime)/1000;
    //    ui->textEdit_demo->setText(QString::number(time)+"\n");

    int length = WIDTH * HEIGHT * COLOR_RGB;
    for (int k = 0; k < CYCLE_COUNT; k++)
    {
        for (int i = 0; i < HEIGHT; i++)
        {
            for (int j = 0; j < WIDTH; j++)
            {
                int a = (i+1)%4;      // 判断输入数据在DitherMatrix的行位置
                int b = (j+1)%4;      // 判断输入数据在DitherMatrix的列位置
                if(a==0)
                {
                    a = 4;
                }
                if(b==0)
                {
                    b = 4;
                }
                int a1 = (i+1)%8;       // 判断输入数据的属于的帧
                int b1 = (j+1)%8;       // 判断输入数据的属于的帧
                if(a1>0 && a1<=4)
                {
                    a1 = 0;
                }
                else
                {
                    a1 = 1;
                }
                if(b1>0 && b1<=4)
                {
                    b1 = 0;
                }
                else
                {
                    b1 = 1;
                }

                int pixIndex = (i*WIDTH+j);
                int subpixIndex = pixIndex * COLOR_RGB;
                double M_R = 0;
                double M_G = 0;
                double M_B = 0;

                // R通道低四位索引Dither Matrix矩阵
                if(R_index[pixIndex]>=1 && R_index[pixIndex]<=7)
                {
                    M_R = matrixValue[int(R_index[pixIndex]-1)*4+a-1][R_cycle[a1][b1]*4+b-1];
                }
                else if(R_index[pixIndex]<1)
                {
                    dither_RGB[subpixIndex+0+k*length] = (unsigned char)(R_out[pixIndex]/16);
                }
                else
                {
                    for (int t = 0; t < CYCLE_COUNT; t++)
                    {
                        if(int(R_index[pixIndex])==frame[t][1])
                        {
                            M_R = 1 -  matrixValue[frame[t][0]*4+a-1][R_cycle[a1][b1]*4+b-1];
                            break;
                        }
                    }
                }
                // R通道Dither补偿
                if (M_R > 0.5)
                {
                    dither_RGB[subpixIndex+0+k*length] = (unsigned char)((R_out[pixIndex] + 16)/16);

                }
                else
                {
                    dither_RGB[subpixIndex+0+k*length] = (unsigned char)(R_out[pixIndex]/16);
                }

                //                qDebug()<<"dither_RGB[subpixIndex+0+k*length]"<<dither_RGB[subpixIndex+0+k*length]*16;        // 调试打印数据

                // G通道低四位索引Dither Matrix矩阵
                if(G_index[pixIndex]>=1 && G_index[pixIndex]<=7)
                {
                    M_G = matrixValue[int(G_index[pixIndex]-1)*4+a-1][G_cycle[a1][b1]*4+b-1];
                }
                else if (G_index[pixIndex]<1)
                {
                    dither_RGB[subpixIndex+1+k*length] = (unsigned char)(G_out[pixIndex]/16);
                }
                else
                {
                    for (int t = 0; t < CYCLE_COUNT; t++)
                    {
                        if(int(G_index[pixIndex])==frame[t][1])
                        {
                            M_G = 1 - matrixValue[frame[t][0]*4+a-1][G_cycle[a1][b1]*4+b-1];
                            break;
                        }

                    }

                }
                // G通道补偿
                if (M_G > 0.5)
                {
                    dither_RGB[subpixIndex+1+k*length] = (unsigned char)((G_out[pixIndex] + 16)/16);
                }
                else
                {
                    dither_RGB[subpixIndex+1+k*length] = (unsigned char)(G_out[pixIndex]/16);
                }

                //                qDebug()<<"dither_RGB[subpixIndex+1+k*length]"<<dither_RGB[subpixIndex+1+k*length]*16;        // 调试打印数据

                // B通道低四位索引Dither Matrix矩阵
                if(B_index[pixIndex]>=1 && B_index[pixIndex]<=7)
                {
                    M_B = matrixValue[int(B_index[pixIndex]-1)*4+a-1][B_cycle[a1][b1]*4+b-1];
                }
                else if (B_index[pixIndex]<1)
                {
                    dither_RGB[subpixIndex+2+k*length] = (unsigned char)(B_out[pixIndex]/16);
                }
                else
                {
                    for (int t = 0; t < CYCLE_COUNT; t++)
                    {
                        if(int(B_index[pixIndex])==frame[t][1])
                        {
                            M_B = 1 - matrixValue[frame[t][0]*4+a-1][B_cycle[a1][b1]*4+b-1];
                            break;
                        }

                    }

                }
                //  B通道补偿
                if(M_B > 0.5)
                {
                    dither_RGB[subpixIndex+2+k*length] = (unsigned char)((B_out[pixIndex] + 16)/16);
                }
                else
                {
                    dither_RGB[subpixIndex+2+k*length] = (unsigned char)(B_out[pixIndex]/16);
                }
                //                qDebug()<<"dither_RGB[subpixIndex+2+k*length]"<<dither_RGB[subpixIndex+2+k*length]*16;        // 调试打印数据
            }
        }


        // 帧循环
        for (int y = 0; y < 2; y++)
        {
            for (int x = 0; x < 2; x++)
            {
                // R通道帧循环
                if (R_cycle[y][x] < 7)
                {
                    R_cycle[y][x] = R_cycle[y][x] + 1;
                }
                else
                {
                    R_cycle[y][x] = R_cycle[y][x] - 7;
                }
                // G通道帧循环
                if (G_cycle[y][x] < 7)
                {
                    G_cycle[y][x] = G_cycle[y][x] + 1;
                }
                else
                {
                    G_cycle[y][x] = G_cycle[y][x] - 7;
                }
                // B通道帧循环
                if (B_cycle[y][x] < 7)
                {
                    B_cycle[y][x] = B_cycle[y][x] + 1;
                }
                else
                {
                    B_cycle[y][x] = B_cycle[y][x] - 7;
                }

            }

        }

    }
    double endTime1=clock();
    double time1=(endTime1-startTime)/1000;
    //    ui->textEdit_demo->setText(QString::number(time)+"    "+QString::number(time1));
}

//加载matrix
void Dither::getDitherMatrix(double matrix[32][32])
{
    for(int i = 0;i < 32; i++){
        for(int j = 0 ; j<32;j++){
            matrixValue[i][j] = matrix[i][j];
        }
    }
}

void Dither::ditherRun()
{
    if (Isplay == false) {
        Isplay = true;
        int argc = 0;
        char **argv;
        glutInit(&argc, argv);
        glutInitDisplayMode(
            GLUT_DOUBLE
            | GLUT_RGB); //GLUT_DOUBLR:一次存两帧，存一个播放一个；GLUT_RGB：播放格式是RGB；
        glutInitWindowPosition(SCREEN_POS_X, SCREEN_POS_Y); //播放视频的位置
        glutInitWindowSize(screenWidth, screenHeight); // 播放窗口尺寸

        glutCreateWindow(FileName);
        // get from ui;
        double R = 2448;
        double G = 3472;
        double B = 3744;
        qDebug() << "success";
        autoDither(R, G, B);
        displayLoop();
    } else {
        glutShowWindow();
    }
}

int count=0;
void display()
{

    //    mutex.lock();
    double t1=clock();
    if(count > 7)
    {
        count = 0;
    }
    size_t size = WIDTH*HEIGHT*COLOR_RGB*sizeof(unsigned char);
    //    qDebug()<<dither_RGB;
    memcpy(outputBuffer,&dither_RGB[WIDTH*HEIGHT*COLOR_RGB * count], size);
    count++;
    //    qDebug()<<"缓存[4]的值:"<<outputBuffer[4];
    //  示例1， 通过导入.rgb格式文件查看效果
    //    if (fread(outputBuffer, 1, 320*180*COLOR_RGB, fp) != 320*180*COLOR_RGB)
    //    {    qDebug()<<"test1";
    //        // Loop
    //        fseek(fp, 0, SEEK_SET);
    //          qDebug()<<"test2";
    //        fread(outputBuffer, 1, 320*180*COLOR_RGB, fp);
    //     }

    //display
    glRasterPos3f(-1.0,1.0,0.0);
    glPixelZoom(((1.0*screenWidth)/WIDTH), ((-1.0*screenHeight)/HEIGHT));
    glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE , outputBuffer);
    double t2=clock();
    double t=(t2-t1)/1000;

    glutSwapBuffers();
    //     qDebug()<<"单次显示用时:"<<t<<"   "<<clock();
    //    mutex.unlock();
}

void timeFunc(int)
{
    glutTimerFunc(Refresh[0], timeFunc, 0);
    display();
}

void Dither::displayLoop()
{
    glutDisplayFunc(&display);
    //    LONG style;HWND hWnd;
    //        //////////////////////////////////////
    //    QString name=QString::fromLocal8Bit(FileName);
    //    hWnd = FindWindow(NULL,(LPCWSTR)name.unicode());
    //    if(!hWnd)
    //        qDebug()<<"NO!"<<endl;

    //    style = GetWindowLong(hWnd, GWL_STYLE);
    //    style = style & (~WS_CAPTION) & ~(WS_BORDER) & ~WS_THICKFRAME ;
    //    SetWindowLong( hWnd, GWL_STYLE, style);   //无边框无标题
    glutTimerFunc(Refresh[0], timeFunc, 0);

    // 设置窗口关闭后继续执行程序，而不是exit直接退出了程序
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    glutMainLoop();//进入循环，播放视频
}

void Dither::loadDitherMatrix(QString fileName_dither_matrix)
{
    QFile file(fileName_dither_matrix);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        std::cerr << "Cannot open file for reading: "
                  << qPrintable(file.errorString()) << std::endl;
        return;
    }

    QStringList fileLine;
    fileLine.clear();
    QTextStream in(&file);

    //CSV解析Dither Matrix
    for(int i=0; i < Dither_Matrix_COUNT ; i++)
    {
        fileLine = in.readLine().split(',');
        int colCount=fileLine.count();
        if(colCount != Dither_Matrix_COUNT)
        {
            QMessageBox::information(this, tr("Error"), tr("数据格式错误，Dither Matrix Size：32x32，请检查后重新导入"));
            return;
        }
        for(int j=0;j<Dither_Matrix_COUNT;j++)
        {
            matrixValue[i][j] = fileLine[j].toInt();
        }
    }

    file.close();
    qDebug()<<fileName_dither_matrix;

}

void Dither::getDitherMatrix()
{
    QFile file("./Dither_matrix.csv");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        std::cerr << "Cannot open file for reading: "
                  << qPrintable(file.errorString()) << std::endl;
        return;
    }

    QStringList fileLine;
    fileLine.clear();
    QTextStream in(&file);

    //CSV解析Dither Matrix
    for(int i=0; i < Dither_Matrix_COUNT ; i++)
    {
        fileLine = in.readLine().split(',');
        int colCount=fileLine.count();
        if(colCount != Dither_Matrix_COUNT)
        {
            QMessageBox::information(this, tr("Error"), tr("数据格式错误，Dither Matrix Size：32x32，请检查后重新导入"));
            return;
        }
        for(int j=0;j<Dither_Matrix_COUNT;j++)
        {
            matrixValue[i][j] = fileLine[j].toInt();
        }
    }

    file.close();
    qDebug()<<"dither matrix loading success!";
}

void Dither::setDitherPixmap(int R, int G, int B)
{
    double R_d=R;
    double G_d=G;
    double B_d=B;
    autoDither(R_d,G_d,B_d);
}

void Dither::close()
{
    if (Isplay == true) {
        glutDestroyWindow(glutGetWindow());
    }
}

void Dither::hide()
{
    glutHideWindow();
}

double Dither::valueCal(double value, double ratio)
{
    double val_output = round(value*16*ratio/4095);
    return val_output;
}

void Dither::fullscreen()
{
    glutFullScreen();
}

void Dither::fullscreenEnd()
{
    glutReshapeWindow(screenWidth, screenHeight);
    glutPositionWindow(0,0);

}
