#pragma once
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

// This module is for the bulk of the file and directory copying.

namespace cp{
// We use this function to copy over the filesystem entry, regardless
// of whether it's a file, directory, symlink. However, it is obvious
// that we cannot trivially copy over any other type of filesystem entry, 
// so we ignore them without throwing errors.
//
// For symlinks specifically:
// we NEED to change the symlink destination, and one way to do this is to assume
// that the referent will also be copied into the correct location.
// We will change ALL ABSOLUTE LINKS INTO RELATIVE LINKS.
bool copy_entry(const fs::path& from, const fs::path& to, fs::copy_option opt = fs::copy_option::fail_if_exists);
}
