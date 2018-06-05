#pragma once
#include <vector>
#include <set>
#include "walk.h"
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

// Some required forward decls:
namespace walk{
bool check_bounds(const fs::path& root, const fs::path& p);
}

// This module is for the bulk of the file and directory copying.
namespace cp{
// We need to resolve symlinks of a path, especially the initial path.
// We will reduce the path into a vector of paths that need to be included
// to accurately represent the symlink path.
//
// This takes care of recursive checks on symlinks.
std::vector<fs::path> realpath(const fs::path& p);

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

// TODO: test this
// Creates directories up to path.
// This tries to:
// 1. first see if the path currently exists.
// 2. If at any part path doesn't exist, make it.
bool recursive_copy_entry(const fs::path& src, const fs::path& dst, fs::copy_option opt = fs::copy_option::fail_if_exists);
}
