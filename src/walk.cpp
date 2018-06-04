#include "walk.h"
#include <iostream>
#include <cassert>

// TODO: Replace string throws with class throws

// Returns all ABSOLUTE paths.
std::vector<fs::path> walk::realpath(const fs::path& p){
    // There is a possibility that the symlinks may be
    // cyclic. Therefore we need a set for uniqueness.
    std::set<fs::path> s;
    fs::path absp = fs::absolute(p);
    fs::path root;
    for(const auto& part : absp){
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
    fs::path absr = fs::absolute(root);
    fs::path absp = fs::absolute(p);
    auto rit = absr.begin(), pit = absp.begin();
    for(; rit != absr.end() && pit != absp.end(); ++rit, ++pit){
        if(*rit != *pit)
           return false; 
    }
    return rit == absr.end();
}

fs::path walk::replace_prefix(const fs::path& p, const fs::path& orig_root, const fs::path& new_root){
    const fs::path& absp = fs::absolute(p);
    const fs::path& abs_orig_root = fs::absolute(orig_root);
    fs::path cur;
    bool changed_root = false;
    for (const auto& part : absp){
        cur /= part;
        // TODO: Make sure equivalent here doesn't break symlinks.
        if(fs::equivalent(cur, abs_orig_root)){
            cur = new_root; 
            changed_root = true;
        }
    }
    if(!changed_root)
        throw "walk::replace_prefix - are you sure you inputted the orig_root & p correctly?";
    return cur;
}

// TODO: Add tests.
void walk::generate_walk(const fs::path& src, const fs::path& dst,
        const fs::path& sp, const fs::path& dp){
    // We resolve src.
    std::vector<fs::path> roots = walk::realpath(sp);
    // Make sure nothing is outside of src.
    for (const auto& root_path : roots){
        if(!check_bounds(src, root_path))
            throw "walk::generate_walk - the sp has symlinks pointing to files outside of src.";
        if(fs::equivalent(root_path, sp))
            continue;
        auto dst_path = walk::replace_prefix(root_path, src, dst);
        cp::copy_entry(root_path, dst_path);
    }

    // Copy all recursive entries.
    // TODO: Check for symlink cycles
    std::function<void(const fs::path&, const fs::path&)> recurse_make = 
    [&](const fs::path& spath, const fs::path& dpath){
        const auto& stat = fs::symlink_status(spath); 
        if(stat.type() == fs::file_not_found){
            throw "walk::generate_walk::recurse_make - error, could not read file";
        }
        std::cout << "COPYING : " << spath << " TO : " << dpath << std::endl;
        cp::copy_entry(spath, dpath);
        if(!fs::is_directory(stat))
            return;
        const auto& end = fs::directory_iterator();
        // Recursively call recurse_make
        for(fs::directory_iterator it(spath); it != end; ++it){
            const auto& next_spath = (*it).path();
            const auto& next_dpath = dpath / next_spath.filename();
            recurse_make(next_spath, next_dpath);
        }
    };
    recurse_make(sp, dp);
}
