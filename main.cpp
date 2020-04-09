#include "login.h"
#include <QApplication>
#include <QSqlDatabase>     //QSqlDatabase创建连接数据库实例
#include <QDebug>
#include <QMessageBox>

bool CreatConnectionObj();
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Login w;
    w.show();
    if(CreatConnectionObj()==false){
        QMessageBox::about(NULL,"提示","连接数据库失败，请检查网络或者联系管理员。");
        return 0;
    }

    return a.exec();
}

//连接数据库函数
bool CreatConnectionObj()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");   //数据库为SQL Server类型
    db.setHostName("localhost");                            //数据库主机名
    db.setDatabaseName("SECS");                             //数据库名
    db.setUserName("sa");                                   //数据库用户名
    db.setPassword("sa");                                   //数据库密码
    if(db.open())                                           //打开数据库连接，调用open()方法打开数据库物理连接
    {
        qDebug()<<"Open Success...";
        return true;
    }
    else
    {
        qDebug()<<"Open Failed...";
        return false;
    }
}
