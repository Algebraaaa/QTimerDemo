#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QTime>
#include <QElapsedTimer>
#include<QScrollBar>
QT_BEGIN_NAMESPACE
namespace Ui {class Widget;}
QT_END_NAMESPACE
//使用枚举类列出了该程序的两种状态：运行状态与停止状态
//当程序运行时，应该只有stop按钮可以按下
//当程序结束时，应该只有stop按钮不可以按下
enum class UiMode
{
    Running,
    Stopped
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    QTimer *m_timer=nullptr;
    int m_shotCount=0;
    int m_shotTotal=0;
    bool m_shotActive=false;
    bool m_userAtBottom = true;
    Qt::TimerType m_shotType;
    QElapsedTimer m_counter;
    void applyQSS();
    void setUiMode(UiMode mode);
    Ui::Widget *ui;

private slots:
    void do_timer_timeout();
    void do_timer_shot();
    void on_btnStart_clicked();
    void on_btnStop_clicked();
    void on_btnOneShot_clicked();
};


#endif // WIDGET_H
