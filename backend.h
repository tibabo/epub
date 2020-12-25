#pragma once

#include <iostream>
#include <QObject>
#include <QUrl>
#include <QString>
#include <qqml.h>
#include <QFileInfo>


using namespace std;

class SingletonEpubReader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int someProperty READ someProperty WRITE setSomeProperty NOTIFY somePropertyChanged)

public:
    explicit SingletonEpubReader(QObject* parent = nullptr) : QObject(parent) {}

    Q_INVOKABLE int doSomething()
    {
        setSomeProperty(5);
        return m_someProperty;
    }

    Q_INVOKABLE void openFile(const QUrl & filename);

    int someProperty() const { return m_someProperty; }
    void setSomeProperty(int val) {
        if (m_someProperty != val) {
            m_someProperty = val;
            emit somePropertyChanged(val);
        }
    }

signals:
    void somePropertyChanged(int newValue);

private:
    int m_someProperty = 0;
};
