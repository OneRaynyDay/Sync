#pragma once
#include <boost/filesystem.hpp>
#include <vector>
#include <set>
namespace fs = boost::filesystem;

namespace walk{
// We need to resolve symlinks of a path, especially the initial path.
// We will reduce the path into a vector of paths that need to be included
// to accurately represent the symlink path.
//
// This takes care of recursive checks on symlinks.
std::vector<fs::path> realpath(const fs::path& p);

// We need to recursively walk two folders in parallel and create files
// if they don't yet exist.
void generate_walk(const fs::path& src, const fs::path& dst);
}
