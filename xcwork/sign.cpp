#include "sign.h"
#include "ui_sign.h"

Sign::Sign(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Sign)
{
    ui->setupUi(this);
    ui->addButton->setStyleSheet("background-color:rgb(86, 144, 130);color:rgb(255, 255, 255);");
    ui->numLab->setStyleSheet("border: none;");
    ui->passLab->setStyleSheet("border: none;");
    ui->conLab->setStyleSheet("border: none;");
    QRegExp regExp1("[A-Za-z0-9]{1,20}");
    QRegExp regExp2("[0-9]{1,20}");
    ui->passEdit->setValidator(new QRegExpValidator(regExp1, this));
    ui->conEdit->setValidator(new QRegExpValidator(regExp1, this));
    ui->numEdit->setValidator(new QRegExpValidator(regExp2, this));
    ui->errorText->setStyleSheet("border: none;color:rgb(255, 0, 0);");
}

Sign::~Sign()
{
    delete ui;
}

void Sign::on_addButton_clicked()
{
    if(ui->numEdit->text().length()==0){
        ui->errorText->setStyleSheet("border: none;color:rgb(255, 0, 0);");
        ui->errorText->setText("错误：工号不得为空！");
        return;
    }
    if(ui->numEdit->text().length()!=8){
        ui->errorText->setStyleSheet("border: none;color:rgb(255, 0, 0);");
        ui->errorText->setText("错误：工号应为8位！");
        return;
    }
    if(ui->passEdit->text().length()==0){
        ui->errorText->setStyleSheet("border: none;color:rgb(255, 0, 0);");
        ui->errorText->setText("错误：密码不得为空！");
        return;
    }
    if(ui->conEdit->text().length()==0){
        ui->errorText->setStyleSheet("border: none;color:rgb(255, 0, 0);");
        ui->errorText->setText("错误：确认密码不得为空！");
        return;
    }
    if(ui->conEdit->text()!=ui->passEdit->text()){
        ui->errorText->setStyleSheet("border: none;color:rgb(255, 0, 0);");
        ui->errorText->setText("错误：确认密码必须与密码相同！");
        return;
    }
    ui->errorText->setStyleSheet("border: none;color:rgb(86, 144, 130);");
    ui->errorText->setText("正在创建中，请稍等");
    QNetworkRequest request;
    QNetworkAccessManager* naManager = new QNetworkAccessManager(this);

    QObject::connect(naManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(requestFinished(QNetworkReply*)));
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    request.setUrl(QUrl::fromUserInput("http://8.130.115.212:9091/api/Values/LoginAndSign"));

    QJsonObject object;
    object.insert("account",ui->numEdit->text());
    object.insert("keyword",ui->passEdit->text());
    object.insert("status","Sign");

    QByteArray byte_array = QJsonDocument(object).toJson();
    qDebug()<< "user  info " << object;
    QNetworkReply* reply = naManager->post(request,byte_array); //发起post请求
}

void Sign::requestFinished(QNetworkReply *reply)
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
        ui->errorText->setText("创建失败，请检查网络！");
        return;
    }
    else
    {
        qDebug() << "loginResultJson  is  " <<  QString::fromStdString(bytes.toStdString());
        loginResultJson =  QString::fromStdString(bytes.toStdString());
        if(bytes.toStdString()=="OK"){
            qDebug() << "账号" << ui->numEdit->text()<<"已创建成功";
            ui->errorText->setStyleSheet("border: none;color:rgb(86, 144, 130);");
            ui->errorText->setText("账号" + ui->numEdit->text()+"已创建成功");
        }
        else{
            ui->errorText->setStyleSheet("border: none;color:rgb(255, 0, 0);");
            ui->errorText->setText("该账号已创建，账号创建失败！");
            return;
        }
    }
}
