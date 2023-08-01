#ifndef SIGN_H
#define SIGN_H
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

namespace Ui {
class Sign;
}

class Sign : public QWidget
{
    Q_OBJECT

public:
    explicit Sign(QWidget *parent = nullptr);
    ~Sign();

private:
    Ui::Sign *ui;

private slots:
    void on_addButton_clicked();
    void requestFinished(QNetworkReply *reply);      //用来接收网络访问结果的槽函数
};

#endif // SIGN_H
