#include "uipack_as_widget_upload.h"

UIpack_as_widget_upload::UIpack_as_widget_upload(QWidget *parent) : QWidget(parent)
{
    this->uiInit();
}


void UIpack_as_widget_upload::uiInit()
{
    this->setLayout(&vbl_main);

    vbl_main.addLayout(&hbl_up);
    vbl_main.addLayout(&hbl_second);
    vbl_main.addWidget(&tbv_upload);
    vbl_main.addLayout(&hbl_down);

    hbl_up.addWidget(&pb_download);
    hbl_up.addStretch();

    hbl_second.addLayout(&fl_searchbox);
    hbl_second.addWidget(&pb_search);
    hbl_second.addWidget(&pb_refresh);
    hbl_second.addStretch();
    hbl_second.addWidget(&pb_upload);
    hbl_second.addWidget(&pb_delete);

    fl_searchbox.addRow(&lb_search, &le_search);

    hbl_down.addStretch();
    hbl_down.addWidget(&pb_logout);

    pb_download.setText("下载页面");
    pb_search.setText("搜索");
    pb_refresh.setText("刷新");
    pb_upload.setText("上传");
    pb_delete.setText("删除");
    pb_logout.setText("登出");


    return;
}
