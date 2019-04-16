#include "mainwindow.h"
#include <QDebug>

#include<QStyleFactory>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    createForm();
    createTrayIcon();
    loadConfig();

    setMinimumWidth(500);
    setWindowTitle("KubeLaunch Settings");

    qred.setNamedColor("red");
    qgreen.setNamedColor("green");
    qblack.setNamedColor("black");

}

Widget::~Widget()
{
 }


void Widget::createForm()
{

    kubePathLabel =  new QLabel("KubeCtl path:");
    kubePathEdit  =  new QLineEdit();
    kubePathButton = new QToolButton();
    kubePathButton->setText("...");
    kubePathButton->setToolTip("Finf kubectl in your filesystem");
    kubeRestart   =  new QCheckBox("Auto restart");
    kubeRestart->setToolTip("Auto restart kubectl each 1 minute");

    kubeList = new QTableWidget();
    kubeList->setColumnCount(2);
    kubeList->setSelectionMode(QAbstractItemView::SingleSelection);
    kubeList->setSelectionBehavior(QAbstractItemView::SelectRows);
    kubeList->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    kubeList->horizontalHeader()->setStretchLastSection(true);
    kubeList->setWordWrap(false);
    kubeList->setAlternatingRowColors(true);

    kubeAddRow = new QPushButton("Add Row");
    kubeDelRow = new QPushButton("Delete Row");
    kubeSave = new QPushButton("Save config");

    kubelogs = new LogWindow();

    restartTimer = new QTimer();

    QHBoxLayout *kubeLayout1 = new QHBoxLayout;
    kubeLayout1->addWidget(kubePathLabel);
    kubeLayout1->addWidget(kubePathEdit);
    kubeLayout1->addWidget(kubePathButton);
    kubeLayout1->addWidget(kubeRestart);

    QHBoxLayout *kubeLayout2 = new QHBoxLayout;
    kubeLayout2->addWidget(kubeSave);
    kubeLayout2->addStretch();
    kubeLayout2->addWidget(kubeAddRow);
    kubeLayout2->addWidget(kubeDelRow);


    QVBoxLayout *VLayout = new QVBoxLayout;
    VLayout->addLayout(kubeLayout1);
    VLayout->addWidget(kubeList);
    VLayout->addLayout(kubeLayout2);

    setLayout(VLayout);

    connect(kubeAddRow,SIGNAL(clicked()),this,SLOT(addRow()));
    connect(kubeDelRow,SIGNAL(clicked()),this,SLOT(delRow()));
    connect(kubeSave,SIGNAL(clicked()),this,SLOT(saveConfig()));
    connect(kubePathButton,SIGNAL(clicked()),this,SLOT(findKubectl()));
    connect(restartTimer,SIGNAL(timeout()),this,SLOT(restartAll()));

}


void Widget::createTrayIcon()
{
    trayIconMenu = new QMenu(this);

    connect(trayIconMenu, SIGNAL(triggered(QAction*)), this, SLOT(menuClicked(QAction*)));

    trayIcon = new QSystemTrayIcon(QIcon(":/logo-32"),this);
    trayIcon->setToolTip(tr("Kubectl launcher"));
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->show();
}


void Widget::addRow()
{
    kubeList->insertRow(kubeList->rowCount());
}


void Widget::delRow()
{
    QList<QTableWidgetSelectionRange> ranges = kubeList->selectedRanges();

    if ( ranges.isEmpty() )
    return;

    int begin = ranges.at(0).topRow(),
      count = ranges.at(0).rowCount();

    do
    {
        kubeList->removeRow(begin);
    } while ( --count );
}

void Widget::loadConfig()
{
    kubeConfig = new  QSettings(QDir::toNativeSeparators(QDir::homePath()+QStringLiteral("/kubelaunch.ini")), QSettings::IniFormat);

    kubePathEdit->setText(kubeConfig->value("system/kubepath","kubectl").toString());
    kubeRestart->setChecked(kubeConfig->value("system/kuberestart","").toBool());
    if (kubeRestart->isChecked())
    {
        restartTimer->start(5000);
    }

    QStringList header;
    header.append("name");
    header.append("args");

    kubeList->setHorizontalHeaderLabels(header);

    trayIconMenu->addAction(QIcon(":/gear"),tr("Settings"));
    trayIconMenu->addAction(QIcon(":/list"),tr("Logs"));

    kubeConfig->beginGroup("list");
    QStringList keys = kubeConfig->allKeys();
    kubeList->setRowCount(keys.count());


    for (int i = 0; i < keys.count(); ++i)
    {
        kubeList->setItem(i,0,new QTableWidgetItem(keys.at(i)));
        kubeList->setItem(i,1,new QTableWidgetItem(kubeConfig->value(keys.at(i)).toString()));

        trayIconMenu->addAction(QIcon(":/play"),"Run "+keys.at(i));
    }

    kubeConfig->endGroup();

    trayIconMenu->addSeparator();
    trayIconMenu->addAction(QIcon(":/close"),tr("Quit"));

}


void Widget::saveConfig()
{
    kubeConfig->setValue("system/kubepath",kubePathEdit->text());
    kubeConfig->setValue("system/kuberestart",kubeRestart->isChecked());

    kubeConfig->remove("list");

    for (int i= 0; i < kubeList->rowCount(); ++i)
    {
        if (kubeList->item(i,0) && kubeList->item(i,1))
        {
            kubeConfig->setValue("list/"+kubeList->item(i,0)->text(),kubeList->item(i,1)->text());
        }
        else
        {
            QMessageBox::critical(this,"Error","Cell in row "+QString::number(i)+" is empty.");
            return;
        }
    }

    kubeConfig->sync();

    reloadConfig();
}

void Widget::reloadConfig()
{

    killAll();
    kubectl.clear();
    trayIconMenu->clear();
    kubeList->clear();
    restartTimer->stop();

    loadConfig();
}

void Widget::findKubectl()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Set KubeCtl path"), "/", tr("Any Files (*)"));
    if (!fileName.isEmpty())
        kubePathEdit->setText(fileName);
}


void Widget::menuClicked(QAction* item)
{
    if ( item->text() == "Settings")
    {
        show();
    }
    else if ( item->text() == "Logs")
    {
        kubelogs->show();
    }
    else if ( item->text() == "Quit")
    {
        exitClick();
    }
    else
    {
        QStringList itemText = item->text().split(" ");
        QString action=itemText.first();
        itemText.removeFirst();
        QString name =  itemText.join(" ");

        if (action == "Run")
        {
            item->setIcon(QIcon(":/stop"));

            if (kubectl.contains(name))
            {
                kubectl[name]->kill();
                delete kubectl[name];
            }

            kubectl[name] = new QProcess();
            kubectl[name]->start(kubeConfig->value("system/kubepath").toString(),kubeConfig->value("list/"+name).toString().split(" "));

            connect(kubectl[name],SIGNAL(readyReadStandardOutput()),this,SLOT(getLogs()));
            connect(kubectl[name],SIGNAL(readyReadStandardError()),this,SLOT(getLogs()));
            connect(kubectl[name],SIGNAL(finished(int, QProcess::ExitStatus)),this,SLOT(getLogs(int, QProcess::ExitStatus)));

            item->setText("Stop "+name);
        }
        else if (action == "Stop")
        {
            item->setIcon(QIcon(":/play"));
            kubectl[name]->kill();
            item->setText("Run "+name);
        }
    }
}

void Widget::killAll()
{
    QHash<QString, QProcess*>::const_iterator i;
    for (i = kubectl.constBegin(); i != kubectl.constEnd(); ++i)
    {
      i.value()->kill();
      delete i.value();
    }
}

void Widget::restartAll()
{
    QHash<QString, QProcess*>::const_iterator i;
    for (i = kubectl.constBegin(); i != kubectl.constEnd(); ++i)
    {
      if (i.value()->state())
      {
        i.value()->close();
        Sleep(1000);
        i.value()->start();

//        kubelogs->logs->moveCursor(QTextCursor::End);
        kubelogs->logs->setTextColor(qgreen);
        kubelogs->logs->append("-- Process "+i.key()+" was restarted.");
      }
    }
}

void Widget::getLogs()
{
    QHash<QString, QProcess*>::const_iterator i;
    for (i = kubectl.constBegin(); i != kubectl.constEnd(); ++i)
    {
        QString errors = i.value()->readAllStandardError();
        QString outs = i.value()->readAllStandardOutput();

        if (!errors.isEmpty() or !outs.isEmpty())
        {
//            kubelogs->logs->moveCursor(QTextCursor::End);

            kubelogs->logs->setTextColor(qgreen);
            kubelogs->logs->append("<< "+i.key()+"");

            kubelogs->logs->setTextColor(qblack);
            kubelogs->logs->append(errors);

            kubelogs->logs->setTextColor(qred);
            kubelogs->logs->append(outs);
            QScrollBar* sb = kubelogs->logs->verticalScrollBar();
            sb->setValue(sb->maximum());
        }
    }
}

void Widget::getLogs(int code, QProcess::ExitStatus state)
{
//    kubelogs->logs->moveCursor(QTextCursor::End);
    kubelogs->logs->setTextColor(qred);
    kubelogs->logs->append("!! Process was finished with code: "+QString::number(code)+". And state: "+(state?"CrashExit":"NormalExit")+".");
}

void Widget::exitClick()
{
    killAll();
    trayIcon->hide();
    qApp->quit();
}
