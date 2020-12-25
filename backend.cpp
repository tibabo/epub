#include "backend.h"
#include "minizip/unzip.h"

void SingletonEpubReader::openFile(const QUrl & filurl)
{
    std::string str = filurl.toLocalFile().toStdString();
    cout << str << endl;

    unsigned  char * buf;
    unsigned long long length;

    auto  f = unZipOneFile((char*)"content.opf", &buf, &length, (char *)str.c_str());
    return;
}
