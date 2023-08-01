#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //设置左侧标签导航栏内容与样式
    QString str="组织规划||0||,部门信息|组织规划|||,部门管理|组织规划|||,员工信息||0||,我的信息|员工信息|||,全体信息|员工信息|||,职务管理||0||,我的职务|职务管理|||,简历管理|职务管理|||,入职登记|职务管理|||,合同管理||0||,合同信息查看|合同管理|||,合同信息管理|合同管理|||,考勤管理||0||,考勤排班|考勤管理|||,我的日历|考勤管理|||,考勤数据修改|考勤管理|||,薪资管理||0||,我的薪水|薪资管理|||,薪水变更|薪资管理|||";

    ui->navListView->setItems(str);
    ui->navListView->setChildBgNormalColor(QColor(255,255,255));
    ui->navListView->setChildTextNormalColor(QColor(0,0,0));
    ui->navListView->setChildBgHoverColor(QColor(100,149,255));
    ui->navListView->setChildTextHoverColor(QColor(255,255,255));
    ui->navListView->setChildBgSelectedColor(QColor(255,255,255));
    ui->navListView->setChildTextSelectedColor(QColor(0,0,0));
    ui->navListView->setTriangleColor(QColor(100,149,255));
    ui->navListView->setLineColor(QColor(100,149,255));
    ui->navListView->setParentBgNormalColor(QColor(65,105,225));
    ui->navListView->setParentTextNormalColor(QColor(255,255,255));
    ui->navListView->setParentBgHoverColor(QColor(0,0,255));
    ui->navListView->setParentTextHoverColor(QColor(255,255,255));
    ui->navListView->setParentBgSelectedColor(QColor(65,105,225));
    ui->navListView->setParentTextSelectedColor(QColor(255,255,255));
    /*changed*/
    //注册事件过滤器
    installEventFilter(this);
    //创建QNetworkAccessManager对象
    networkManager = new QNetworkAccessManager(this);
    //隐藏标题栏
    setWindowFlags (Qt::CustomizeWindowHint);

    //合同查看的查询按键
    connect(ui->testpush,SIGNAL(clicked(bool)),this,SLOT(BtnshowhelloSlot()));


    //将上方标签按钮加入按钮组
    topgroup->addButton(ui->toolButton,0);
    topgroup->addButton(ui->toolButton_2,1);
    topgroup->addButton(ui->toolButton_3,2);
    ui->widgetTitle->setProperty("form", "title");
    ui->widgetLeft->setProperty("nav", "left");
    ui->widgetTop->setProperty("nav", "top");
    connect(ui->navListView,SIGNAL(pressed(int,int)),this,SLOT(GetLeftPress(int,int)));
    connect(topgroup,SIGNAL(buttonClicked(int)),this,SLOT(GetTopPress(int)));
    ui->navListView->setCurrentRow(1);
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(UpdateLeft()));
    timer->start(10);
    ui->stackedWidget->setCurrentIndex(0);


    requestData("E0001");
    ui->page5_stack->setCurrentIndex(0);

    showWorkSchedule();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent( QCloseEvent * event ){
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "退出程序", "确定要退出吗?", QMessageBox::Yes|QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            // 用户选择了退出，关闭程序
            event->accept();
        } else {
            // 用户选择了取消，忽略关闭事件
            event->ignore();
        }
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event){
        {
            static bool isMousePressed = false;		//鼠标是否按下
            static QPoint mousePos;					//鼠标按下位置在窗体中的坐标

            QMouseEvent *ev = static_cast<QMouseEvent *>(event);

            //鼠标左键按下
            if (ev->type()==QEvent::MouseButtonPress)
            {
                if (ev->button()==Qt::LeftButton)
                {
                    isMousePressed = true;
                    mousePos = ev->globalPos() - pos();		//鼠标全局坐标-窗体左上顶点坐标
                }
                else
                {
                    exit(0);
                }

            }
            else if (ev->type() == QEvent::MouseButtonRelease)
            {
                isMousePressed = false;
                return true;
            }
            else if (ev->type() == QEvent::MouseMove)
            {
                if (isMousePressed && (ev->buttons() && Qt::LeftButton))
                {
                    this->move(ev->globalPos() - mousePos);
                    return true;
                }
            }
            return QWidget::eventFilter(watched, event);;
        }
}

void MainWindow::GetLeftPress(int index, int parentIndex)
{
    if(parentIndex == -1){ //当点击的是可展开标签时，更新记录的展开状态
        parentexpand[index]=1-parentexpand[index];
        return;
    }
    parentIndexnow=parentIndex;
    indexnow=index;
    topgroup->button(0)->setChecked(true);
    topgroup->setExclusive(false);
    topgroup->button(0)->setChecked(false);
    topgroup->setExclusive(true);
    int page=0;
    if(parentIndex<=2){
        page=parentIndex*2+index;
    }
    else if(parentIndex<=4){
        page=parentIndex*2+index+1;
    }
    else{
        page=parentIndex*2+index+2;
    }
    /*
     * 此处进行页面切换
     */
    ui->stackedWidget->setCurrentIndex(page);
}

void MainWindow::GetTopPress(int index)
{
    ui->navListView->setCurrentRow(-1);
    indexnow=0;
    parentIndexnow=-1;
    int page=14+index;
    /*
     * 此处进行页面切换
     */
    ui->stackedWidget->setCurrentIndex(page);
}

void MainWindow::UpdateLeft()
{
    if(parentexpand[parentIndexnow]){
        int row=0;
        for(int i=0;i<parentIndexnow;i++){
            row+=parentexpand[i]*parentlong[i]+1;
        }
        row+=indexnow+1;
        ui->navListView->setCurrentRow(row);
    }
}

void MainWindow::on_btnMenu_Close_clicked()
{
    close();
}

void MainWindow::on_btnMenu_Min_clicked()
{
    showMinimized();
}

void MainWindow::on_btnMenu_Max_clicked()
{
    if (this->isMaximized())
    {
        this->showNormal();//还原事件
        ui->btnMenu_Max->setText("□");
        ui->btnMenu_Max->setToolTip(tr("最大化"));
    }
    else
    {
        this->showMaximized();//最大化事件
        ui->btnMenu_Max->setText("▣");
        ui->btnMenu_Max->setToolTip(tr("最大化还原"));
    }
}

void MainWindow::requestData(const QString& empID)
{
    QUrl url("http://8.130.115.212:1000/api/DeployInfoQuery/GetDeployInfo/" + empID);

    QNetworkRequest request;
    request.setUrl(url);

    QNetworkReply* reply = networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, &MainWindow::handleNetworkReply);
}

void MainWindow::handleNetworkReply()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray response = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response);

        // 解析JSON数据
        if (!jsonDoc.isArray()) {
            // 错误处理: 如果返回的JSON不是数组
            return;
        }

        QJsonArray jsonArray = jsonDoc.array();
        model = new QStandardItemModel(jsonArray.size(), 6, this);
        model->setHeaderData(0, Qt::Horizontal, tr("Business ID"));
        model->setHeaderData(1, Qt::Horizontal, tr("Enterprise Name"));
        model->setHeaderData(2, Qt::Horizontal, tr("Department Name"));
        model->setHeaderData(3, Qt::Horizontal, tr("Post Name"));
        model->setHeaderData(4, Qt::Horizontal, tr("Start Time"));
        model->setHeaderData(5, Qt::Horizontal, tr("End Time"));

        for (int i = 0; i < jsonArray.size(); ++i)
        {
            QJsonObject jsonObj = jsonArray.at(i).toObject();

            QStandardItem* item1 = new QStandardItem(jsonObj.value("businessID").toString());
            QStandardItem* item2 = new QStandardItem(jsonObj.value("businessEnterpriseName").toString());
            QStandardItem* item3 = new QStandardItem(jsonObj.value("businessDepartmentName").toString());
            QStandardItem* item4 = new QStandardItem(jsonObj.value("businessPostName").toString());
            QStandardItem* item5 = new QStandardItem(jsonObj.value("startTime").toString());
            QStandardItem* item6 = new QStandardItem(jsonObj.value("endTime").toString());

            model->setItem(i, 0, item1);
            model->setItem(i, 1, item2);
            model->setItem(i, 2, item3);
            model->setItem(i, 3, item4);
            model->setItem(i, 4, item5);
            model->setItem(i, 5, item6);
        }

        // 在QTableView中显示表格数据
        ui->deployment->setModel(model);
    }

    reply->deleteLater();
}

void MainWindow::BtnshowhelloSlot(void)  //合同查看函数
{
    QUrl url("http://8.130.115.212:2580/api/WebDBtest/GetText2?Employee_ID=E0001&Contract_ID=A0001");
    QNetworkRequest request;

    request.setUrl(url);
    QNetworkAccessManager manager;
    QNetworkReply *reply = manager.get(request);
    QEventLoop eventLoop;
    QObject::connect(reply, SIGNAL(finished()),&eventLoop, SLOT(quit()));
    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

    //对请求的返回异常进行处理
    if(reply->error() != QNetworkReply::NoError)
    {
        ui->textBrowser_2->insertPlainText("错误，请联系管理员");
    }

    //解析返回的Json结果
    QByteArray replyData = reply->readAll();
    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(replyData, &json_error));
    if(json_error.error != QJsonParseError::NoError)
    {
        ui->textBrowser_2->insertPlainText("错误，请联系管理员");
    }
    QJsonObject rootObj = jsonDoc.object();
    QString codeStr = rootObj.value("employeeID").toString();
    QString codeStr2 = rootObj.value("contractId").toString();
    QString codeStr3 = rootObj.value("contractType").toString();
    QString codeStr4 = rootObj.value("contractState").toString();
    QString codeStr5 = rootObj.value("effectiveDate").toString();
    QString codeStr6 = rootObj.value("inoperativeDate").toString();
    QString codeStr7 = rootObj.value("documentLink").toString();

    ui->textBrowser_2->setText("");
    ui->textBrowser_3->setText("");
    ui->textBrowser_4->setText("");
    ui->textBrowser_5->setText("");
    ui->textBrowser_6->setText("");
    ui->textBrowser_7->setText("");
    ui->textBrowser_8->setText("");

    ui->textBrowser_2->insertPlainText(codeStr);
    ui->textBrowser_3->insertPlainText(codeStr2);
    ui->textBrowser_4->insertPlainText(codeStr3);
    ui->textBrowser_5->insertPlainText(codeStr4);
    ui->textBrowser_6->insertPlainText(codeStr5);
    ui->textBrowser_7->insertPlainText(codeStr6);
    ui->textBrowser_8->insertPlainText(codeStr7);

}

void MainWindow::showWorkSchedule(){

    //Qurl url();
    //QNetworkAccessManager manager;
    //等后端完成相应接口后添加获取数据的代码

    work_model = new QStandardItemModel();

    QStringList line_header;
    line_header<<"周一"<<"周二"<<"周三"<<"周四"<<"周五"<<"周六"<<"周日";
    work_model->setHorizontalHeaderLabels(line_header);
    QStringList row_header;
    row_header<<"上午"<<"下午";
    work_model->setVerticalHeaderLabels(row_header);

    QString data={"1111100"};

    for(int i=0; i<7; i++){
        if(data[i]=='0'){
            work_model->setItem(0,i,new QStandardItem("×"));
            work_model->setItem(1,i,new QStandardItem("×"));
        }
        else{
            work_model->setItem(0,i,new QStandardItem("√"));
            work_model->setItem(1,i,new QStandardItem("√"));
        }
    }

    ui->schedule->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->schedule->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->schedule->setModel(work_model);

    QString id={"123456"};
    ui->work_id->setText(id);
}
