#ifndef MAINWINDOW_WITH_TRAY_H
#define MAINWINDOW_WITH_TRAY_H

#include <QDialog>
#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>

class mainWindow_with_tray : public QMainWindow
{
    Q_OBJECT
public:
    explicit mainWindow_with_tray(QWidget *parent = nullptr);

private slots:
    void on_action_quit_triggered();
    void onSystemTrayIconClicked(QSystemTrayIcon::ActivationReason reason);
private:
    void closeEvent(QCloseEvent *event);
private:
    QAction *quitAction;
    QSystemTrayIcon *trayicon;
    QMenu *trayiconMenu;
};

#endif // MAINWINDOW_WITH_TRAY_H
