#include "uipack_as_widget_download.h"

UIpack_as_widget_download::UIpack_as_widget_download(QWidget *parent) : QWidget(parent)
{
    this->uiInit();
}


void UIpack_as_widget_download::uiInit()
{

    this->setLayout(&vbl_main);

    vbl_main.addLayout(&hbl_up);
    vbl_main.addWidget(&tbv_download);
    vbl_main.addLayout(&hbl_down);

    hbl_up.addLayout(&fl_searchbox);
    hbl_up.addWidget(&pb_search);
    hbl_up.addWidget(&pb_refresh);
    hbl_up.addStretch();
    hbl_up.addWidget(&pb_download);

    fl_searchbox.addRow(&lb_search, &le_search);

    hbl_down.addStretch();
    hbl_down.addWidget(&pb_close);


    pb_search.setText("搜索");
    pb_refresh.setText("刷新");
    pb_download.setText("下载");
    pb_close.setText("关闭");

    return;
}
