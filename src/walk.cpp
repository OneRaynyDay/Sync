#include "walk.h"
#include <iostream>
#include <cassert>

std::vector<fs::path> walk::realpath(const fs::path& p){
    // There is a possibility that the symlinks may be
    // cyclic. Therefore we need a set for uniqueness.
    std::set<fs::path> s;
    fs::path absp = fs::absolute(p);
    fs::path root;
    for(auto& part : absp){
        root /= part; 
        // if the root currently is a symlink,
        // then we need to add it to our list.
        if (fs::is_symlink(root)){
            s.insert(root);
            // Now we start at the canonical path.
            root = fs::canonical(root);
        }
    }
    // Add the last file regardless of whether it's a symlink.
    s.insert(root);
    return std::vector<fs::path>(s.begin(), s.end());
}

bool walk::check_bounds(const fs::path& root, const fs::path& p){
    return walk::weak_check_bounds(root, p) ||
        walk::weak_check_bounds(fs::canonical(root), p);
}

bool walk::weak_check_bounds(const fs::path& root, const fs::path& p){
    // "zip" two paths and check every element is the same.
    auto rit = root.begin(), pit = p.begin();
    for(; rit != root.end() && pit != p.end(); ++rit, ++pit){
        if(*rit != *pit)
           return false; 
    }
    return rit == root.end();
}

// void walk::generate_walk(const fs::path& src, const fs::path& dst,
        // const fs::path& sp, const fs::path& dp){
    // // We resolve SRC.
    // std::vector<fs::path> roots = walk::realpath(src / sp);
    //
    // // Copy all entries of root first.
    // // TODO: Check for symlink cycles
// }
