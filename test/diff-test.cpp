#include "catch.hpp"
#include "../src/diff.h"

TEST_CASE( "diff correctly diffs all file entry objects.", "[check::diff]" ){
    system("scripts/generate-test.sh");
}
