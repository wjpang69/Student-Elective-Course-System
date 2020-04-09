#ifndef DATASTATISTICS_H
#define DATASTATISTICS_H

#include <QWidget>
#include <QSqlQueryModel>

namespace Ui {
class DataStatistics;
}

class DataStatistics : public QWidget
{
    Q_OBJECT
public:
    explicit DataStatistics(QWidget *parent = 0);
    ~DataStatistics();

    void initialize();				//初始化

private slots:
    void on_btn_Fresh_clicked();	//刷新
    void on_tableView_Course_clicked(const QModelIndex &index);
                                    //获取当前选中行信息

private:
    Ui::DataStatistics *ui;
};

#endif // DATASTATISTICS_H
