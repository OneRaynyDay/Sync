#include <iostream>
#include <boost/filesystem.hpp>
#include "diff.h"
#include "walk.h"
#include "cp.h"
namespace fs = boost::filesystem;
/**
 * Just a sandbox. Disregard.
 */
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
    // std::cout << check::diff("test", "test") << std::endl;
    // std::cout << check::diff("test", "not a thing") << std::endl;
    // fs::path p("a/b/_4/b/_4/b/_4/1");
    // auto v = cp::realpath(p);
    // for(auto x : v){
        // std::cout << x << "..." << std::endl;
    // }
    // std::cout << get_content("Makefile") << std::endl;
    // fs::copy_directory("a", "b");
    // std::cout << fs::exists("a") << fs::exists("bbb") << std::endl;
    // for(auto it = p.begin(); it != p.end(); it++){
        // std::cout << *it << std::endl;
    // }
    // std::cout << fs::canonical("a/b/_4") << "\n" << fs::weakly_canonical("a/b/_4") << std::endl;
    // fs::create_symlink("a/10", "a/9");
    // std::cout << fs::relative("/Users/dev/home/cpp/sync/a/2", fs::path("a/b/_2").parent_path()) << std::endl;
    // fs::directory_iterator end = fs::directory_iterator();
    // for(fs::directory_iterator it("a/b"); it != end; ++it){
        // std::cout << ":O : " << *it << std::endl;
    // }
    // try{
        // walk::generate_walk( "", "", "a", "b");
    // }
    // catch(const char* s){
        // std::cout << s << std::endl;
    // }
    // std::cout << fs::exists(fs::absolute(fs::path(""))) << std::endl;
    // fs::create_directory("build/main.o");
    walk::generate_walk("a", "a/b/jump/f", "b");
}
