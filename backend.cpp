#include "backend.h"
#include "minizip/unzip.h"
#include <QXmlStreamReader>
#include <QDomDocument>

using namespace std;

void SingletonEpubReader::openFile(const QUrl & filurl)
{
    setcreator("");
    setpublishDate("");
    setpublisher("");
    settitle("");
    setlanguage("");

    string str = filurl.toLocalFile().toStdString();
    cout << str << endl;

    unsigned  char * buf;
    unsigned long long length;

    if(unZipOneFile((char*)"content.opf", &buf, &length, (char *)str.c_str()))
    {
        QXmlStreamReader xmlReader((const char*)buf);
        QDomDocument doc;
        doc.setContent(&xmlReader,false);
        QDomElement root=doc.documentElement();

        // Get root names and attributes
        QString Type=root.tagName();
        QString version=root.attribute("version","0.0");
        QString uuid=root.attribute("unique-identifier","");

        // Get the first child of the root (Markup metadata is expected)
        QDomElement Component=root.firstChild().toElement();

        // Loop while there is a child
        while(!Component.isNull())
        {
            // Check if the child tag name is metadata
            if (Component.tagName()=="metadata")
            {
                // Get the first child of the component
                QDomElement Child=Component.firstChild().toElement();

                // Read each child of the metadata node
                while (!Child.isNull())
                {
                    // Read Name and value
                    if (Child.tagName()=="dc:creator")
                    {
                        setcreator(Child.firstChild().toText().data());
                        cout << "   Creator = " << creator().toStdString().c_str() << endl;
                    }
                    if (Child.tagName()=="dc:title")
                    {
                        settitle(Child.firstChild().toText().data());
                        cout << "   Title = " << title().toStdString().c_str() << endl;
                    }
                    if (Child.tagName()=="dc:language")
                    {
                        setlanguage(Child.firstChild().toText().data());
                        cout << "   Language = " << language().toStdString().c_str() << endl;
                    }
                    if (Child.tagName()=="dc:publisher")
                    {
                        setpublisher(Child.firstChild().toText().data());
                        cout << "   Publisher = " << publisher().toStdString().c_str() << endl;
                    }
                    if (Child.tagName()=="dc:date")
                    {
                        setpublishDate(Child.firstChild().toText().data());
                        cout << "   Date = " << publishDate().toStdString().c_str() << endl;
                    }

                    // Next child
                    Child = Child.nextSibling().toElement();
                }
            }

            // Next component
            Component = Component.nextSibling().toElement();
        }
        free(buf);
    }
    else
    {
        cout << "content.opf not found" << endl;
    }

    valuesChanged();

    return;
}
