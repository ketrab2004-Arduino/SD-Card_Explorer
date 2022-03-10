#ifndef __EXPLORER_INFO_HOLDER__
#define __EXPLORER_INFO_HOLDER__

#include <SD.h>

struct FileInfoHolder
{
    bool isNULL = false;
    bool isDirectory = false;

    String folderPath;

    String name;
    uint32_t size;

    FileInfoHolder();
    FileInfoHolder(File file, String folder);

    File openFile();
    File openFile(uint8_t mode);

    operator bool();
};

#endif