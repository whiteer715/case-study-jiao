#include "widget.h"
#include "ui_widget.h"

#include <QFile>
#include <Qdebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QShortcut>
#include <QWheelEvent>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //textEdit内光标位置改变信号与槽的绑定
    connect(ui->textEdit,SIGNAL(cursorPositionChanged()),this,SLOT(Fun_cursorPositionChanged()));


    //制作两个快捷键
    QShortcut *shortcutopen = new QShortcut(QKeySequence("Ctrl+o"), this);  //打开快捷键
    QShortcut *shortcutsave = new QShortcut(QKeySequence("Ctrl+s"), this);  //保存快捷键
    QShortcut *shortcutZoomIn = new QShortcut(QKeySequence("Ctrl+="), this);   //字体放大

    //把ctrl+o的信号添加槽，调用打开按键的槽函数
    QObject::connect(shortcutopen, &QShortcut::activated,[&](){	Widget::on_btnOpen_clicked();});    //ctrl+o
    QObject::connect(shortcutsave, &QShortcut::activated,[&](){	Widget::on_btnSave_clicked();});    //ctrl+s
    QObject::connect(shortcutZoomIn, &QShortcut::activated,[&](){
        qDebug() << "TEST";
        QFont font = ui->textEdit->font();
        int fontSize = font.pointSize();//获得当前字体大小
        if (fontSize==-1) return;
        //改变大小，并设置
        int newfontSize = fontSize + 1;
        font.setPointSize(newfontSize);
        ui->textEdit->setFont(font);
    });


    //通过下面这行代码进行显示说明，让窗口变化时，布局及其子控件随之调整
    this->setLayout(ui->verticalLayout);


}

Widget::~Widget()
{
    delete ui;
}


//============= 信号函数 =============
// void Widget::enterEvent(QEnterEvent *event)
// {
//     qDebug() << "鼠标移入窗体";
// }

// void Widget::leaveEvent(QEvent *event)
// {
//     qDebug() << "鼠标移出窗体";
// }

// void Widget::wheelEvent(QWheelEvent *event)
// {
//     qDebug() << event->angleDelta();
// }

void Widget::closeEvent(QCloseEvent *event)
{
    int ret = QMessageBox::warning(this, tr("关闭"),
                                   tr("文件已被修改.\n"
                                      "是否关闭?"),
                                   QMessageBox::Yes | QMessageBox::No);
    switch (ret) {
    case QMessageBox::Yes:
        event->accept();//接受
        break;
    case QMessageBox::No:
        event->ignore();//取消
        break;
    default:
        // should never be reached
        break;
    }

}


//============= 槽函数 =============

//光标改变位置
void Widget::Fun_cursorPositionChanged()
{
    //输出行列号
    QTextCursor cursor = ui->textEdit->textCursor();    //获取当前光标位置
    qDebug() << cursor.columnNumber() +1 ;  //打印光标列号，由于程序中第一个位置为0
    qDebug() << cursor.blockNumber() +1 ;   //打印光标行号
    QString blockNum = QString::number(cursor.blockNumber() +1);  //将列整型转换成字符型
    QString columnNum = QString::number(cursor.columnNumber() +1);
    const QString label = "行:" +blockNum+",列:"+columnNum;   //字符串合并
    ui->label->setText(label);

    //设置当前行高亮
    QList<QTextEdit:: ExtraSelection> extraSelections;  //列表容器声明
    QTextEdit:: ExtraSelection ext; //对象声明
    //1.知道当前行
    ext.cursor = ui->textEdit->textCursor();
    //2.颜色
    QBrush qBrush(Qt:: yellow);
    ext.format.setBackground(qBrush);
    ext.format.setProperty(QTextFormat::FullWidthSelection, true);
    //3.设置
    extraSelections.append(ext);
    ui->textEdit->setExtraSelections(extraSelections);


}

//打开文件按钮的槽函数
void Widget::on_btnOpen_clicked()
{
    //打开文件夹
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open File"),
                                                    "D:/1study_biancheng/QT_model/test01/untitled3",//默认打开的文件夹名称
                                                    tr("Text files (*.txt);;Images (*.png *.xpm *.jpg)"));  //打开的文件格式
    qDebug()<<"打开文件名："<< fileName;

    //清空textEdit控件的全部内容
    ui->textEdit->clear();

    //读取
    // QFile file;//头文件内已有公有变量file
    file.setFileName(fileName);
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text)){  //判断是否打开
        qDebug()<<"未打开文件";    //文件已只读方式打开，open为布尔类型
    }
    QTextStream in(&file);
    // in.setCodec("UTF-8");   //设置编码为UTF-8
    while (!in.atEnd()){
        QString context = in.readLine();
        qDebug()<<"文档内容："<< context;

        //文件内容读取入文本框中
        // ui->textEdit->setText(context); //setText()读取时会覆盖，最后仅保留一行
        ui->textEdit->append(context);  //append()读取追加，可获得全部文档内容
    }
}

//新建和保存文件按钮的槽函数
void Widget::on_btnSave_clicked()
{
    //若文件已经打开，不用弹窗
    if (!file.isOpen()){
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                        "D:/1study_biancheng/QT_model/test01/untitled3",
                                                        tr("Text(*.txt *.doc)"));//保存文件
        qDebug()<<" 保存文件名："<< fileName;
        //导入文件
        // QFile file;//头文件内已有公有变量file
        file.setFileName(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
            qDebug() << "Cannot open file for writing: "<< file.errorString();
            return;
        }
        this->setWindowTitle("新建文本");
    }

    //写入：一定要有的
    QTextStream out(&file);	//拷贝文件内容
    QString contexts = ui->textEdit->toPlainText(); //获得textEdit内的所有文字信息
    out << contexts;  //写入文件内容
}

//关闭按钮的槽函数
void Widget::on_btnClose_clicked()
{
    qDebug() <<" close exe ";
    qDebug()<<file.isOpen();
    //QMessageBox消息弹窗
    int ret = QMessageBox::warning(this, tr("关闭"),
                                   tr("文件已修改.\n"
                                      "是否保存修改内容?"),
                                   QMessageBox::Save | QMessageBox::Discard
                                       | QMessageBox::Cancel,
                                   QMessageBox::Save);
    //不同按钮被按下
    switch (ret) {
    case QMessageBox::Save:
        //保存文件
        on_btnSave_clicked();   //执行保存函数
        break;
    case QMessageBox::Discard:
        //丢弃
        ui->textEdit->clear();  //清空内容
        //判断文件是否有被打开，isOpen返回值为布尔类型
        if (file.isOpen()){
            file.close();   //回收变量
        }
        break;
    case QMessageBox::Cancel:
        //误触
        break;
    default:
        // should never be reached
        break;
    }
}

// //下拉框变化时，文字自动转化读取格式
// void Widget::on_comboBox_currentIndexChanged(int index)
// {
//     qDebug()<<"currentItsignal";
//     ui->textEdit->clear();
//     if(file.isOpen()){
//         qDebug()<<"file is Open";
//         QTextStream in(&file);
//         in.setCodec(ui->comboBox->currentText().toStdString().c_str());
//         file.seek(0);
//         while(!in.atEnd()){
//             QString context = in.readLine();
//             ui->textEdit->append(context);
//         }
//     }
// }
