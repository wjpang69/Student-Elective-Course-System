#include "enter_admin.h"
#include "ui_enter_admin.h"

//构造函数
Enter_Admin::Enter_Admin(QWidget *parent) :QWidget(parent),ui(new Ui::Enter_Admin)
{
    ui->setupUi(this);
    initialize();
}

//析构函数
Enter_Admin::~Enter_Admin()
{
    delete ui;
}

//初始化函数
void Enter_Admin::initialize(){
    ui->tabWidget_Manage->findChildren<QTabBar*>().at(0)->hide();       //隐藏子夹头

    StuManage* pStuManage = new StuManage();
    ui->tabWidget_Manage->addTab(pStuManage,tr("学生管理"));
    CourseManage* pCourseManage  = new CourseManage();
    ui->tabWidget_Manage->addTab(pCourseManage,tr("课程管理"));
    DataStatistics* pDataStatistics  = new DataStatistics();
    ui->tabWidget_Manage->addTab(pDataStatistics,tr("数据统计"));
}

//切换tabWidget槽函数
void Enter_Admin::on_toolButton_Index_clicked()
{
    ui->tabWidget_Manage->setCurrentIndex(0);
}

//切换tabWidget槽函数
void Enter_Admin::on_toolButton_StudentManage_clicked()
{
    ui->tabWidget_Manage->setCurrentIndex(1);
}

//切换tabWidget槽函数
void Enter_Admin::on_toolButton_CourseManage_clicked()
{
    ui->tabWidget_Manage->setCurrentIndex(2);
}

//切换tabWidget槽函数
void Enter_Admin::on_toolButton_statistics_clicked()
{
    ui->tabWidget_Manage->setCurrentIndex(3);
}

//关闭
void Enter_Admin::on_toolButton_ExitSys_clicked()
{
    this->close();
}
