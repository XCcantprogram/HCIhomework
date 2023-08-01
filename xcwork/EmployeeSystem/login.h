#ifndef LOGIN_H
#define LOGIN_H
#pragma execution_character_set("utf-8")
#include <QWidget>
#include <QDesktopWidget>
#include <QRegExpValidator>
#include <QMouseEvent>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonParseError>
#include <QJsonObject>
#include <QDebug>
#include "mainwindow.h"

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

private:
    Ui::Login *ui;
    bool passwordCansee=false;
    bool mouse_is_press=0;         //表示当前鼠标是否被按下
    QPoint mouse_posit;            //鼠标的相对坐标

protected:
    virtual void mousePressEvent(QMouseEvent *event);    //鼠标点击事件
    virtual void mouseMoveEvent(QMouseEvent *event);     //鼠标移动事件
    virtual void mouseReleaseEvent(QMouseEvent *event);  //鼠标释放事件

private slots:
    void on_closeButton_clicked();
    void on_minButton_clicked();
    void on_eyePict_clicked();
    void on_loginButton_clicked();
    void requestFinished(QNetworkReply *reply);      //用来接收网络访问结果的槽函数
};

#endif // LOGIN_H
