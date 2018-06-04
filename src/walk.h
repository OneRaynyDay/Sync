#pragma once
#include <boost/filesystem.hpp>
#include <vector>
#include <set>
#include "cp.h"
namespace fs = boost::filesystem;

namespace walk{
// We need to resolve symlinks of a path, especially the initial path.
// We will reduce the path into a vector of paths that need to be included
// to accurately represent the symlink path.
//
// This takes care of recursive checks on symlinks.
std::vector<fs::path> realpath(const fs::path& p);

// Check if a path is inside root.
// Returns true if it is within bounds, false else.
// The definition of "within bounds" is a bit hard to define
// for symlinks; are we talking about the referent or the symlink?
//
// For this purpose, we define root to be its canonical path.
// And we also define p to be within bounds of p if:
// 1. p is in the path of root.
// 2. p is in the path of canonical(root).
//
// Example:
// canonical(SRC) -> SRC'
// Given some path SRC'/a, it is within bounds
// Given some path SRC/a, it is within bounds
// Given some path P/a, where canonical(P/a) = SRC'/a, 
//  it is NOT within bounds.

// Weak check bounds only checks naively whether p is in root.
bool weak_check_bounds(const fs::path& root, const fs::path& p);
// check_bounds employs the above algorithm in both
// canonical form and regular form.
bool check_bounds(const fs::path& root, const fs::path& p);

// Replace the prefix of a path given the src and dst.
// Assumes all inputs are absolute paths.
fs::path replace_prefix(const fs::path& p, const fs::path& orig_root, const fs::path& new_root);

// We need to recursively walk two folders in parallel and create files
// if they don't yet exist.
//
// POLICY REGARDING SYMLINKS:
// - If at any point we are out of bounds of src_root, then we throw an exception.
//
// Example:
// SRC/a/b/c -> DST/a/b/c
//
// SRC SIDE:
// SRC/a -> SRC/x
// SRC/x/b -> SRC/y
// SRC/y/c
//
// DST SIDE:
// DST/a -> DST/x
// DST/x/b -> DST/y
// DST/y/c
void generate_walk(const fs::path& src_root, const fs::path& dst_root, 
        const fs::path& src, const fs::path& dst);
}
