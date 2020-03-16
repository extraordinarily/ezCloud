#include "uipack_as_widget_login.h"

UIpack_as_widget_login::UIpack_as_widget_login(QWidget *parent) : QWidget(parent)
{
    this->uiInit();
}


void UIpack_as_widget_login::uiInit()
{
    this->setLayout(&vbl_main);

    vbl_main.addLayout(&fl_up);
    vbl_main.addLayout(&hbl_down);

    for(int i = 0; i < 3; i++)
    {
        fl_up.addRow(&lb_tips[i], &le_input[i]);
    }
    lb_tips[0].setText("主机ip");
    lb_tips[1].setText("用户名");
    lb_tips[2].setText("密码");

    hbl_down.addWidget(&pb_login);
    hbl_down.addWidget(&pb_register);

    pb_login.setText("登录");
    pb_register.setText("注册");

    return;
}
