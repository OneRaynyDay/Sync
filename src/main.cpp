#include <iostream>
#include <boost/filesystem.hpp>
#include "diff.h"
namespace fs = boost::filesystem;

void listFiles(fs::path d){
    fs::directory_iterator end;
    for(fs::directory_iterator it(d); it != end; ++it){
        std::cout << it->path().filename() << std::endl;
    }
}


int main(){
    std::cout << check::diff("test", "test") << std::endl;
    std::cout << check::diff("test", "not a thing") << std::endl;
}

