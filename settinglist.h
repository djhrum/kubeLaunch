#ifndef SETTINGLIST_H
#define SETTINGLIST_H

#include <QObject>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QRandomGenerator>
#include <QDir>


class SettingList : public QObject
{
    Q_OBJECT
private:
    QJsonObject* kubeList;

public:
    explicit SettingList(QObject *parent = nullptr);
    bool loadList();
    QJsonValue getValue(const QString key, bool inList = false);
    QJsonValue setValue(const QString key, const QString value, bool inList = false);
    QStringList getKeys();

signals:

public slots:
};

#endif // SETTINGLIST_H
