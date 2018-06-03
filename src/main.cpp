#include <iostream>
#include <boost/filesystem.hpp>
#include "diff.h"
#include "walk.h"
namespace fs = boost::filesystem;

void listFiles(fs::path d){
    fs::directory_iterator end;
    for(fs::directory_iterator it(d); it != end; ++it){
        std::cout << it->path().filename() << std::endl;
    }
}

std::string get_content(const std::string& fname){
    std::ifstream f(fname);
    return std::string( (std::istreambuf_iterator<char>(f)),
                        (std::istreambuf_iterator<char>()));
}

int main(){
    std::cout << check::diff("test", "test") << std::endl;
    std::cout << check::diff("test", "not a thing") << std::endl;
    fs::path p("a/b/_4/b/_4/b/_4/1");
    auto v = walk::realpath(p);
    for(auto x : v){
        std::cout << x << "..." << std::endl;
    }
    std::cout << get_content("Makefile") << std::endl;
    // fs::copy_directory("a", "b");
    // std::cout << fs::exists("a") << fs::exists("bbb") << std::endl;
}
