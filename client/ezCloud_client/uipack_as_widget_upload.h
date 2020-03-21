#ifndef UIPACK_AS_WIDGET_UPLOAD_H
#define UIPACK_AS_WIDGET_UPLOAD_H

#include <QObject>
#include <QWidget>
#include "socket_defines.h"

class UIpack_as_widget_upload : public QWidget
{
    Q_OBJECT
public:
    QVBoxLayout vbl_main;
    QHBoxLayout hbl_up;
    QHBoxLayout hbl_second;
    QHBoxLayout hbl_down;
    QTableView tbv_upload;

    QFormLayout fl_searchbox;
    QLineEdit le_search;
    QLabel lb_search;

    QPushButton pb_download;

    QPushButton pb_search;
    QPushButton pb_refresh;
    QPushButton pb_upload;
    QPushButton pb_delete;

    QPushButton pb_logout;


public:
    explicit UIpack_as_widget_upload(QWidget *parent = nullptr);

    void setDisable(bool);
    void uiInit();

signals:

};

#endif // UIPACK_AS_WIDGET_UPLOAD_H
