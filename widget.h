#ifndef WIDGET_H
#define WIDGET_H

#include <QFile>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    //添加公有成员变量
    QFile file;

public:
    //事件
    // void enterEvent(QEnterEvent *event) override;   //移入事件
    // void leaveEvent(QEvent *event) override;     //移出事件
    // void wheelEvent(QWheelEvent *event) override;    //滚轮事件
    void closeEvent(QCloseEvent *event) override;    //关闭事件


public slots:
    void Fun_cursorPositionChanged();//光标改变槽函数声明

private slots:
    void on_btnClose_clicked();

    //打开文件按钮的槽函数
    void on_btnOpen_clicked();

    void on_btnSave_clicked();

    // void on_comboBox_currentIndexChanged(int index);

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
