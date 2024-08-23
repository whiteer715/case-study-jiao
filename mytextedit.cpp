#include "mytextedit.h"

#include <QDebug>
#include <QWheelEvent>


MyTextEdit::MyTextEdit(QWidget *parent) :QTextEdit(parent)
{

}

void MyTextEdit::wheelEvent(QWheelEvent *e)
{
    qDebug()<< "鼠标滚轮事件：";
    qDebug()<< e->angleDelta().y(); //鼠标滚轮
    //判断是否ctrl和滚轮一起按下
    if(aaaa==1){
        //滚轮上下滚动触发事件
        if(e->angleDelta().y()>0){
            zoomIn();   //自带函数：放大
        }else if(e->angleDelta().y()<0){
            zoomOut();//缩小
        }
        e->accept();    //事件处理完毕
    }else{
        //条件不成立，恢复父类程序执行
        QTextEdit::wheelEvent(e);//调用父类
    }
}

void MyTextEdit::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Control){
        qDebug()<<"ctrl按键被按下";
        aaaa=1;
    }
    QTextEdit::keyPressEvent(e);//调用父类处理
}

void MyTextEdit::keyReleaseEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Control){
        qDebug()<<"ctrl按键被释放";
        aaaa=0;
    }
    QTextEdit::keyReleaseEvent(e);//调用父类处理
}
