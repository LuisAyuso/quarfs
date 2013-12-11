#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <ctime>

struct FileListener{
    virtual void notifiyModification(const std::string& filename)=0;
};


class FileHandler{

        std::map<std::string, std::vector<FileListener*>> listeners;
        std::map<std::string, std::time_t> timestamps;

        FileHandler()=default;
        FileHandler(const FileHandler&)=delete;
        FileHandler(FileHandler&&)=delete;
        FileHandler operator=(const FileHandler&)=delete;
        FileHandler operator=(FileHandler&&)=delete;

    public:

        void registerHandle(const std::string& filename, FileListener* list);
        void checkForModifications();

        static FileHandler& getInstance();
};

void updateFileHandler();
