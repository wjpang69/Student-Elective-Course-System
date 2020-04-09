#ifndef ENTER_STU_H
#define ENTER_STU_H

#include <QWidget>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QDebug>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QMessageBox>

namespace Ui {
class Enter_Stu;
}

class Enter_Stu : public QWidget
{
    Q_OBJECT
public:
    explicit Enter_Stu(QWidget *parent = 0);
    ~Enter_Stu();

    void initialize();                  //初始化
    void ExecSQL(QString sql);			//执行T-SQL语句


private slots:
    void on_btn_Fresh_clicked();		//刷新
    void on_btn_AddMy_clicked();		//选课
    void on_btn_DelMy_clicked();		//退课
    void on_tableView_Course_clicked(const QModelIndex &index);
                            //获取当前选中行信息
    void on_tableWidget_SelectedCourse_clicked(const QModelIndex &index);
                            //获取当前选中行信息
    void search();                   //搜索
private:
    Ui::Enter_Stu *ui;

public:
    QString Stu_No;
    QString Stu_Name;
};

#endif // ENTER_STU_H
