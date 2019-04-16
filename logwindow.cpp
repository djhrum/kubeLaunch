#include "logwindow.h"

LogWindow::LogWindow(QWidget *parent) : QWidget(parent)
{
    logs = new QTextBrowser();
    logs->setReadOnly(true);

    clearbutton = new QPushButton("Clear");

    connect(clearbutton,SIGNAL(clicked()),logs,SLOT(clear()));


    QHBoxLayout *hlayot = new QHBoxLayout();
    hlayot->addStretch();
    hlayot->addWidget(clearbutton);

    QVBoxLayout *mainlayot = new QVBoxLayout();
    mainlayot->addWidget(logs);
    mainlayot->addLayout(hlayot);
    setLayout(mainlayot);
    setMinimumSize(500,300);
    setWindowTitle("KubeLaunch Logs");
}
