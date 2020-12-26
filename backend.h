#pragma once

#include <iostream>
#include <QObject>
#include <QUrl>
#include <QString>
#include <qqml.h>
#include <QFileInfo>
#include <QDomDocument>

using namespace std;

#define STRING_PROPERTY(a) public : \
    QString a() const { return m_##a;} \
    private: \
        QString m_##a;

class SingletonEpubReader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString creator     READ creator      NOTIFY valuesChanged)
    Q_PROPERTY(QString publishDate READ publishDate  NOTIFY valuesChanged)
    Q_PROPERTY(QString publisher   READ publisher    NOTIFY valuesChanged)
    Q_PROPERTY(QString language    READ language     NOTIFY valuesChanged)
    Q_PROPERTY(QString title       READ title        NOTIFY valuesChanged)


    STRING_PROPERTY(creator)
    STRING_PROPERTY(publishDate)
    STRING_PROPERTY(publisher)
    STRING_PROPERTY(language)
    STRING_PROPERTY(title)

    signals:
        void valuesChanged();

  public:
    explicit SingletonEpubReader(QObject* parent = nullptr) : QObject(parent) {}


    Q_INVOKABLE void openFile(const QUrl & filename);
   private:
    bool findOpfFileInZip(const char * path, unsigned  char ** buffer , unsigned long long *length);
    void checkTag(const QDomElement & child, const QString & tag, QString & dest);
};
