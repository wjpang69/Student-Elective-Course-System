#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include <QSqlDatabase>     //QSqlDatabase创建连接数据库实例
#include <QString>
#include <QDebug>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>
#include "enter_stu.h"
#include "enter_admin.h"


namespace Ui {
class Login;
}

class Login : public QMainWindow
{
    Q_OBJECT
public:
    explicit Login(QWidget *parent = 0);
    ~Login();

    bool CreatConnectionObj();			//创建数据库连接
    void ExecSQL(QString sql);			//执行T-SQL语句
    QString GetHtml(QString url);		//获取网页源码
    QString GetNetIP(QString webCode);	//分割源代码获得外网IP

private slots:
    void on_btn_Login_clicked();		//登录槽函数

private:
    Ui::Login *ui;

private:
    Enter_Stu* pEnter_Stu;
};
#endif // LOGIN_H
