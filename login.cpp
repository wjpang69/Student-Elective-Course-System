#include "login.h"
#include "ui_login.h"


//构造函数
Login::Login(QWidget *parent) :QMainWindow(parent),ui(new Ui::Login)
{
    ui->setupUi(this);
   // pEnter_Stu=new Enter_Stu();

}

//析构函数
Login::~Login()
{
    delete ui;
}

//执行SQL语句函数
void Login::ExecSQL(QString sql)
{
    QSqlQueryModel *model=new QSqlQueryModel;       //QSqlQueryModel类为SQL的结果集提供了一个只读的数据模型
    model->setQuery(sql);                           //执行SQL语句
}

//获取网页所有源代码
QString Login::GetHtml(QString url) {
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(url)));
    QByteArray responseData;
    QEventLoop eventLoop;
    QObject::connect(manager, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    responseData = reply->readAll();
    return QString(responseData);
}

//获取本机公网IP地址
QString Login::GetNetIP(QString webCode) {
    QString tmp = webCode.split("[").last();
    return tmp.split("]").first();
}

//登录槽函数
void Login::on_btn_Login_clicked()
{
    QString user=ui->lineEdit_user->text();
    QString pwd =ui->lineEdit_pwd ->text();
    int type = ui->comboBox->currentIndex();

    if(user!="" || pwd!=""){
        if(type==0){//普通用户
            QSqlQueryModel *model1=new QSqlQueryModel;         //QSqlQueryModel类为SQL的结果集提供了一个只读的数据模型
            model1->setQuery("SELECT COUNT (*) FROM SECS.dbo.Student WHERE NO='"+ user +"'");    //执行SQL语句
            QModelIndex index1=model1->index(0,0);             //返回指定的行和列的索引(index)  即表格的第一个格
            QString QueryUser=index1.data().toString();                 //取索引对应的值 并且转为QString类型
            qDebug()<<QueryUser;

            QSqlQueryModel *model2=new QSqlQueryModel;
            model2->setQuery("SELECT Pwd FROM SECS.dbo.Student WHERE No='"+ user +"'");
            QModelIndex index2=model2->index(0,0);
            QString QueryPwd=index2.data().toString();
            qDebug()<<QueryPwd;

            //判断正误
            if(QueryUser != "0" && pwd == QueryPwd)
            {
                QMessageBox::about(NULL,"提示","学生你好，登录成功。");
                pEnter_Stu=new Enter_Stu();
                pEnter_Stu->Stu_No=user;
                pEnter_Stu->show();
                pEnter_Stu->initialize();
                this->hide();                                   //把登录本窗口隐藏
            }
            else
            {
                QMessageBox::about(NULL,"提示","用户名或密码错误！");

            }
        }
        else{//管理员

            //查询数据库中IP
            QSqlQueryModel *model3=new QSqlQueryModel;
            model3->setQuery("SELECT AdminIP FROM SECS.dbo.Admin WHERE AdminUser='"+ user +"'");
            QModelIndex index3=model3->index(0,0);
            QString QueryIP=index3.data().toString();

            //获取当前IP
            QString CurrentIP=GetNetIP(GetHtml("http://2019.ip138.com/ic.asp"));

            //判断
            QSqlQueryModel *model4=new QSqlQueryModel;
            model4->setQuery("SELECT COUNT (*) FROM SECS.dbo.Admin WHERE AdminUser='"+ user +"'");
            QModelIndex index4=model4->index(0,0);
            QString QueryUser=index4.data().toString();
            qDebug()<<QueryUser;

            QSqlQueryModel *model5=new QSqlQueryModel;
            model5->setQuery("SELECT AdminPwd FROM SECS.dbo.Admin WHERE AdminUser ='"+ user +"'");
            QModelIndex index5=model5->index(0,0);
            QString QueryPwd=index5.data().toString();
            qDebug()<<QueryPwd;

            //判断正误
            if(QueryUser != "0" && pwd == QueryPwd)
            {
                if(CurrentIP==QueryIP)
                {
                    QMessageBox::about(NULL,"提示","管理员你好，登录成功。");
                    Enter_Admin* pEnter_Admin=new Enter_Admin();
                    pEnter_Admin->show();
                    this->hide();                                   //把登录本窗口隐藏
                }
                else
                {
                    QMessageBox::about(NULL,"提示","请使用内网登录！");
                }
            }
            else
            {
                QMessageBox::about(NULL,"提示","用户名或密码错误！");

            }



        }
    }else{
         QMessageBox::about(NULL,"提示","请输入完整的用户名或密码！");
    }

}



