#ifndef UIPACK_AS_WIDGET_DOWNLOAD_H
#define UIPACK_AS_WIDGET_DOWNLOAD_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <socket_defines.h>

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

public:
    explicit UIpack_as_widget_download(QWidget *parent = nullptr);

    void uiInit();

signals:

};

#endif // UIPACK_AS_WIDGET_DOWNLOAD_H
