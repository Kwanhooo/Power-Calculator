#include "widget.h"
#include "ui_widget.h"
#include <iostream>

void EXchange(char B[], int len, double &result,bool &flag);
void Change(const char *S,char OPS[],int &len);

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    setFixedSize(this->width(), this->height());
    setWindowTitle("PowerCal");

    QPixmap iconPixmap(":images/icon.png");
    QIcon mainWindowIcon;
    mainWindowIcon.addPixmap(iconPixmap);
    this->setWindowIcon(mainWindowIcon);

    ui->output->setDisabled(true);

    QFont outputFont = this->ui->output->font();
    outputFont.setPixelSize(20);
    ui->output->setFont(outputFont);

    connect(ui->btn_0, SIGNAL(clicked()), this, SLOT(btn_0_clicked()));
    connect(ui->btn_1, SIGNAL(clicked()), this, SLOT(btn_1_clicked()));
    connect(ui->btn_2, SIGNAL(clicked()), this, SLOT(btn_2_clicked()));
    connect(ui->btn_3, SIGNAL(clicked()), this, SLOT(btn_3_clicked()));
    connect(ui->btn_4, SIGNAL(clicked()), this, SLOT(btn_4_clicked()));
    connect(ui->btn_5, SIGNAL(clicked()), this, SLOT(btn_5_clicked()));
    connect(ui->btn_6, SIGNAL(clicked()), this, SLOT(btn_6_clicked()));
    connect(ui->btn_7, SIGNAL(clicked()), this, SLOT(btn_7_clicked()));
    connect(ui->btn_8, SIGNAL(clicked()), this, SLOT(btn_8_clicked()));
    connect(ui->btn_9, SIGNAL(clicked()), this, SLOT(btn_9_clicked()));
    connect(ui->btn_add, SIGNAL(clicked()), this, SLOT(btn_add_clicked()));
    connect(ui->btn_minus, SIGNAL(clicked()), this, SLOT(btn_minus_clicked()));
    connect(ui->btn_multiply, SIGNAL(clicked()), this, SLOT(btn_multiply_clicked()));
    connect(ui->btn_divide, SIGNAL(clicked()), this, SLOT(btn_divide_clicked()));
    connect(ui->btn_mod,SIGNAL(clicked()),this,SLOT(btn_mod_clicked()));
    connect(ui->btn_l_paren,SIGNAL(clicked()),this,SLOT(btn_l_paren_clicked()));
    connect(ui->btn_r_paren,SIGNAL(clicked()),this,SLOT(btn_r_paren_clicked()));
    connect(ui->btn_dot,SIGNAL(clicked()),this,SLOT(btn_dot_clicked()));
    connect(ui->btn_backspace,SIGNAL(clicked()),this,SLOT(btn_backspace_clicked()));
    connect(ui->btn_c, SIGNAL(clicked()), this, SLOT(btn_c_clicked()));
    connect(ui->btn_equal, SIGNAL(clicked()), this, SLOT(btn_equal_clicked()));
}

Widget::~Widget()
{
    delete ui;
}

int index = 0;

char getNextChar(std::string exp)
{
    std::cout<<index<<std::endl;
    char ch = exp.at(index);
    index++;
    return ch;

}

bool isOperator(char c)
{
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' || c == '%' || c == '^' || c == '.' || c == '#')
        return true;
    else
        return false;
}

bool isValidInput(char c)
{
    if ((c >= '0' && c <= '9') || isOperator(c))
    {
        return true;
    }
    return false;
}

char ComparePriority(char a, char b)
{
    if (a == '+')
    {
        if (b == '*' || b == '/' || b == '(' || b == '^' || b == '%')
            return '<';
        else
            return '>';
    }
    else if (a == '-')
    {
        if (b == '*' || b == '/' || b == '(' || b == '^' || b == '%')
            return '<';
        else
            return '>';
    }

    else if (a == '*')
    {
        if (b == '(' || b == '^')
            return '<';
        else
            return '>';
    }
    else if (a == '/')
    {
        if (b == '(' || b == '^')
            return '<';
        else
            return '>';
    }
    else if (a == '%')
    {
        if (b == '(' || b == '^')
            return '<';
        else
            return '>';
    }
    else if (a == '^')
    {
        if (b == '(')
            return '<';
        else
            return '>';
    }
    else if (a == '(')
    {
        if (b == ')')
            return '=';
        else if (b == '#')
            return '!';
        else
            return '<';
    }
    else if (a == ')')
    {
        if (b == '(')
            return '!';
        else
            return '>';
    }
    else if (a == '#')
    {
        if (b == ')')
            return '!';
        if (b == '#')
            return '=';
        else
            return '<';
    }
}

double Calculate(double temp_1, double temp_2, char op)
{
    switch (op)
    {
    case '+':
        return temp_1 + temp_2;
    case '-':
        return 1.0 * temp_1 - temp_2;
    case '*':
        return temp_1 * temp_2;
    case '/':
        return 1.0 * temp_1 / temp_2;
    case '^':
        return 1.0 * pow(temp_1, temp_2);
    case '%':
        return static_cast<double>((int)temp_1 % (int)temp_2);
    }
}

bool ExpValidate(std::string exp)
{
    QString s;
    s.fromStdString(exp);
    if(!(s.contains('(')||s.contains(')')))
        return true;

    index = 0;
    QStack<char> validate_stack;
    for (index;index<exp.length();index++)
    {
        if(exp.at(index)=='(')
        {
            validate_stack.push(exp.at(index));
            continue;
        }
        if(exp.at(index)==')')
        {
            if(validate_stack.isEmpty())
                return false;
            if(validate_stack.top()=='(')
            {
                validate_stack.pop();
            }
        }
    }
    if(!validate_stack.isEmpty())
        return false;
    return true;
}

double Widget::getResult(std::string exp)
{
    if(!ExpValidate(exp))
    {
        QMessageBox::information(this,tr("错误"),tr("表达式不合法，请重新输入！"));
        return -3;
    }

    QStack<char> operator_stack;
    QStack<double> num_stack;
    QStack<double> temp_num_stack;

    double sum = 0;
    double digit;
    int exponent = 0;
    index = 0;

    //储存从符号栈(OperatorStack)中弹出的符号字符
    char operator_for_cal;
    //临时储存从数字栈(NumStack)中弹出的数字字符
    double left_num, right_num;

    operator_stack.push('#');

    char temp_char = getNextChar(exp);

    if (!isValidInput(temp_char))
    {
        return -10086.5;//错误输入
    }

    while (temp_char != '#' || operator_stack.top() != '#')
    {
        while (!isOperator(temp_char))
        {
            temp_num_stack.push(temp_char-'0');
            temp_char = getNextChar(exp);

            if (!isValidInput(temp_char))
            {
                QMessageBox::information(this,tr("错误"),tr("表达式不合法，请重新输入！"));
                return -10086.5;
            }
            if (temp_char == '.')
            {
                temp_char = getNextChar(exp);

                if (!isValidInput(temp_char)){
                    QMessageBox::information(this,tr("错误"),tr("表达式不合法，请重新输入！"));
                    return -10056.5;
                }

                int little_exp = -1;
                while (!isOperator(temp_char))
                {
                    sum += (temp_char - '0') * pow(10, little_exp);
                    little_exp--;
                    temp_char = getNextChar(exp);
                    if (!isValidInput(temp_char)){
                        QMessageBox::information(this,tr("错误"),tr("表达式不合法，请重新输入！"));
                        return -10086.5;
                    }
                }
            }
        }
        while (temp_num_stack.isEmpty() == false)
        {
            digit = temp_num_stack.pop();
            sum = sum + digit * pow(10, exponent);
            exponent++;
        }
        exponent=0;
        if (sum != 0)
        {
            num_stack.push((double)sum);
            sum = 0;
        }
        if (isOperator(temp_char))
        {
            switch (ComparePriority(operator_stack.top(), temp_char))
            {
            case '<':
                operator_stack.push(temp_char);
                temp_char = getNextChar(exp);
                if (!isValidInput(temp_char)){
                    QMessageBox::information(this,tr("错误"),tr("表达式不合法，请重新输入！"));
                    return -10086.5;
                }
                break;

            case '>':
                operator_for_cal = operator_stack.pop();
                right_num = num_stack.pop();
                left_num = num_stack.pop();
                num_stack.push(Calculate(left_num, right_num, operator_for_cal));
                break;

            case '=':
                operator_for_cal = operator_stack.pop();
                temp_char = getNextChar(exp);
                if (!isValidInput(temp_char)){
                    QMessageBox::information(this,tr("错误"),tr("表达式不合法，请重新输入！"));
                    return -10086.5;
                }
                break;

            case '!':
                QMessageBox::information(this,tr("错误"),tr("输入错误，符号不匹配！"));
                return -20086.5;
            }
        }
    }

    double ExpResult = num_stack.top();
    return ExpResult;

}

void Widget::btn_0_clicked()
{
    QString output = ui->output->text();
    output = output + "0";
    ui->output->setText(output);
}

void Widget::btn_1_clicked()
{
    QString output = ui->output->text();
    output = output + "1";
    ui->output->setText(output);
}

void Widget::btn_2_clicked()
{
    QString output = ui->output->text();
    output = output + "2";
    ui->output->setText(output);
}

void Widget::btn_3_clicked()
{
    QString output = ui->output->text();
    output = output + "3";
    ui->output->setText(output);
}

void Widget::btn_4_clicked()
{
    QString output = ui->output->text();
    output = output + "4";
    ui->output->setText(output);
}

void Widget::btn_5_clicked()
{
    QString output = ui->output->text();
    output = output + "5";
    ui->output->setText(output);
}

void Widget::btn_6_clicked()
{
    QString output = ui->output->text();
    output = output + "6";
    ui->output->setText(output);
}

void Widget::btn_7_clicked()
{
    QString output = ui->output->text();
    output = output + "7";
    ui->output->setText(output);
}

void Widget::btn_8_clicked()
{
    QString output = ui->output->text();
    output = output + "8";
    ui->output->setText(output);
}

void Widget::btn_9_clicked()
{
    QString output = ui->output->text();
    output = output + "9";
    ui->output->setText(output);
}

void Widget::btn_add_clicked()
{
    QString output = ui->output->text();
    if(!output.isEmpty()){
        if((!(output.at(output.length()-1)>='0'&&output.at(output.length()-1)<='9'))&&output.at(output.length()-1)!=')'&&output.at(output.length()-1)!='(')            return;
    }
    output = output + "+";
    ui->output->setText(output);
}

void Widget::btn_minus_clicked()
{
    QString output = ui->output->text();
    if(!output.isEmpty()){
        if((!(output.at(output.length()-1)>='0'&&output.at(output.length()-1)<='9'))&&output.at(output.length()-1)!=')'&&output.at(output.length()-1)!='(')            return;
    }
    output = output + "-";
    ui->output->setText(output);
}

void Widget::btn_multiply_clicked()
{
    QString output = ui->output->text();
    if(!output.isEmpty()){
        if((!(output.at(output.length()-1)>='0'&&output.at(output.length()-1)<='9'))&&output.at(output.length()-1)!=')'&&output.at(output.length()-1)!='(')        return;
    }
    output = output + "*";
    ui->output->setText(output);
}

void Widget::btn_divide_clicked()
{
    QString output = ui->output->text();
    if(!output.isEmpty()){
        if((!(output.at(output.length()-1)>='0'&&output.at(output.length()-1)<='9'))&&output.at(output.length()-1)!=')'&&output.at(output.length()-1)!='(')            return;
    }
    output = output + "/";
    ui->output->setText(output);
}

void Widget::btn_c_clicked()
{
    QString output = ui->output->text();
    output = "";
    ui->output->setText(output);
}

void Widget::btn_equal_clicked()
{
    QString expression = ui->output->text();


    if(expression.at(expression.length()-1)=='+'||expression.at(expression.length()-1)=='-'||expression.at(expression.length()-1)=='*'||expression.at(expression.length()-1)=='/'||expression.at(expression.length()-1)=='%'||expression.at(expression.length()-1)=='.')
    {
        QMessageBox::information(this,tr("错误"),tr("输入错误"));
        return;
    }
    std::string exp = expression.toStdString();
    exp+='#';
    std::cout<<exp<<std::endl;

    QString result = QString("%1").arg(getResult(exp));
    if(result=="-10086.5")
        ui->output->setText("Error Code: INVALID_INPUT");
    if(result=="-20086.5")
        ui->output->setText("Error Code: CANNOT_MATCH_OPERATOR");
    std::cout<<result.toStdString()<<std::endl;
    ui->output->setText(result);
}

void Widget::btn_backspace_clicked()
{
    QString output = ui->output->text();
    if(output.isEmpty())
        return;
    output = output.remove(output.length()-1,1);
    ui->output->setText(output);
}

void Widget::btn_l_paren_clicked()
{
    QString output = ui->output->text();
    output += "(";
    ui->output->setText(output);
}

void Widget::btn_r_paren_clicked()
{
    QString output = ui->output->text();
    output += ")";
    ui->output->setText(output);
}

void Widget::btn_mod_clicked()
{
    QString output = ui->output->text();
    if(!output.isEmpty()){
        if((!(output.at(output.length()-1)>='0'&&output.at(output.length()-1)<='9'))&&output.at(output.length()-1)!=')'&&output.at(output.length()-1)!='(')            return;
    }
    output += "%";
    ui->output->setText(output);
}

void Widget::btn_dot_clicked()
{
    QString output = ui->output->text();
    if(!output.isEmpty()){
        if((!(output.at(output.length()-1)>='0'&&output.at(output.length()-1)<='9'))&&output.at(output.length()-1)!=')'&&output.at(output.length()-1)!='(')
            return;
    }
    output += ".";
    ui->output->setText(output);
}

void Widget::on_btn_empty_clicked()
{
    this->close();
}

void Widget::on_btn_sound_clicked()
{
    QMessageBox::information(this,tr("Power Calculator"),tr("Power Calculator©\nA Simple but Powerful Calculator.\n\nTech Otakus Save the World\n\nDeveloped by Kwanho Lee.\nSee more on http://120.78.235.195/"));
}
