#include "widget.h"
#include <QPushButton>
#include <QPalette>
//#include <QDebug>
//#include <QSize>

Widget::Widget(QWidget *parent) :
    QWidget(parent)
{
    setWindowTitle(tr("Painter"));
    this->setStyleSheet("background-color: grey");


    OK = new QPushButton(tr("init"),this);
    OK->setGeometry(Width*0.3, Height*0.85, 120, 50);
    OK->setText(tr("OK"));
    OK->setStyleSheet("QPushButton  {color: black;\
                      background-color: white;\
                      border-style: outset;\
                      border-width: 2px;\
                      border-color: beige;}");

    OK->setFont(QFont("consolas", 20));
    QObject::connect(OK,SIGNAL(clicked()),this,SLOT(OK_clicked()));

    Clear = new QPushButton(tr("init"),this);
    Clear->setGeometry(Width*0.1, Height*0.85, 120, 50);
    Clear->setText(tr("Clear"));
    Clear->setStyleSheet("QPushButton  {color: red;\
                         background-color: white;\
                         border-style: outset;\
                         border-width: 2px;\
                         border-color: beige;}");

    Clear->setFont(QFont("consolas", 20));
    QObject::connect(Clear,SIGNAL(clicked()),this,SLOT(Clear_clicked()));

    OCR = new QPushButton(tr("init"),this);
    OCR->setGeometry(Width*0.5, Height*0.85, 120, 50);
    OCR->setText(tr("OCR"));
    OCR->setStyleSheet("QPushButton  {color: blue;\
                         background-color: white;\
                         border-style: outset;\
                         border-width: 2px;\
                         border-color: beige;}");

    OCR->setFont(QFont("consolas", 20));
    QObject::connect(OCR,SIGNAL(clicked()),this,SLOT(OCR_clicked()));


}

void Widget::OK_clicked()
{
    copy=1;//be condition of clear
    update();//強制更新畫面
    //qDebug()<<"OK";
}


void Widget::Clear_clicked()
{
    _lines.clear();//clear all lines
    update();//強制更新畫面
    //qDebug()<<"Clear";
}

void Widget::OCR_clicked()
{
    QPixmap qpx;
    QImage img = qpx.grabWidget(this,0,0,Width*0.5, Height*0.75).\
            scaled(28, 28, Qt::KeepAspectRatio, Qt::SmoothTransformation).toImage();
    img.save("SaveImg.ppm","ppm");
    update();
}

//任何QPaint相關函式都應放在PaintEvent中
//QPaint會適時傳入更新過的QPaintEvent，但仍會有遺漏，所以透過update()強制更新畫面
void Widget::paintEvent(QPaintEvent *)
{
    int i=0;
    //先畫白色底
    QPainter whiter(this);
    whiter.setPen(Qt::white);
    whiter.setBrush(Qt::white);
    whiter.drawRect(0, 0, Width*0.5-1, Height*0.75);// initialize Canvas
    whiter.drawRect(Width*0.5+1, 0, Width*0.5-1, Height*0.75);

    //設定黑色畫筆
    QPainter path(this);
    QPen pen(Qt::black, 20, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin);
    path.setPen(pen);

    for(i=0; i<_lines.size(); i++)
    {
        const QVector<QPoint> &line = _lines.at(i);
        for(int j=0;j<line.size()-1;j++)
        {
            path.drawLine(line.at(j), line.at(j+1));
            //qDebug()<<line.at(j);
        }
    }

    //繪製右邊COPY
    if(copy==1)
    {
        for(i=0; i<_lines.size(); i++)
        {
            const QVector<QPoint> &line = _lines.at(i);
            for(int j=0;j<line.size()-1;j++)
            {

                QPoint start = line.at(j);
                QPoint end = line.at(j+1);
                //qDebug()<<line.at(j);
                start.setX(line.at(j).x()+Width/2);
                end.setX(line.at(j+1).x()+Width/2);

                //qDebug()<<start<<","<<end;
                path.drawLine(start, end);
            }
        }
        copy=0;
    }
}

void Widget::mouseMoveEvent(QMouseEvent *ev)
{

    if(_lines.size() == 0)
    {
        QVector<QPoint> line;
        _lines.append(line);
    }

    addLine(ev->pos());
    update();
}

void Widget::mousePressEvent(QMouseEvent *ev)
{
    QVector<QPoint> line;
    _lines.append(line);

    addLine(ev->pos());
}

void Widget::mouseReleaseEvent(QMouseEvent *ev)
{
    addLine(ev->pos());
}


void Widget::addLine(QPoint pos)
{
    QVector<QPoint> &lastLine = _lines.last();

    if(pos.x()<=Width/2-10 && pos.x()>5 && pos.y()<=Height*0.75)
    {
        erase=0;
        lastLine.append(pos);
    }
}
