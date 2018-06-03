#include "catch.hpp"
#include "../src/walk.h"
#include <boost/filesystem.hpp>
#include <iostream>

template <typename C, typename E>
bool contains(const C& v, const E& e){
    for(auto x : v){
        if (x == e)
            return true;
    }
    return false; 
}

template <typename C>
void print(const C& v){
    for(auto x : v){
        std::cout << x << std::endl;
    }
}

TEST_CASE( "walk::realpath correctly decomposes symlink paths.", "[walk::realpath]" ){
    system("test/scripts/generate-tests.sh");
    SECTION( "walk::realpath correctly gives a single entry for files." ){
        fs::path p = fs::absolute(fs::path("a/1"));
        auto v = walk::realpath(p);
        REQUIRE(contains(v, fs::canonical("a/1")));
        REQUIRE(v.size() == 1);
    }
    SECTION( "walk::realpath correctly resolves symlinks." ){
        fs::path p = fs::absolute(fs::path("a/b/_4/b/_4/b/_4/1"));
        auto v = walk::realpath(p); 
        REQUIRE(contains(v, fs::absolute("a/1")));
        REQUIRE(contains(v, fs::absolute("a/b/_4")));
        REQUIRE(v.size() == 2);
    }
    SECTION( "walk::realpath correctly stops at a symlink." ){
        fs::path p = fs::absolute(fs::path("a/b/_4/b/_4/b/_4"));
        auto v = walk::realpath(p); 
        REQUIRE(contains(v, fs::absolute("a")));
        REQUIRE(contains(v, fs::absolute("a/b/_4")));
    }
    SECTION( "walk::realpath correctly stops at a directory." ){
        fs::path p = fs::absolute(fs::path("a/b/_4/b/_4/b"));
        auto v = walk::realpath(p); 
        REQUIRE(contains(v, fs::absolute("a/b")));
    }
    SECTION( "walk::realpath works for relative paths as well." ){
        fs::path p("a/b/_4/b/_4/b/_4/1");
        auto v = walk::realpath(p); 
        REQUIRE(contains(v, fs::absolute("a/1")));
        REQUIRE(contains(v, fs::absolute("a/b/_4")));
    }
    system("test/scripts/cleanup.sh");
}
