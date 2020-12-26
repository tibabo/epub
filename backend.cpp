#include "backend.h"
#include "minizip/unzip.h"
#include <QDomDocument>

using namespace std;

const char *pathExtension(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

bool findOpfFile(const char * path, unsigned  char ** buffer , unsigned long long *length)
{
    int ret;
    zipFile zip = unzOpen(path);
    if(zip == NULL) return 0;

    unzGoToFirstFile(zip);
    do{
        unz_file_info fileInfo;
        memset(&fileInfo, 0, sizeof(unz_file_info));
        ret = unzOpenCurrentFile(zip);
        ret = unzGetCurrentFileInfo(zip, &fileInfo, NULL, 0, NULL, 0, NULL, 0);

        char *file = (char *)malloc(fileInfo.size_filename + 1);
        unzGetCurrentFileInfo(zip, &fileInfo, file, fileInfo.size_filename + 1, NULL, 0, NULL, 0);
        file[fileInfo.size_filename] = '\0';

        char *fname = NULL;

        char * subptr = strrchr (file, '/');
        if(subptr)
        {
            fname = subptr + 1;
        }
        else
        {
            fname = file;
        }

        if(strcasecmp(pathExtension(fname),"opf") == 0) {
            free(file);
            *buffer = (unsigned char *)malloc(fileInfo.uncompressed_size);
            *length = unzReadCurrentFile(zip, *buffer, fileInfo.uncompressed_size);
            if(*length<=0){
                printf("fail to extract zip %s\n",path);
            }
            break;
        }
        free(file);
        unzCloseCurrentFile( zip );
        ret = unzGoToNextFile( zip );
    } while(ret == UNZ_OK && ret != UNZ_END_OF_LIST_OF_FILE);

    if(ret == UNZ_END_OF_LIST_OF_FILE)
    {
        printf("fail to extract zip file %s\n",path);
        return false;
    }
    return true;
}

void SingletonEpubReader::openFile(const QUrl & filurl)
{
    setcreator("");
    setpublishDate("");
    setpublisher("");
    settitle("");
    setlanguage("");

    string str = filurl.toLocalFile().toStdString();
    cout << str << endl;

    unsigned char * buf;
    unsigned long long length;

    if(findOpfFile(str.c_str(), &buf, &length))
    {
        QByteArray array((const char *)buf,(int)length);
        QDomDocument doc;
        doc.setContent(array,false);
        QDomElement root=doc.documentElement();

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

    emit valuesChanged();

    return;
}
