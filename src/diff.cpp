#include "diff.h"

bool check::diff(fs::path p1, fs::path p2){
    // One file exists and the other one no longer exists.
    // if both files don't exist, then this should throw.
    if (fs::exists(p1) ^ fs::exists(p2))
        return true;
    // Both files exist but are different types.
    if (fs::status(p1).type() != fs::status(p2).type())
        return true;
    // Files have different edit times.
    if (fs::last_write_time(p1) != fs::last_write_time(p2))
        return true;
    // Files have different sizes. (Only applicable to regular files)
    if (fs::is_regular_file(p1) && fs::file_size(p1) != fs::file_size(p2))
        return true;
    return false;
}
