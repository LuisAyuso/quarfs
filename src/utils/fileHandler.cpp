#include "fileHandler.h"

#include <assert.h>
#include <boost/filesystem/operations.hpp>




void FileHandler::registerHandle(const std::string& filename, FileListener* listener){

    // store listener for file
    auto& list = listeners[filename];
    list.push_back(listener);

    // store last modification
    boost::filesystem::path p(filename);
    assert(boost::filesystem::exists(p));
    timestamps[filename] = boost::filesystem::last_write_time( p );
}

void FileHandler::checkForModifications(){

    for (auto& pair : timestamps){
        boost::filesystem::path p(pair.first);
        if (boost::filesystem::exists(p)){
            auto time = boost::filesystem::last_write_time(p);
            if (pair.second < time){
                pair.second = time;
                for (auto& listener : listeners[pair.first]){
                    listener->notifiyModification(pair.first);
                }
            }
        }
    }
}

FileHandler& FileHandler::getInstance(){
    static FileHandler f;
    return f;
}

void updateFileHandler(){
    FileHandler::getInstance().checkForModifications();
}
