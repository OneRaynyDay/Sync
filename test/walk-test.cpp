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

// TEST_CASE( "walk::realpath correctly decomposes symlink paths.", "[walk::realpath]" ){
    // system("test/scripts/generate-tests.sh");
    // SECTION( "walk::realpath correctly gives a single entry for files." ){
        // fs::path p = fs::absolute(fs::path("a/1"));
        // auto v = walk::realpath(p);
        // REQUIRE(contains(v, fs::canonical("a/1")));
        // REQUIRE(v.size() == 1);
    // }
    // SECTION( "walk::realpath correctly resolves symlinks." ){
        // fs::path p = fs::absolute(fs::path("a/b/_4/b/_4/b/_4/1"));
        // auto v = walk::realpath(p);
        // REQUIRE(contains(v, fs::absolute("a/1")));
        // REQUIRE(contains(v, fs::absolute("a/b/_4")));
        // REQUIRE(v.size() == 2);
    // }
    // SECTION( "walk::realpath correctly stops at a symlink." ){
        // fs::path p = fs::absolute(fs::path("a/b/_4/b/_4/b/_4"));
        // auto v = walk::realpath(p);
        // REQUIRE(contains(v, fs::absolute("a")));
        // REQUIRE(contains(v, fs::absolute("a/b/_4")));
    // }
    // SECTION( "walk::realpath correctly stops at a directory." ){
        // fs::path p = fs::absolute(fs::path("a/b/_4/b/_4/b"));
        // auto v = walk::realpath(p);
        // REQUIRE(contains(v, fs::absolute("a/b")));
    // }
    // SECTION( "walk::realpath works for relative paths as well." ){
        // fs::path p("a/b/_4/b/_4/b/_4/1");
        // auto v = walk::realpath(p);
        // REQUIRE(contains(v, fs::absolute("a/1")));
        // REQUIRE(contains(v, fs::absolute("a/b/_4")));
    // }
    // system("test/scripts/cleanup.sh");
// }
//
// TEST_CASE( "walk::weak_check_bounds correctly checks bounds of files.", "[walk::weak_check_bounds]" ){
    // system("test/scripts/generate-tests.sh");
    // SECTION( "walk::weak_check_bounds checks a file is in a directory." ){
        // REQUIRE(walk::weak_check_bounds("a", "a/b"));
        // REQUIRE(walk::weak_check_bounds("a", "a/b/c"));
    // }
    // SECTION( "walk::weak_check_bounds checks a file is not in a directory." ){
        // REQUIRE(!walk::weak_check_bounds("a/b", "a"));
    // }
    // SECTION( "walk::weak_check_bounds checks a file is the directory." ){
        // REQUIRE(walk::weak_check_bounds("a/b", "a/b"));
    // }
    // system("test/scripts/cleanup.sh");
// }
//
// TEST_CASE( "walk::check_bounds correctly checks bounds of files with symlinks.", "[walk::check_bounds]" ){
    // system("test/scripts/generate-tests.sh");
    // SECTION( "walk::check_bounds checks a file is in a directory." ){
        // REQUIRE(walk::check_bounds("a", "a/b"));
        // REQUIRE(walk::check_bounds("a", "a/b/c"));
    // }
    // SECTION( "walk::check_bounds checks a file is not in a directory." ){
        // REQUIRE(!walk::check_bounds("a/b", "a"));
    // }
    // SECTION( "walk::check_bounds checks a file is the directory." ){
        // REQUIRE(walk::check_bounds("a/b", "a/b"));
    // }
    // SECTION( "walk::check_bounds checks a file is in canonical(directory)." ){
        // REQUIRE(walk::check_bounds("a/b/_4", "a"));
    // }
    // SECTION( "walk::check_bounds checks a file is appropriately not in canonical(file)." ){
        // REQUIRE(!walk::check_bounds("a/b/_1", "a"));
    // }
    // SECTION( "walk::check_bounds checks a file is appropriately not in absolute canonical(file)." ){
        // REQUIRE(!walk::check_bounds("a/b/_2", "a"));
    // }
    // system("test/scripts/cleanup.sh");
// }
//
// TEST_CASE( "walk::replace_prefix replaces the stem of a path.", "[walk::replace_prefix]" ){
    // system("test/scripts/generate-tests.sh");
    // SECTION( "walk::replace_prefix correctly replaces a root path." ){
        // REQUIRE(walk::replace_prefix("a/b/_2", "a", "b") == "b/b/_2");
    // }
    // SECTION( "walk::replace_prefix throws when a root path is not correct." ){
        // REQUIRE_THROWS(walk::replace_prefix("a/b/_2", "b", "b") == "b/b/_2");
    // }
    // SECTION( "walk::replace_prefix works on absolute paths, relative root." ){
        // REQUIRE(walk::replace_prefix(fs::absolute("a/b/_2"), "a", "b") == "b/b/_2");
    // }
    // SECTION( "walk::replace_prefix works on absolute root, relative paths." ){
        // REQUIRE(walk::replace_prefix("a/b/_2", fs::absolute("a"), "b") == "b/b/_2");
    // }
    // system("test/scripts/cleanup.sh");
// }

TEST_CASE( "walk::generate_walk successfully generates a copy.", "[walk::generate_walk]" ){
    system("test/scripts/generate-tests.sh");
    // SECTION( "walk::generate_walk syncs a to b." ){
        // walk::generate_walk("", "", "a", "b");
    // }
    // SECTION( "walk::generate_walk syncs a to absolute path b." ){
        // walk::generate_walk("", "", "a", fs::absolute("b"));
    // }
    // SECTION( "walk::generate_walk syncs absolute path a to b." ){
        // walk::generate_walk("", "", fs::absolute("a"), "b");
    // }
    // SECTION( "walk::generate_walk syncs a AND b as an absolute path." ){
        // walk::generate_walk("", "", fs::absolute("a"), fs::absolute("b"));
    // }
    // SECTION( "walk::generate_walk syncs (symlinks on path a) to b." ){
        // walk::generate_walk("a/b/_3/b/_4", "", fs::absolute("a/b"), fs::absolute("b"));
    // }
    SECTION( "walk::generate_walk syncs with weird jumping symlinks on path." ){
        // We want the resulting structure in b:
        // b/b/jump
        // b/b/c/d/e/f/1
        // b/b/c/d/e/f/g
        walk::generate_walk("a", "a/b/jump/f", "b");
    }
    // SECTION( "walk::generate_walk syncs a to (symlinks on path b)." ){
        // // TODO
    // }
    // SECTION( "walk::generate_walk rightfully rejects when symlinks point out of root." ){
        // walk::generate_walk("a/b", "", "a/b", "b");
    // }
    // system("test/scripts/cleanup.sh");
}
