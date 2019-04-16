#include "settinglist.h"
#include <QDebug>

SettingList::SettingList(QObject *parent) : QObject(parent)
{
  loadList();
}


bool SettingList::loadList()
{
    QFile loadFile(QDir::toNativeSeparators(QDir::homePath()+QStringLiteral("/kubelaunch.json")));

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QByteArray savedData = loadFile.readAll();

    QJsonDocument loadDoc( QJsonDocument::fromJson(savedData));

    kubeList = new QJsonObject(loadDoc.object());

    return true;
}

QJsonValue SettingList::getValue(const QString key, bool inList)
{
    if (inList)
    {
        if (kubeList->contains("list") && kubeList->value("list").toObject().contains(key) && kubeList->value("list").toObject().value(key).toObject().contains("args") )
        {
            return kubeList->value("list").toObject().value(key).toObject().value("args");
        }
    }
    else
    {
        if (kubeList->contains(key))
        {
            return kubeList->value(key);
        }
    }

    return false;

}

QJsonValue SettingList::setValue(const QString key,const QString value, bool inList)
{
    if (inList)
    {
        if (!kubeList->contains("list"))
        {
            kubeList->insert("list",QJsonValue::Object);
        }
//        kubeList->value("list").toObject().insert()
        kubeList->value("list").toObject().value(key).toObject().insert("args", value);
        qDebug() << kubeList->value("list").toObject().value(key).toObject().value("args");
    }
    else
    {
        kubeList->insert(key,value);
    }


    return false;

}

QStringList SettingList::getKeys()
{
    return  kubeList->value("list").toObject().keys();

}
