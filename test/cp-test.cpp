#include "catch.hpp"
#include <iostream>
#include <fstream>
#include <streambuf>
#include <unistd.h>
#include <limits.h>

#include "../src/cp.h"

template <typename C, typename E>
bool contains(const C& v, const E& e){
    for(auto x : v){
        if (x == e)
            return true;
    }
    return false; 
}

std::string get_content(const std::string& fname){
    std::ifstream f(fname);
    return std::string( (std::istreambuf_iterator<char>(f)),
                        (std::istreambuf_iterator<char>()));
}

TEST_CASE( "cp::realpath correctly decomposes symlink paths.", "[cp::realpath]" ){
    system("test/scripts/generate-tests.sh");
    SECTION( "walk::realpath correctly gives a single entry for files." ){
        fs::path p = fs::absolute(fs::path("a/1"));
        auto v = cp::realpath(p);
        REQUIRE(contains(v, fs::canonical("a/1")));
        REQUIRE(v.size() == 1);
    }
    SECTION( "cp::realpath correctly resolves symlinks." ){
        fs::path p = fs::absolute(fs::path("a/b/_4/b/_4/b/_4/1"));
        auto v = cp::realpath(p);
        REQUIRE(contains(v, fs::absolute("a/1")));
        REQUIRE(contains(v, fs::absolute("a/b/_4")));
        REQUIRE(v.size() == 2);
    }
    SECTION( "cp::realpath correctly stops at a symlink." ){
        fs::path p = fs::absolute(fs::path("a/b/_4/b/_4/b/_4"));
        auto v = cp::realpath(p);
        REQUIRE(contains(v, fs::absolute("a")));
        REQUIRE(contains(v, fs::absolute("a/b/_4")));
    }
    SECTION( "cp::realpath correctly stops at a directory." ){
        fs::path p = fs::absolute(fs::path("a/b/_4/b/_4/b"));
        auto v = cp::realpath(p);
        REQUIRE(contains(v, fs::absolute("a/b")));
    }
    SECTION( "cp::realpath works for relative paths as well." ){
        fs::path p("a/b/_4/b/_4/b/_4/1");
        auto v = cp::realpath(p);
        REQUIRE(contains(v, fs::absolute("a/1")));
        REQUIRE(contains(v, fs::absolute("a/b/_4")));
    }
    system("test/scripts/cleanup.sh");
}

TEST_CASE( "cp::copy_entry correctly copies.", "[cp::copy_entry]" ){
    system("test/scripts/generate-tests.sh");
    SECTION( "cp::copy_entry correctly copies a file." ){
        REQUIRE(cp::copy_entry("a/2", "a/4"));
        REQUIRE(get_content("a/2") == "Hello world!\n");
        REQUIRE(get_content("a/4") == "Hello world!\n");
    }
    SECTION( "cp::copy_entry correctly copies a directory." ){
        REQUIRE(cp::copy_entry("a/b", "a/c"));
        REQUIRE(fs::is_directory("a/c"));
    }
    SECTION( "cp::copy_entry correctly copies a symlink." ){
        REQUIRE(cp::copy_entry("a/b/_4", "a/_4"));
        REQUIRE(fs::read_symlink("a/b/_4") == "..");
        REQUIRE(fs::read_symlink("a/_4") == "..");
    }
    SECTION( "cp::copy_entry correctly errors on a symlink that points to nothing." ){
        CHECK_THROWS(cp::copy_entry("a/b/_5", "a/_5"));
    }
    SECTION( "cp::copy_entry correctly takes an absolute symlink and turns it to relative." ){
        REQUIRE(cp::copy_entry("a/b/_2", "a/b/c/d/_2"));
        REQUIRE(fs::read_symlink("a/b/c/d/_2") == "../2");
    }
    SECTION( "cp::copy_entry correctly returns false for a device file." ){
        // Assumes /dev/urandom exists on this machine. Only OS X/Linux.
        REQUIRE(!cp::copy_entry("/dev/urandom", "a/_device"));
    }
    SECTION( "cp::copy_entry correctly does not overwrite a file." ){
        CHECK_THROWS(cp::copy_entry("a/1", "a/2"));
    }
    SECTION( "cp::copy_entry correctly does not overwrite a symlink." ){
        CHECK_THROWS(cp::copy_entry("a/b/_4", "a/b/_1"));
    }
    SECTION( "cp::copy_entry correctly returns an assertion error on nonexisting from." ){
        REQUIRE(!cp::copy_entry("a/NOTEXIST", "a/5"));
    }
    system("test/scripts/cleanup.sh");
}

// TODO: Add more tests
TEST_CASE( "cp::recursive_copy_entry correctly copies all incremental dirs.", "[cp::recursive_copy_entry]" ){
    system("test/scripts/generate-tests.sh");
    SECTION( "cp::recursive_copy_entry correctly copies a file nested within uncreated directories." ){
        REQUIRE(cp::recursive_copy_entry("a/b/c/d/e/f/1", "b/b/c/d/e/f/1"));
        REQUIRE(fs::exists("b/b/c/d/e/f/1"));
    }
    system("test/scripts/cleanup.sh");
}
