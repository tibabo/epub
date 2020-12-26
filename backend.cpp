#include "backend.h"
#include "minizip/unzip.h"

using namespace std;

// return extension

const char *pathExtension(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

// find first file wit opf extension and unzip it

bool SingletonEpubReader::findOpfFileInZip(const char * path, unsigned  char ** buffer , unsigned long long *length)
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

// check tag name and assign its value to corresponding string property

void SingletonEpubReader::checkTag(const QDomElement & child, const QString & tag, QString & dest)
{
    if (child.tagName() == tag)
    {
        dest = child.firstChild().toText().data();
        qDebug() << tag << " : " << dest;
    }
}

// Open epub file find opf, find metadata , assign to corresponding string property
// and signal to user interface

void SingletonEpubReader::openFile(const QUrl & filurl)
{
    // clear previous string properties

    m_creator = "";
    m_publishDate ="";
    m_publisher ="";
    m_title ="";
    m_language ="";

    string str = filurl.toLocalFile().toStdString();
    qDebug() << "openFile " << filurl.toLocalFile();

    unsigned char * buf;
    unsigned long long length;

    // Open epub and unzip opf file
    if(findOpfFileInZip(str.c_str(), &buf, &length))
    {
        QByteArray array((const char *)buf,(int)length);
        QDomDocument doc;
        doc.setContent(array,false);
        QDomElement root=doc.documentElement();

        // Get the first child of the root (Markup metadata is expected)
        QDomElement component=root.firstChild().toElement();

        // Loop while there is a child
        while(!component.isNull())
        {
            // Check if the child tag name is metadata
            if (component.tagName()=="metadata")
            {
                // Get the first child of the component
                QDomElement child=component.firstChild().toElement();

                // Read each child of the metadata node
                while (!child.isNull())
                {
                    checkTag(child,"dc:creator", m_creator);
                    checkTag(child,"dc:title", m_title);
                    checkTag(child,"dc:language", m_language);
                    checkTag(child,"dc:publisher", m_publisher);
                    checkTag(child,"dc:date", m_publishDate);

                    // Next child
                    child = child.nextSibling().toElement();
                }
            }

            // Next component
            component = component.nextSibling().toElement();
        }
        free(buf);
    }
    else
    {
        qDebug() << " *.opf not found";
    }

    // signal to user interface for update
    emit valuesChanged();

    return;
}
