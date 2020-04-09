#ifndef STUMANAGE_H
#define STUMANAGE_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QDebug>
#include <QSqlQuery>

namespace Ui {
class StuManage;
}

class StuManage : public QWidget
{
    Q_OBJECT
public:
    explicit StuManage(QWidget *parent = 0);
    ~StuManage();

    void initialize();				//初始化
    void ExecSQL(QString sql);			//执行T-SQL语句


private slots:
    void on_btn_Fresh_clicked();		//刷新
    void on_btn_Act_clicked();			//执行操作
    void on_tableView_Student_clicked(const QModelIndex &index);
                            //获取当前选中行信息
    void on_comboBox_ActType_currentIndexChanged(int index);
                            //删除设置只读
    void search();                      //搜索

private:
    Ui::StuManage *ui;

private:
    QString EditNo;				//编辑前No存放
};


#endif // STUMANAGE_H
