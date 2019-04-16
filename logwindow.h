#ifndef LOGWINDOW_H
#define LOGWINDOW_H

#include <QWidget>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QPushButton>

class LogWindow : public QWidget
{
    Q_OBJECT
private:

    QPushButton* clearbutton;

public:
    explicit LogWindow(QWidget *parent = nullptr);

    QTextBrowser *logs;

signals:

public slots:
};

#endif // LOGWINDOW_H
