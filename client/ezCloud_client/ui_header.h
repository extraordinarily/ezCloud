#ifndef UI_HEADER_H
#define UI_HEADER_H

#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHeaderView>
#include <QTableView>
#include <QStandardItemModel>

class UIpack_as_widget_download : public QWidget
{
    Q_OBJECT
public:
    QVBoxLayout vbl_main;
    QHBoxLayout hbl_up;
    QHBoxLayout hbl_down;
    QTableView tbv_download;

    QFormLayout fl_searchbox;
    QLineEdit le_search;
    QLabel lb_search;

    QPushButton pb_download;
    QPushButton pb_search;
    QPushButton pb_refresh;
    QPushButton pb_close;

    explicit UIpack_as_widget_download(QWidget *parent = nullptr) : QWidget(parent)
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
    }
};

class UIpack_as_widget_login : public QWidget
{
    Q_OBJECT
public:
    QVBoxLayout vbl_main;
    QFormLayout fl_up;
    QHBoxLayout hbl_down;
    QLabel lb[3];
    QLineEdit ipAddr;
    QPushButton pb_login;
    QPushButton pb_register;

    explicit UIpack_as_widget_login(QWidget *parent = nullptr) : QWidget(parent)
    {
        this->setLayout(&vbl_main);

        vbl_main.addLayout(&fl_up);
        vbl_main.addLayout(&hbl_down);

        fl_up.addRow(&lb[0], &ipAddr);
        fl_up.addRow(&lb[1], &lb[2]);

        lb[0].setText("主机ip");
        lb[1].setText("设备状态：");

        hbl_down.addWidget(&pb_login);
        hbl_down.addWidget(&pb_register);

        pb_login.setText("登录");
        pb_register.setText("注册");
    }
};

class UIpack_as_widget_upload : public QWidget
{
    Q_OBJECT
public:
    QVBoxLayout vbl_main;
    QHBoxLayout hbl_second;
    QHBoxLayout hbl_down;
    QStandardItemModel model;
    QTableView tbv_upload;

    QFormLayout fl_searchbox;
    QLineEdit le_search;
    QLabel lb_search;

    QPushButton pb_refresh;
    QPushButton pb_upload;
    QPushButton pb_delete;
    QPushButton pb_download;

    int currentIndex;

    explicit UIpack_as_widget_upload(QWidget *parent = nullptr) : QWidget(parent)
    {
        this->setLayout(&vbl_main);

        vbl_main.addLayout(&hbl_second);
        vbl_main.addWidget(&tbv_upload);

        tbv_upload.resize(850,400);
        model.setHorizontalHeaderLabels({"filename","MD5","cookie","ip","length","zaixian"});
        tbv_upload.horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        tbv_upload.setSelectionBehavior(QAbstractItemView::SelectRows);
        tbv_upload.setModel(&model);

        vbl_main.addLayout(&hbl_down);

        hbl_second.addLayout(&fl_searchbox);
        hbl_second.addWidget(&pb_refresh);
        hbl_second.addStretch();
        hbl_second.addWidget(&pb_upload);
        hbl_second.addWidget(&pb_delete);
        hbl_second.addWidget(&pb_download);


        fl_searchbox.addRow(&lb_search, &le_search);

        hbl_down.addStretch();

        pb_refresh.setText("刷新");
        pb_upload.setText("上传");
        pb_delete.setText("删除");
        pb_download.setText("下载");
    }
};

#endif // UI_HEADER_H
