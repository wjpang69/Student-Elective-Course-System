#ifndef ENTER_ADMIN_H
#define ENTER_ADMIN_H

#include <QWidget>
#include "stumanage.h"
#include "coursemanage.h"
#include "datastatistics.h"

namespace Ui {
class Enter_Admin;
}

class Enter_Admin : public QWidget
{
    Q_OBJECT

public:
    explicit Enter_Admin(QWidget *parent = 0);
    ~Enter_Admin();

    void initialize();

private slots:
    void on_toolButton_Index_clicked();
    void on_toolButton_StudentManage_clicked();
    void on_toolButton_CourseManage_clicked();
    void on_toolButton_statistics_clicked();
    void on_toolButton_ExitSys_clicked();

private:
    Ui::Enter_Admin *ui;
};

#endif // ENTER_ADMIN_H
