#include "enter_stu.h"
#include "ui_enter_stu.h"

//构造函数
Enter_Stu::Enter_Stu(QWidget *parent) :QWidget(parent),ui(new Ui::Enter_Stu)
{
    ui->setupUi(this);
    connect(ui->lineEdit_Search,SIGNAL(textChanged(QString)),this,SLOT(search()));  //将字符变化的事件，与函数连接
}

//析构函数
Enter_Stu::~Enter_Stu()
{
    delete ui;
}

//初始化函数
void Enter_Stu::initialize()
{
    //Course
    ui->tableView_Course->setSelectionBehavior(QAbstractItemView::SelectRows);                      //设置整行选择
    ui->tableView_Course->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);  //设置自适应宽度
    ui->tableView_Course->setSelectionMode(QAbstractItemView::SingleSelection);                     //设置每次只能一行

    //SelectedCourse
    ui->tableWidget_SelectedCourse->setEditTriggers (QAbstractItemView::NoEditTriggers);            //设置不能编辑

    //初始化载入
    on_btn_Fresh_clicked();                                                                           //刷新一下

    //读取学号
    ui->label_No->setText(Stu_No);

    //读取姓名
    QSqlQueryModel *model=new QSqlQueryModel;
    model->setQuery("SELECT Name FROM SECS.dbo.Student WHERE No ='"+ Stu_No +"'");
    QModelIndex index=model->index(0,0);
    Stu_Name=index.data().toString();
    ui->label_Name->setText(Stu_Name);
}

//搜索槽函数
void Enter_Stu::search()
{
    QSqlQueryModel *model=new QSqlQueryModel;
    QString search=ui->lineEdit_Search->text();
    QString sql;
    if(ui->comboBox_SearchType->currentIndex()==0)
    {
        sql = "SELECT No as 课程代码 ,Name as 课程名称,Nature as 课程性质 ,Period as 总学时, Credit as 学分, Term as 开课学期,"
                      " Number as 选修人数 FROM SECS.dbo.Course WHERE Name LIKE '%"+search+"%'";
    }
    else if(ui->comboBox_SearchType->currentIndex()==1)
    {
        sql = "SELECT No as 课程代码 ,Name as 课程名称,Nature as 课程性质 ,Period as 总学时, Credit as 学分, Term as 开课学期,"
                      " Number as 选修人数 FROM SECS.dbo.Course WHERE No LIKE '%"+search+"%'";
    }
    model->setQuery(sql);
    ui->tableView_Course->setModel(model);


}

//执行SQL语句函数
void Enter_Stu::ExecSQL(QString sql)
{
    QSqlQueryModel *model=new QSqlQueryModel;       //QSqlQueryModel类为SQL的结果集提供了一个只读的数据模型
    model->setQuery(sql);                           //执行SQL语句
}

//刷新槽函数
void Enter_Stu::on_btn_Fresh_clicked()
{
    //Course
        //查询部分
    QSqlQueryModel* model=new QSqlQueryModel;       //为SQL查询的结果集提供了一个只读的数据模型
    QString sql = "SELECT No as 课程代码 ,Name as 课程名称,Nature as 课程性质 ,Period as 总学时, Credit as 学分, Term as 开课学期,"
                  " Number as 选修人数 FROM SECS.dbo.Course";
    model->setQuery(sql);
        //建立模型部分
    ui->tableView_Course->setModel(model);         //将数据导入TableView

    //SelectedCourse
    QSqlQuery query;
    int i = 0, j = 0, nColumn, nRow;
    query.prepare("SELECT CourseName as 课程名称 FROM SECS.dbo.SelectedCourse WHERE StuNo='"+Stu_No+"'");
    query.exec();           //当执行exec()后会把指针放在记录集中第一个记录之上
    query.last();           //指针放到最后一个记录
    nRow = query.at() + 1;  //获取指针所在行，确定数据的行数
    ui->tableWidget_SelectedCourse->setRowCount(nRow);          //在UI界面的tableWidget中新建立指定需要行数
    nColumn = ui->tableWidget_SelectedCourse->columnCount();    //获取列数，UI界面已经设置为1咧，实质上就是int常量1.
    query.first();          //指针放到最开始，开始获取数据
    while(j<nRow)           //双循环导入数据
    {
        for (i = 0; i<nColumn; i++)
        {
            ui->tableWidget_SelectedCourse->setItem(j, i, new QTableWidgetItem(query.value(i).toString()));
        }
        j++;
        query.next();   //下一格（行）数据  这里因为只有一列素以一格就是一行
    }

}

//获取选中行内容槽函数 Course
void Enter_Stu::on_tableView_Course_clicked(const QModelIndex &index)
{

    //获取选中行信息
    int row = ui->tableView_Course->currentIndex().row();           //获取当前选中行的行数
    QAbstractItemModel *model = ui->tableView_Course->model();
    QModelIndex view_index = model->index(row,1);                   //选中行第二列的索引
    QString name = model->data(view_index).toString();

    //赋值到标签
    ui->lable_CurrentCourse1->setText(name);
}

//获取选中行内容槽函数 SelectedCourse
void Enter_Stu::on_tableWidget_SelectedCourse_clicked(const QModelIndex &index)
{
    //获取选中行信息
    int row=ui->tableWidget_SelectedCourse->currentRow();
    QString str = ui->tableWidget_SelectedCourse->item(row,0)->text();

    //赋值到标签
    ui->lable_CurrentCourse2->setText(str);
}

//添加学生选课课程槽函数
void Enter_Stu::on_btn_AddMy_clicked()
{
    //判断是否选中课程
    if(ui->lable_CurrentCourse1->text()!="未选中...")
    {
        //取在数据库中是否存在的返回值
            //查询部分
        QSqlQueryModel *model=new QSqlQueryModel;
        QString sql="SELECT COUNT (*) FROM SECS.dbo.SelectedCourse WHERE StuNo='"+ui->label_No->text()+"' AND CourseName='"+ ui->lable_CurrentCourse1->text()+"'";
        model->setQuery(sql);
            //通过model处理部分
        QModelIndex index=model->index(0,0);            //返回指定的行和列的索引(index)  即表格的第一个格
        qDebug()<<index.data();

        //判断
        if(index.data()==0)
        {
        //课程不存在

            //添加到数据库中
            ExecSQL("INSERT INTO SECS.dbo.SelectedCourse VALUES ('"+Stu_No+"','"+Stu_Name+"','"+ ui->lable_CurrentCourse1->text()+"')");

            //修改数据库中选修人数 +1
            ExecSQL("UPDATE SECS.dbo.Course SET Number=Number+1 WHERE Name='"+ui->lable_CurrentCourse1->text()+"'");

            //刷新 重新载入
            on_btn_Fresh_clicked();

            //提示
            QMessageBox::about(NULL,"提示","选修该课程成功！");
        }
        else
        {
        //课程存在
            //提示
            QMessageBox::about(NULL,"提示","该课程已经选修！");

        }
    }
    else
    {
        QMessageBox::about(NULL,"提示","你还没有选择课程！");
    }



}

//删除学生选修课程槽函数
void Enter_Stu::on_btn_DelMy_clicked()
{
    //询问是否删除
    int choose=QMessageBox::question(this, tr("提示"),QString(tr("确认退选该门课程吗？")),QMessageBox::Yes | QMessageBox::No);

    //确认删除
    if (choose==QMessageBox::Yes)
    {
        //判断是否选中课程
        if(ui->lable_CurrentCourse2->text()!="未选中...")
        {
            //删除数据库中
            ExecSQL("DELETE FROM SECS.dbo.SelectedCourse WHERE StuNo='"+Stu_No+"' AND CourseName = '"+ui->lable_CurrentCourse2->text()+"'");

            //修改数据库中选修人数 -1
            ExecSQL("UPDATE SECS.dbo.Course SET Number=Number-1 WHERE Name='"+ui->lable_CurrentCourse2->text()+"'");

            //刷新 重新载入
            on_btn_Fresh_clicked();

            //重置状态
            ui->lable_CurrentCourse2->setText("未选中...");

            //提示成功
            QMessageBox::about(NULL,"提示","退选该课程成功！");
        }
        else
        {
            QMessageBox::about(NULL,"提示","你还没有选择课程！");
        }
    }


}


