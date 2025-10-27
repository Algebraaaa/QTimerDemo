#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent): QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    applyQSS();
    m_timer=new QTimer(this);
    m_timer->setTimerType(Qt::PreciseTimer);
    connect(m_timer,&QTimer::timeout,this,&Widget::do_timer_timeout);
    connect(ui->plainTextEdit->verticalScrollBar(), &QScrollBar::valueChanged,
            this, [this](int value)
            {
                QScrollBar *bar = ui->plainTextEdit->verticalScrollBar();

        //[这里有个语法基础]：a == b 并不是一个“表达式字符串”，而是一个会被计算出结果的表达式
                m_userAtBottom = (value == bar->maximum());
            });

}

Widget::~Widget()
{
    delete ui;
}

void Widget::applyQSS()
{
    // ---------------- QSS 样式 ----------------
    QString style = R"(
        /* 整体背景 */
        QWidget {
            background-color: #f0f0f0;
            font-family: '微软雅黑';
            font-size: 12pt;
        }

        /* 按钮样式 */
        QPushButton {
            background-color: #4CAF50;
            color: white;
            border-radius: 5px;
            padding: 5px 15px;
        }
        QPushButton:hover {
            background-color: #45a049;
        }
        QPushButton:disabled {
            background-color: #d3d3d3;
            color: #808080;
        }

        /* LCD 数字显示样式 */
        QLCDNumber {
            background-color: #222;
            color: #00FF00;
            border: 2px solid #555;
            border-radius: 5px;
        }

        /* 文本输出框 */
        QPlainTextEdit {
            background-color: #fff;
            border: 1px solid #ccc;
            border-radius: 5px;
            padding: 5px;
        }

        /* SpinBox 样式 */
        QSpinBox {
            border: 1px solid #ccc;
            border-radius: 5px;
            padding: 2px 5px;
            background-color: #fff;
        }

        /* RadioButton 样式 */
        QRadioButton {
            spacing: 5px;
        }
        QRadioButton::indicator {
            width: 16px;
            height: 16px;
            border-radius: 8px;
            border: 1px solid #555;
            background: white;
        }
        QRadioButton::indicator:checked {
            background: #4CAF50;
            border: 1px solid #333;
        }
    )";

    this->setStyleSheet(style);
    // ---------------- QSS 结束 ----------------
}

void Widget::setUiMode(UiMode mode)
{
    switch(mode)
    {
    //当程序运行时，应该只有stop按钮可以按下
    case UiMode::Running:
        ui->btnStart->setEnabled(false);
        ui->btnOneShot->setEnabled(false);
        ui->btnStop->setEnabled(true);
        break;
    //当程序结束时，应该只有stop按钮不可以按下
    case UiMode::Stopped:
        ui->btnStart->setEnabled(true);
        ui->btnOneShot->setEnabled(true);
        ui->btnStop->setEnabled(false);
        break;
    }
}

void Widget::do_timer_timeout()
{
    QApplication::beep();
    //获取当前时间
    QTime curTime=QTime::currentTime();
    ui->LCDHour->display(curTime.hour());
    ui->LCDMin->display(curTime.minute());
    ui->LCDSec->display(curTime.second());
    // 计算经过时间
    int tmMsec = m_counter.elapsed();
    QString str = QStringLiteral("流逝的时间：%1毫秒").arg(tmMsec);

    // 输出到 plainTextEdit
    ui->plainTextEdit->appendPlainText(str);

    //如果用户没有手动滚动，程序自动滚到底；如果用户正在查看上方历史记录，程序就不要动滚动条。
    if (m_userAtBottom)
        ui->plainTextEdit->ensureCursorVisible();

    if(m_timer->isSingleShot())
    {
        //如果是单次计时，则在timeout后进入stop模式
        //那么，连续计时的情况呢？则需要用户手动点击停止按钮，进入stop模式
        setUiMode(UiMode::Stopped);
    }
}

void Widget::on_btnStart_clicked()
{
    setUiMode(UiMode::Running);
    m_timer->setInterval(ui->spinBox->value());
    if(ui->radioContinue->isChecked())
        m_timer->setSingleShot(false);
    else
        m_timer->setSingleShot(true);
    if(ui->radioPrecise->isChecked())
        m_timer->setTimerType(Qt::PreciseTimer);
    else if(ui->radioCoarse->isChecked())
        m_timer->setTimerType(Qt::CoarseTimer);
    else
        m_timer->setTimerType(Qt::VeryCoarseTimer);
    m_timer->start();
    m_counter.start();
}


void Widget::on_btnStop_clicked()
{
    setUiMode(UiMode::Stopped);
    m_timer->stop();
    m_shotActive=false;
    int tmMsec=m_counter.elapsed();
    int ms=tmMsec % 1000;
    int sec=tmMsec/1000;
    QString str=QString("流逝的时间：%1秒，%2毫秒").arg(sec).arg(ms,3,10,QChar('0'));
    ui->plainTextEdit->appendPlainText(str);
    ui->plainTextEdit->ensureCursorVisible();
}


void Widget::on_btnOneShot_clicked()
{
    setUiMode(UiMode::Running);
    int intv=ui->spinBox->value();

    m_shotTotal=3;
    m_shotCount=0;
    m_shotActive=true;

    if(ui->radioPrecise->isChecked())
        m_shotType = Qt::PreciseTimer;
    else if(ui->radioCoarse->isChecked())
        m_shotType = Qt::CoarseTimer;
    else
        m_shotType = Qt::VeryCoarseTimer;

    m_counter.start();

    // 启动第一次单次定时
    QTimer::singleShot(intv, m_shotType, this, &Widget::do_timer_shot);

}
void Widget::do_timer_shot()
{
    if(!m_shotActive)return;
    QApplication::beep();

    // 输出流逝时间
    int tmMsec = m_counter.elapsed();
    QString str = QString("第%1次触发，流逝时间：%2毫秒").arg(m_shotCount + 1).arg(tmMsec);
    ui->plainTextEdit->appendPlainText(str);
    ui->plainTextEdit->ensureCursorVisible();

    m_shotCount++;

    // 判断是否还需要继续触发
    if(m_shotCount < m_shotTotal)
    {
        m_counter.restart();
        int interval = ui->spinBox->value();
        QTimer::singleShot(interval, m_shotType, this, &Widget::do_timer_shot);
    }
    else
    {
        m_shotActive=false;
        // 触发完成，恢复按钮
        setUiMode(UiMode::Stopped);
    }
}

