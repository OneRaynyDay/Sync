#include "catch.hpp"
#include "../src/diff.h"

TEST_CASE( "check::diff correctly diffs all file entry objects.", "[check::diff]" ){
    system("test/scripts/generate-tests.sh");
    SECTION( "check::diff diffs on files." ){
        REQUIRE(!check::diff("a/1", "a/1"));
        REQUIRE(!check::diff("a/2", "a/2"));
        REQUIRE(!check::diff("a/2", "a/3"));
        REQUIRE(check::diff("a/1", "a/2"));
        REQUIRE(check::diff("a/1", "a/DOESNOTEXIST"));
    }
    system("test/scripts/cleanup.sh");
}
