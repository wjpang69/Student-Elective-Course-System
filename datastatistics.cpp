#include "datastatistics.h"
#include "ui_datastatistics.h"

//构造函数
DataStatistics::DataStatistics(QWidget *parent) :QWidget(parent),ui(new Ui::DataStatistics)
{
    ui->setupUi(this);
    on_btn_Fresh_clicked();
    initialize();
}

//析构函数
DataStatistics::~DataStatistics()
{
    delete ui;
}

//初始化函数
void DataStatistics::initialize()
{
    ui->tableView_Course->setSelectionBehavior(QAbstractItemView::SelectRows);                      //设置整行选择
    ui->tableView_Course->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);  //设置自适应宽度
    ui->tableView_Course->setSelectionMode(QAbstractItemView::SingleSelection);                     //设置每次只能一行

    ui->tableView_Student->setSelectionBehavior(QAbstractItemView::SelectRows);                      //设置整行选择
    ui->tableView_Student->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);  //设置自适应宽度
    ui->tableView_Student->setSelectionMode(QAbstractItemView::SingleSelection);                     //设置每次只能一行
}

//刷新槽函数
void DataStatistics::on_btn_Fresh_clicked()
{
    //课程数目部分
    QSqlQueryModel *model1=new QSqlQueryModel;
    model1->setQuery("SELECT COUNT (*) FROM SECS.dbo.Course");
    QModelIndex index1=model1->index(0,0);
    ui->label_Course->setText(index1.data().toString());

    //学生数目部分
    QSqlQueryModel *model2=new QSqlQueryModel;
    model2->setQuery("SELECT COUNT (*) FROM SECS.dbo.Student");
    QModelIndex index2=model2->index(0,0);
    ui->label_Student->setText(index2.data().toString());

    //课程部分
    QSqlQueryModel* model3=new QSqlQueryModel;
    QString sql3 = "SELECT No as 课程代码 ,Name as 课程名称 FROM SECS.dbo.Course";
    model3->setQuery(sql3);
    ui->tableView_Course->setModel(model3);
}

//获取选中课程学生槽函数
void DataStatistics::on_tableView_Course_clicked(const QModelIndex &index)
{
    //获取选中行信息
    int row = ui->tableView_Course->currentIndex().row();           //获取当前选中行的行数
    QAbstractItemModel *model0 = ui->tableView_Course->model();
    QModelIndex view_index;
    view_index= model0->index(row,1);

    //执行查询
    QSqlQueryModel* model=new QSqlQueryModel;
    QString sql = "SELECT StuNo as 学号 ,StuName as 姓名 FROM SECS.dbo.SelectedCourse WHERE CourseName='"+model0->data(view_index).toString()+"'";
    model->setQuery(sql);
    ui->tableView_Student->setModel(model);
}
