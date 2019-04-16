#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QtGui>
#include <QWidget>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QTableWidget>
#include <QToolButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDir>
#include <QStringList>
#include <QSettings>
#include <QPushButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QProcess>
#include <QHash>
#include <QScrollBar>
#include <QHeaderView>

//#include "settinglist.h"
//#include "qjsonmodel.h"
#include "logwindow.h"

class Widget : public QWidget
{
    Q_OBJECT

private:

    void createForm();
    void createTrayIcon();
    void loadConfig();
    void reloadConfig();
    void killAll();

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    QTableWidget* kubeList;
    QLabel*      kubePathLabel;
    QCheckBox*   kubeRestart;
    QLineEdit*   kubePathEdit;
    QToolButton* kubePathButton;
    QPushButton* kubeAddRow;
    QPushButton* kubeDelRow;
    QPushButton* kubeSave;

    QSettings *kubeConfig;
    QHash <QString, QProcess*> kubectl;
    QColor qred,qgreen,qblack;

    QTimer* restartTimer;

    LogWindow * kubelogs;


public:
    Widget(QWidget *parent = 0);
    ~Widget();

private slots:
//    void getdata();
//    void readout();
//    void readerr();
//    void trayClick(QSystemTrayIcon::ActivationReason activationReason);
    void addRow();
    void delRow();
    void saveConfig();
    void findKubectl();
    void restartAll();
//    void runProc(QString name);
    void menuClicked(QAction*);
//    void readItem();
//    void writePath();
    void getLogs();
    void getLogs(int code, QProcess::ExitStatus state);
    void exitClick();

};

#endif // MAINWINDOW_H
