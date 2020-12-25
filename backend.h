#pragma once

#include <iostream>
#include <QObject>
#include <QUrl>
#include <QString>
#include <qqml.h>
#include <QFileInfo>


using namespace std;

#define STRING_PROPERTY(a) public : \
    QString a() const { return m_##a;} \
    void set##a(const QString & val) { \
        if (m_##a != val) { \
            m_##a = val; \
        } \
    } \
    private: \
        QString m_##a;

class SingletonEpubReader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString creator     READ creator     WRITE setcreator     NOTIFY valuesChanged)
    Q_PROPERTY(QString publishDate READ publishDate WRITE setpublishDate NOTIFY valuesChanged)
    Q_PROPERTY(QString publisher   READ publisher   WRITE setpublisher   NOTIFY valuesChanged)
    Q_PROPERTY(QString language    READ language    WRITE setlanguage    NOTIFY valuesChanged)
    Q_PROPERTY(QString title       READ title       WRITE settitle       NOTIFY valuesChanged)


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

};
