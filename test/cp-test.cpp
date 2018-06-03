#include "catch.hpp"
#include <iostream>
#include <fstream>
#include <streambuf>
#include <unistd.h>
#include <limits.h>

#include "../src/cp.h"

std::string get_content(const std::string& fname){
    std::ifstream f(fname);
    return std::string( (std::istreambuf_iterator<char>(f)),
                        (std::istreambuf_iterator<char>()));
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
    SECTION( "cp::copy_entry correctly copies a symlink that points to nothing." ){
        REQUIRE(cp::copy_entry("a/b/_5", "a/_5"));
        REQUIRE(fs::read_symlink("a/b/_5") == "a/b/c/d");
        REQUIRE(fs::read_symlink("a/_5") == "a/b/c/d");
    }
    SECTION( "cp::copy_entry correctly takes an absolute symlink and turns it to relative." ){
        REQUIRE(cp::copy_entry("a/b/_2", "a/_2"));
        REQUIRE(fs::read_symlink("a/_2") == "../2");
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
