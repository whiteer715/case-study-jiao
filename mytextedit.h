#ifndef MYTEXTEDIT_H
#define MYTEXTEDIT_H

#include <QTextEdit>

class MyTextEdit : public QTextEdit
{
public:
    MyTextEdit(QWidget *parent);

protected:
    void wheelEvent(QWheelEvent *e)     override;   //鼠标滚轮事件
    void keyPressEvent(QKeyEvent *e)    override;
    void keyReleaseEvent(QKeyEvent *e)  override;
public:
    bool aaaa=0;	//判断ctrl和滚轮是否一起按下
};

#endif // MYTEXTEDIT_H
