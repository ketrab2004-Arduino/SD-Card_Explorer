#include "FileInfoHolder.h"

FileInfoHolder::FileInfoHolder() {
    isNULL = true;
}
FileInfoHolder::FileInfoHolder(File file, String folder) {
    isDirectory = file.isDirectory();

    name = file.name();
    size = file.size();

    folderPath = folder;
}

File FileInfoHolder::openFile() {
    return SD.open((folderPath + "/" + name).c_str());
}
File FileInfoHolder::openFile(uint8_t mode) {
    return SD.open((folderPath + "/" + name).c_str(), mode);
}

FileInfoHolder::operator bool() {
    return !isNULL;
}
