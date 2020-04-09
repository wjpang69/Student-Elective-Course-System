#ifndef COURSEMANAGE_H
#define COURSEMANAGE_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QMessageBox>

namespace Ui {
class CourseManage;
}

class CourseManage : public QWidget
{
    Q_OBJECT
public:
    explicit CourseManage(QWidget *parent = 0);
    ~CourseManage();

    void initialize();              	//初始化
    void ExecSQL(QString sql);			//执行T-SQL语句


private slots:
    void on_btn_Fresh_clicked();		//刷新
    void on_tableView_Course_clicked(const QModelIndex &index);
                            //获取当前选中行信息
    void on_comboBox_Type_currentIndexChanged(int index);
                            //删除设置只读
    void on_pushButton_Act_clicked();		//执行操作
    void on_btn_Sort_clicked();             //排序
    void search();                      //搜索

private:
    Ui::CourseManage *ui;

private:
    QString EditNo;                 //编辑前No存放
    QString EditName;				//编辑前Name存放

};

#endif // COURSEMANAGE_H
