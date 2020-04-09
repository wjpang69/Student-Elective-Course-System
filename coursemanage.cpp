#include "coursemanage.h"
#include "ui_coursemanage.h"

//构造函数
CourseManage::CourseManage(QWidget *parent) :QWidget(parent),ui(new Ui::CourseManage)
{
    ui->setupUi(this);
    initialize();
    on_btn_Fresh_clicked();
    connect(ui->lineEdit_Search,SIGNAL(textChanged(QString)),this,SLOT(search()));  //将字符变化的事件，与函数连接
}

//析构函数
CourseManage::~CourseManage()
{
    delete ui;
}

//初始化函数
void CourseManage::initialize()
{
    ui->tableView_Course->setSelectionBehavior(QAbstractItemView::SelectRows);                      //设置整行选择
    ui->tableView_Course->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);  //设置自适应宽度
    ui->tableView_Course->setSelectionMode(QAbstractItemView::SingleSelection);                     //设置每次只能一行
}

//搜索槽函数
void CourseManage::search()
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
void CourseManage::ExecSQL(QString sql)
{
    QSqlQueryModel *model=new QSqlQueryModel;       //QSqlQueryModel类为SQL的结果集提供了一个只读的数据模型
    model->setQuery(sql);                           //执行SQL语句
}

//刷新按钮槽函数
void CourseManage::on_btn_Fresh_clicked()
{
    QSqlQueryModel* model=new QSqlQueryModel;
    QString sql = "SELECT No as 课程代码 ,Name as 课程名称,Nature as 课程性质 ,Period as 总学时, Credit as 学分, Term as 开课学期,"
                  " Number as 选修人数 FROM SECS.dbo.Course";
    model->setQuery(sql);
    ui->tableView_Course->setModel(model);
}

//获取选中行信息槽函数
void CourseManage::on_tableView_Course_clicked(const QModelIndex &index)
{
    //获取选中行信息
    int row = ui->tableView_Course->currentIndex().row();           //获取当前选中行的行数
    QAbstractItemModel *model = ui->tableView_Course->model();
    QModelIndex view_index;

    view_index= model->index(row,0);
    ui->lineEdit_No->setText(model->data(view_index).toString());
    EditNo=ui->lineEdit_No->text();
    view_index = model->index(row,1);
    ui->lineEdit_Name->setText(model->data(view_index).toString());
    EditName=ui->lineEdit_Name->text();
    view_index = model->index(row,2);
    ui->lineEdit_Nature->setText(model->data(view_index).toString());
    view_index = model->index(row,3);
    ui->lineEdit_Period->setText(model->data(view_index).toString());
    view_index = model->index(row,4);
    ui->lineEdit_Credit->setText(model->data(view_index).toString());
    view_index = model->index(row,5);
    ui->lineEdit_Term->setText(model->data(view_index).toString());

}

//操作类型改变槽函数
void CourseManage::on_comboBox_Type_currentIndexChanged(int index)
{
    int type=ui->comboBox_Type->currentIndex();
    if(type==2){     //设置只读
        ui->lineEdit_No->setReadOnly(true);
        ui->lineEdit_Name->setReadOnly(true);
        ui->lineEdit_Nature->setReadOnly(true);
        ui->lineEdit_Period->setReadOnly(true);
        ui->lineEdit_Credit->setReadOnly(true);
        ui->lineEdit_Term->setReadOnly(true);
    }
    else
    {
        ui->lineEdit_No->setReadOnly(false);
        ui->lineEdit_Name->setReadOnly(false);
        ui->lineEdit_Nature->setReadOnly(false);
        ui->lineEdit_Period->setReadOnly(false);
        ui->lineEdit_Credit->setReadOnly(false);
        ui->lineEdit_Term->setReadOnly(false);
    }
}

//执行操作槽函数
void CourseManage::on_pushButton_Act_clicked()
{
    int type=ui->comboBox_Type->currentIndex();
    QString No=ui->lineEdit_No->text();
    QString Name=ui->lineEdit_Name->text();
    QString Nature=ui->lineEdit_Nature->text();
    QString Period=ui->lineEdit_Period->text();
    QString Credit=ui->lineEdit_Credit->text();
    QString Term=ui->lineEdit_Term->text();

    if(type==0) //添加
    {
        //取在数据库中是否存在的返回值
            //查询部分
        QSqlQueryModel *model=new QSqlQueryModel;
        QString sql="SELECT COUNT (*) FROM SECS.dbo.Course WHERE No='"+ No+"'";
        model->setQuery(sql);
            //通过model处理部分
        QModelIndex index=model->index(0,0);            //返回指定的行和列的索引(index)  即表格的第一个格
        //判断
        if(index.data()==0)
        {
         //课程不存在
            //不能为空
            if(No==""||Name==""||Nature==""||Period==""||Credit==""||Term=="")
            {
                QMessageBox::about(NULL,"提示","信息不能为空！");
                return;
            }
            //添加数据库中
            ExecSQL("INSERT INTO SECS.dbo.Course VALUES ('"+No+"', '"+Name+"', '"+Nature+"', '"+Period+"', '"+Credit+"', '"+Term+"', '0')");

            //刷新 重新载入
            on_btn_Fresh_clicked();

            //提示成功
            QMessageBox::about(NULL,"提示","添加该课程成功！");
        }
        else
        {
          //课程存在
             //提示
             QMessageBox::about(NULL,"提示","该课程已经存在！");
        }
    }
    else if(type==1) //修改
    {
        //询问是否修改
        int choose=QMessageBox::question(this, tr("提示"),QString(tr("确认修改课程吗？这个过程不可逆！")),QMessageBox::Yes | QMessageBox::No);

        //确认修改
        if (choose==QMessageBox::Yes)
        {
            //判断是否选中课程
            if(ui->lineEdit_No->text()!="")
            {
                //不能为空
                if(No==""||Name==""||Nature==""||Period==""||Credit==""||Term=="")
                {
                    QMessageBox::about(NULL,"提示","信息不能为空！");
                    return;
                }
                //判断No是否重复
                QSqlQueryModel *model=new QSqlQueryModel;
                model->setQuery("SELECT COUNT (*) FROM SECS.dbo.Course WHERE No='"+ No +"'");
                QModelIndex index=model->index(0,0);
                QString Query=index.data().toString();
                if(Query=="1")
                {
                    QMessageBox::about(NULL,"提示","课程代码已经存在！");
                    return;
                }
                //修改数据库表Course中
                ExecSQL("UPDATE SECS.dbo.Course SET No='"+No+"',Name='"+Name+"',Nature='"+Nature+"',"
                        "Period='"+Period+"',Credit='"+Credit+"',Term='"+Term+"' WHERE No='"+EditNo+"'");

                //修改数据库表SelectedCourse中
                ExecSQL("UPDATE SECS.dbo.SelectedCourse SET CourseName='"+Name+"' WHERE CourseName='"+EditName+"'");

                //刷新 重新载入
                on_btn_Fresh_clicked();

                //提示成功
                QMessageBox::about(NULL,"提示","修改该课程成功！");
            }
            else
            {
                QMessageBox::about(NULL,"提示","你还没有选择课程！");
            }
        }

    }
    else if(type==2) //删除
    {
        //询问是否删除
        int choose=QMessageBox::question(this, tr("提示"),QString(tr("确认删除课程吗？这个过程不可逆！")),QMessageBox::Yes | QMessageBox::No);

        //确认删除
        if (choose==QMessageBox::Yes)
        {
            //判断是否选中课程
            if(ui->lineEdit_No->text()!="")
            {
                //删除数据库Course表中
                ExecSQL("DELETE FROM SECS.dbo.Course WHERE No='"+ui->lineEdit_No->text()+"'");

                //删除数据库SelectedCourse表中
                ExecSQL("DELETE FROM SECS.dbo.SelectedCourse WHERE CourseName='"+ui->lineEdit_Name->text()+"'");

                //刷新 重新载入
                on_btn_Fresh_clicked();

                //重置状态
                ui->lineEdit_No->setText("");
                ui->lineEdit_Name->setText("");
                ui->lineEdit_Nature->setText("");
                ui->lineEdit_Period->setText("");
                ui->lineEdit_Credit->setText("");
                ui->lineEdit_Term->setText("");

                //提示成功
                QMessageBox::about(NULL,"提示","删除该课程成功！");
            }
            else
            {
                QMessageBox::about(NULL,"提示","你还没有选择课程！");
            }
        }

    }
}

//排序槽函数
void CourseManage::on_btn_Sort_clicked()
{
    int type=ui->comboBox_SortType->currentIndex();
    int method=ui->checkBox_SortMethod->isChecked();
    QSqlQueryModel* model=new QSqlQueryModel;
    QString sql;

    if (type==0)//选修人数
    {
        if(method==0)
        {
            sql = "SELECT No as 课程代码 ,Name as 课程名称,Nature as 课程性质 ,Period as 总学时, Credit as 学分, Term as 开课学期,"
                              " Number as 选修人数 FROM SECS.dbo.Course ORDER BY Number";
        }
        else if(method==1)
        {
            sql = "SELECT No as 课程代码 ,Name as 课程名称,Nature as 课程性质 ,Period as 总学时, Credit as 学分, Term as 开课学期,"
                              " Number as 选修人数 FROM SECS.dbo.Course ORDER BY Number DESC";
        }
    }
    else if(type==1)//课程代码
    {
        if(method==0)//升序
        {
            sql = "SELECT No as 课程代码 ,Name as 课程名称,Nature as 课程性质 ,Period as 总学时, Credit as 学分, Term as 开课学期,"
                              " Number as 选修人数 FROM SECS.dbo.Course ORDER BY No";
        }
        else if(method==1)//降序
        {
            sql = "SELECT No as 课程代码 ,Name as 课程名称,Nature as 课程性质 ,Period as 总学时, Credit as 学分, Term as 开课学期,"
                              " Number as 选修人数 FROM SECS.dbo.Course ORDER BY No DESC";
        }
    }
    model->setQuery(sql);
    ui->tableView_Course->setModel(model);
}
