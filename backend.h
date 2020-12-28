#pragma once

#include <iostream>
#include <QObject>
#include <QUrl>
#include <QString>
#include <qqml.h>
#include <QFileInfo>
#include <QDomDocument>
#include <QTemporaryDir>

using namespace std;

#define DOM_PROPERTY(a)                                               \
public:                                                               \
    QString a() const { return m_##a.firstChild().toText().data(); }  \
    void set##a(const QString &val) {                                 \
            m_##a.firstChild().setNodeValue(val);                     \
        qDebug() << QT_STRINGIFY(a) << " changed" << val;             \
    }                                                                 \
                                                                      \
private:                                                              \
    Q_PROPERTY(QString a READ a WRITE set##a NOTIFY valuesChanged)    \
    QDomElement m_##a;

#define STRING_PROPERTY(a)                               \
public :                                                 \
    QString a() const { return m_##a;}                   \
private:                                                 \
    Q_PROPERTY(QString a READ a NOTIFY valuesChanged)    \
    QString m_##a;


class SingletonEpubReader : public QObject
{
    Q_OBJECT

    DOM_PROPERTY(creator)
    DOM_PROPERTY(publishDate)
    DOM_PROPERTY(publisher)
    DOM_PROPERTY(language)
    DOM_PROPERTY(title)
    DOM_PROPERTY(description)
    STRING_PROPERTY(cover)

signals:
    void valuesChanged();

public:
    explicit SingletonEpubReader(QObject *parent = nullptr) : QObject(parent) {}
    Q_INVOKABLE void openFile(const QUrl &filename);
    Q_INVOKABLE bool save();

private:
    bool findOpfFileInZip(const char *path, unsigned char **buffer, unsigned long long *length);
    void checkTag(const QDomElement &child, const QString &tag, QDomElement &dest);
    void findMetadata(const QDomElement &component);
    void unzipCover(const QString & coverName, const QString & epubPath);
    void findCoverName(const QDomElement &component, QString &coverName);
    QTemporaryDir *     m_tmpDir = nullptr;
    QString             m_opfPath;
    QDomDocument        m_doc;
    QString             m_epubFile;
    QString             m_contentFilePath;
};
