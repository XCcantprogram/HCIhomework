#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    setWindowFlags(Qt::FramelessWindowHint);
    ui->setupUi(this);
    //设置图标与样式
    QImage ima1;
    ima1.load(":/images/icons8-user-60.png");
    ima1.scaled(ui->userPict->size(),Qt::KeepAspectRatio);
    ui->userPict->setScaledContents(true);
    ui->userPict->setPixmap(QPixmap::fromImage(ima1));
    QImage ima2;
    ima2.load(":/images/icons8-lock-60.png");
    ima2.scaled(ui->passPict->size(),Qt::KeepAspectRatio);
    ui->passPict->setScaledContents(true);
    ui->passPict->setPixmap(QPixmap::fromImage(ima2));
    QImage ima3;
    ima3.load(":/images/mainicon.png");
    ima3.scaled(ui->mainPict->size(),Qt::KeepAspectRatio);
    ui->mainPict->setScaledContents(true);
    ui->mainPict->setPixmap(QPixmap::fromImage(ima3));
    ui->mainPict->setStyleSheet("background-color:rgba(0,0,0,0);border: none;");
    QIcon icon1;
    icon1.addFile(tr(":/images/icons8-closed-eye-100.png"));
    ui->eyePict->setIcon(icon1);
    ui->eyePict->setIconSize(QSize(30,30));
    ui->eyePict->setStyleSheet("background-color:rgba(0,0,0,0);border: none;");
    QIcon icon2;
    icon2.addFile(tr(":/images/icons8-close-100.png"));
    ui->closeButton->setIcon(icon2);
    ui->closeButton->setIconSize(QSize(20,20));
    ui->closeButton->setStyleSheet("background-color:rgba(0,0,0,0);border: none;");
    QIcon icon3;
    icon3.addFile(tr(":/images/icons8-minimize-100.png"));
    ui->minButton->setIcon(icon3);
    ui->minButton->setIconSize(QSize(20,20));
    ui->minButton->setStyleSheet("background-color:rgba(0,0,0,0);border: none;");

    ui->userPict->setStyleSheet("border: none;");
    ui->passPict->setStyleSheet("border: none;");
    ui->numEdit->setStyleSheet("border: none;");
    ui->passEdit->setStyleSheet("border: none;");
    ui->widget->setStyleSheet("border:1px solid #B6B6B6;border-radius:5px;");
    ui->widget_2->setStyleSheet("border:1px solid #B6B6B6;border-radius:5px;");
    ui->widget_3->setStyleSheet("background-color:rgb(86, 144, 130);");
    ui->loginButton->setStyleSheet("background-color:rgb(86, 144, 130);color:rgb(255, 255, 255);");
    ui->errorText->setStyleSheet("border: none;color:rgb(255, 0, 0);");

    //设置输入框范围
    QRegExp regExp1("[A-Za-z0-9]{1,20}");
    ui->passEdit->setValidator(new QRegExpValidator(regExp1, this));
    QRegExp regExp2("[0-9]{1,8}");
    ui->numEdit->setValidator(new QRegExpValidator(regExp2, this));
    ui->passEdit->setEchoMode(QLineEdit::Password); //隐藏密码
}

Login::~Login()
{
    delete ui;
}

void Login::on_closeButton_clicked()
{
    close(); //关闭
}

void Login::on_minButton_clicked()
{
    showMinimized(); //最小化
}

//密码显示模式(隐藏与显示)的切换
void Login::on_eyePict_clicked()
{
    if(passwordCansee){
        QIcon icon1;
        icon1.addFile(tr(":/images/icons8-closed-eye-100.png"));
        ui->eyePict->setIcon(icon1);
        ui->eyePict->setIconSize(QSize(30,30));
        ui->eyePict->setStyleSheet("background-color:rgba(0,0,0,0);border: none;");
        passwordCansee=false;
        ui->passEdit->setEchoMode(QLineEdit::Password); //隐藏密码
    }
    else{
        QIcon icon1;
        icon1.addFile(tr(":/images/icons8-eye-100.png"));
        ui->eyePict->setIcon(icon1);
        ui->eyePict->setIconSize(QSize(30,30));
        ui->eyePict->setStyleSheet("background-color:rgba(0,0,0,0);border: none;");
        passwordCansee=true;
        ui->passEdit->setEchoMode(QLineEdit::Normal); //显示密码
    }
}

void Login::mousePressEvent(QMouseEvent *event)
{
    ui->widget_2->setVisible(1);
    if(event->button() == Qt::LeftButton){
        mouse_is_press=1;
        mouse_posit=event->globalPos()-pos();
    }
}

void Login::mouseMoveEvent(QMouseEvent *event)
{
    if(!mouse_is_press){
        return;
    }
    move(event->globalPos()-mouse_posit);
    mouse_posit=event->globalPos()-pos();
}

void Login::mouseReleaseEvent(QMouseEvent *event)
{
    mouse_is_press=0;
}

void Login::on_loginButton_clicked()
{
    if(ui->numEdit->text().length()==0){
        ui->errorText->setText("错误：工号不得为空！");
        return;
    }
    if(ui->numEdit->text().length()!=8){
        ui->errorText->setText("错误：工号应为8位！");
        return;
    }
    if(ui->passEdit->text().length()==0){
        ui->errorText->setText("错误：密码不得为空！");
        return;
    }
    ui->errorText->setStyleSheet("border: none;color:(86, 144, 130);");
    ui->errorText->setText("正在登录中，请稍等");
    QNetworkRequest request;
    QNetworkAccessManager* naManager = new QNetworkAccessManager(this);

    QObject::connect(naManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(requestFinished(QNetworkReply*)));
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    request.setUrl(QUrl::fromUserInput("http://8.130.115.212:9091/api/Values/LoginAndSign"));

    QJsonObject object;
    object.insert("account",ui->numEdit->text());
    object.insert("keyword",ui->passEdit->text());
    object.insert("status","Login");

    QByteArray byte_array = QJsonDocument(object).toJson();
    qDebug()<< "user  info " << object;
    QNetworkReply* reply = naManager->post(request,byte_array); //发起post请求
}

void Login::requestFinished(QNetworkReply *reply)
{
    QString loginResultJson;
    QByteArray bytes = reply->readAll();
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(statusCode.isValid())
    {
        qDebug() << "status code=" << statusCode.toInt();
    }
    QNetworkReply::NetworkError err = reply->error();
    if(err != QNetworkReply::NoError)
    {
        qDebug() << "Failed: " << reply->errorString();
        ui->errorText->setStyleSheet("border: none;color:rgb(255, 0, 0);");
        ui->errorText->setText("登录失败，请检查网络！");
        return;
    }
    else
    {
        qDebug() << "loginResultJson  is  " <<  QString::fromStdString(bytes.toStdString());
        loginResultJson =  QString::fromStdString(bytes.toStdString());
        if(bytes.toStdString()=="OK"){
            qDebug() << "用户" << ui->numEdit->text()<<"已成功登录";
            MainWindow* w=new MainWindow();
            w->show();
            this->close();
        }
        else{
            ui->errorText->setStyleSheet("border: none;color:rgb(255, 0, 0);");
            ui->errorText->setText("登录失败，请检查工号和密码！");
            return;
        }
    }
}
