#include "stumanage.h"
#include "ui_stumanage.h"

//构造函数
StuManage::StuManage(QWidget *parent) :QWidget(parent),ui(new Ui::StuManage)
{
    ui->setupUi(this);

    initialize();
    on_btn_Fresh_clicked();
    connect(ui->lineEdit_Search,SIGNAL(textChanged(QString)),this,SLOT(search()));  //将字符变化的事件，与函数连接
}

//析构函数
StuManage::~StuManage()
{
    delete ui;
}

//初始化函数
void StuManage::initialize()
{
    ui->tableView_Student->setSelectionBehavior(QAbstractItemView::SelectRows);                      //设置整行选择
    ui->tableView_Student->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);  //设置自适应宽度
    ui->tableView_Student->setSelectionMode(QAbstractItemView::SingleSelection);                     //设置每次只能一行
}

//搜索槽函数
void StuManage::search()
{
    QSqlQueryModel *model=new QSqlQueryModel;
    QString search=ui->lineEdit_Search->text();
    QString sql;
    if(ui->comboBox_SearchType->currentIndex()==0)
    {
        sql = "SELECT Name as 姓名, No as 学号, Sex as 性别, Age as 年龄, Major as 专业, Class as 班级,"
                      " ContactInfo as 联系方式,Pwd as 登录密码 FROM SECS.dbo.Student WHERE Name LIKE '%"+search+"%'";
    }
    else if(ui->comboBox_SearchType->currentIndex()==1)
    {
        sql = "SELECT Name as 姓名, No as 学号, Sex as 性别, Age as 年龄, Major as 专业, Class as 班级,"
                      " ContactInfo as 联系方式,Pwd as 登录密码 FROM SECS.dbo.Student WHERE No LIKE '%"+search+"%'";
    }
    model->setQuery(sql);
    ui->tableView_Student->setModel(model);
}

//执行SQL语句函数
void StuManage::ExecSQL(QString sql)
{
    QSqlQueryModel *model=new QSqlQueryModel;       //QSqlQueryModel类为SQL的结果集提供了一个只读的数据模型
    model->setQuery(sql);                           //执行SQL语句
}

//刷新按钮槽函数
void StuManage::on_btn_Fresh_clicked()
{
    QSqlQueryModel* model=new QSqlQueryModel;
    QString sql = "SELECT Name as 姓名, No as 学号, Sex as 性别, Age as 年龄, Major as 专业, Class as 班级,"
                  " ContactInfo as 联系方式,Pwd as 登录密码 FROM SECS.dbo.Student";
    model->setQuery(sql);
    ui->tableView_Student->setModel(model);
}

//获取选中行信息槽函数
void StuManage::on_tableView_Student_clicked(const QModelIndex &index)
{
    //获取选中行信息
    int row = ui->tableView_Student->currentIndex().row();           //获取当前选中行的行数
    QAbstractItemModel *model = ui->tableView_Student->model();
    QModelIndex view_index;

    view_index= model->index(row,0);
    ui->lineEdit_Name->setText(model->data(view_index).toString());
    view_index = model->index(row,1);
    ui->lineEdit_No->setText(model->data(view_index).toString());
    EditNo=ui->lineEdit_No->text();
    view_index = model->index(row,2);
    ui->lineEdit_Sex->setText(model->data(view_index).toString());
    view_index = model->index(row,3);
    ui->lineEdit_Age->setText(model->data(view_index).toString());
    view_index = model->index(row,4);
    ui->lineEdit_Major->setText(model->data(view_index).toString());
    view_index = model->index(row,5);
    ui->lineEdit_Class->setText(model->data(view_index).toString());
    view_index = model->index(row,6);
    ui->lineEdit_ContactInfo->setText(model->data(view_index).toString());
    view_index = model->index(row,7);
    ui->lineEdit_Pwd->setText(model->data(view_index).toString());

    //加载选修课程
    QSqlQueryModel* model2=new QSqlQueryModel;
    QString sql = "SELECT CourseName AS 选修课程 FROM SECS.dbo.SelectedCourse WHERE StuNo='"+ui->lineEdit_No->text()+"'";
    model2->setQuery(sql);
    ui->tableView_SelectedCourse->setModel(model2);

}

//操作类型改变槽函数
void StuManage::on_comboBox_ActType_currentIndexChanged(int index)
{
    int type=ui->comboBox_ActType->currentIndex();
    if(type==2){     //设置只读
        ui->lineEdit_Name->setReadOnly(true);
        ui->lineEdit_No->setReadOnly(true);
        ui->lineEdit_Sex->setReadOnly(true);
        ui->lineEdit_Age->setReadOnly(true);
        ui->lineEdit_Major->setReadOnly(true);
        ui->lineEdit_Class->setReadOnly(true);
        ui->lineEdit_ContactInfo->setReadOnly(true);
        ui->lineEdit_Pwd->setReadOnly(true);
    }
    else
    {
        ui->lineEdit_Name->setReadOnly(false);
        ui->lineEdit_No->setReadOnly(false);
        ui->lineEdit_Sex->setReadOnly(false);
        ui->lineEdit_Age->setReadOnly(false);
        ui->lineEdit_Major->setReadOnly(false);
        ui->lineEdit_Class->setReadOnly(false);
        ui->lineEdit_ContactInfo->setReadOnly(false);
        ui->lineEdit_Pwd->setReadOnly(false);
    }
}

//执行操作槽函数
void StuManage::on_btn_Act_clicked()
{
    int type=ui->comboBox_ActType->currentIndex();
    QString defaultPwd="123456";
    QString Name=ui->lineEdit_Name->text();
    QString No=ui->lineEdit_No->text();
    QString Sex=ui->lineEdit_Sex->text();
    QString Age=ui->lineEdit_Age->text();
    QString Major=ui->lineEdit_Major->text();
    QString Class=ui->lineEdit_Class->text();
    QString ContactInfo=ui->lineEdit_ContactInfo->text();
    QString Pwd=ui->lineEdit_Pwd->text();

    if(type==0) //添加
    {
        //取在数据库中是否存在的返回值
            //查询部分
        QSqlQueryModel *model=new QSqlQueryModel;
        QString sql="SELECT COUNT (*) FROM SECS.dbo.Student WHERE No='"+ No+"'";
        model->setQuery(sql);
            //通过model处理部分
        QModelIndex index=model->index(0,0);            //返回指定的行和列的索引(index)  即表格的第一个格
        //判断
        if(index.data()==0)
        {
         //学生不存在
            //不能为空
            if(Name==""||No==""||Sex==""||Age==""||Major==""||Class==""||ContactInfo=="")
            {
                QMessageBox::about(NULL,"提示","信息不能为空！");
                return;
            }
            //设置默认密码
            if(ui->lineEdit_Pwd->text()=="")    //默认密码123456
            {
                ui->lineEdit_Pwd->setText(defaultPwd);
            }
            //添加数据库中
            ExecSQL("INSERT INTO SECS.dbo.Student VALUES ('"+Name+"','"+No+"','"+Sex+"','"+Age+"','"+Major+"','"+Class+"','"+ContactInfo+"','"+Pwd+"')");

            //刷新 重新载入
            on_btn_Fresh_clicked();

            //提示成功
            QMessageBox::about(NULL,"提示","添加该学生成功！");
        }
        else
        {
          //学生存在
             //提示
             QMessageBox::about(NULL,"提示","该学生已经存在！");
        }
    }
    else if(type==1) //修改
    {
        //询问是否修改
        int choose=QMessageBox::question(this, tr("提示"),QString(tr("确认修改学生吗？这个过程不可逆！")),QMessageBox::Yes | QMessageBox::No);

        //确认修改
        if (choose==QMessageBox::Yes)
        {
            //不能为空
            if(Name==""||No==""||Sex==""||Age==""||Major==""||Class==""||ContactInfo=="")
            {
                QMessageBox::about(NULL,"提示","信息不能为空！");
                return;
            }
            //判断No是否重复
            QSqlQueryModel *model=new QSqlQueryModel;
            model->setQuery("SELECT COUNT (*) FROM SECS.dbo.Student WHERE No='"+ No +"'");
            QModelIndex index=model->index(0,0);
            QString Query=index.data().toString();
            if(Query=="1")
            {
                QMessageBox::about(NULL,"提示","学号已经存在！");
                return;
            }
            //设置默认密码
            if(ui->lineEdit_Pwd->text()=="")    //默认密码123456
            {
                ui->lineEdit_Pwd->setText(defaultPwd);
            }
            //判断是否选中学生
            if(ui->lineEdit_No->text()!="")
            {
                //修改数据库表Student中
                ExecSQL("UPDATE SECS.dbo.Student SET Name='"+Name+"',No='"+No+"',Sex='"+Sex+"',Age='"+Age+"',"
                        "Major='"+Major+"',Class='"+Class+"',ContactInfo='"+ContactInfo+"',Pwd='"+Pwd+"' WHERE No='"+EditNo+"'");

                //修改数据库表SelectedCourse中
                ExecSQL("UPDATE SECS.dbo.SelectedCourse SET StuName='"+Name+"', StuNo='"+No+"' WHERE StuNo='"+EditNo+"'");

                //刷新 重新载入
                on_btn_Fresh_clicked();

                //提示成功
                QMessageBox::about(NULL,"提示","修改该学生成功！");
            }
            else
            {
                QMessageBox::about(NULL,"提示","你还没有选择学生！");
            }
        }

    }
    else if(type==2) //删除
    {
        //询问是否删除
        int choose=QMessageBox::question(this, tr("提示"),QString(tr("确认删除学生吗？这个过程不可逆！")),QMessageBox::Yes | QMessageBox::No);

        //确认删除
        if (choose==QMessageBox::Yes)
        {
            //判断是否选中课程
            if(ui->lineEdit_No->text()!="")
            {

                //删除数据库Student表中
                ExecSQL("DELETE FROM SECS.dbo.Student WHERE No='"+ui->lineEdit_No->text()+"'");

                //删除数据库SelectedCourse表中
                ExecSQL("DELETE FROM SECS.dbo.SelectedCourse WHERE StuNo='"+ui->lineEdit_No->text()+"'");

                //修改Course表中选修人数
                QSqlQuery query;
                int j=0;
                int nRow;
                query.prepare("SELECT CourseName as 课程名称 FROM SECS.dbo.SelectedCourse WHERE StuNo='"+No+"'");
                query.exec();           //当执行exec()后会把指针放在记录集中第一个记录之上
                query.last();           //指针放到最后一个记录
                nRow=query.at() + 1;    //获取指针所在行，确定数据的行数
                query.first();          //指针放到最开始，开始获取数据
                while(j<nRow)           //循环获取数据并执行SQL
                {
                    ExecSQL("UPDATE SECS.dbo.Course SET Number=Number-1 WHERE Name='"+query.value(0).toString()+"'");
                    j++;
                    query.next();   //下一格（行）数据
                }

                //刷新 重新载入
                on_btn_Fresh_clicked();

                //重置状态
                ui->lineEdit_Name->setText("");
                ui->lineEdit_No->setText("");
                ui->lineEdit_Sex->setText("");
                ui->lineEdit_Age->setText("");
                ui->lineEdit_Major->setText("");
                ui->lineEdit_Class->setText("");
                ui->lineEdit_ContactInfo->setText("");
                ui->lineEdit_Pwd->setText("");

                QSqlQueryModel* model=new QSqlQueryModel;       //新实例化一个空的model覆盖原来显示的，达到清空效果
                model=NULL;
                ui->tableView_SelectedCourse->setModel(model);


                //提示成功
                QMessageBox::about(NULL,"提示","删除该学生成功！");
            }
            else
            {
                QMessageBox::about(NULL,"提示","你还没有选择学生！");
            }
        }

    }
}


