#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QStack>
#include <QString>
#include <QMessageBox>
#include <QDebug>
#include <cmath>
#include <string>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
    static double result;

private slots:
    void btn_0_clicked();
    void btn_1_clicked();
    void btn_2_clicked();
    void btn_3_clicked();
    void btn_4_clicked();
    void btn_5_clicked();
    void btn_6_clicked();
    void btn_7_clicked();
    void btn_8_clicked();
    void btn_9_clicked();
    void btn_add_clicked();
    void btn_minus_clicked();
    void btn_multiply_clicked();
    void btn_divide_clicked();
    void btn_backspace_clicked();
    void btn_l_paren_clicked();
    void btn_r_paren_clicked();
    void btn_mod_clicked();
    void btn_dot_clicked();
    void btn_c_clicked();
    void btn_equal_clicked();
    void on_btn_empty_clicked();

    void on_btn_sound_clicked();

private:
    Ui::Widget *ui;
    double getResult(std::string exp);
};


#endif // WIDGET_H
