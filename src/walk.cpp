// Set to 0 if turn off debug
#define DEBUG(x) do { if(0) std::cerr << x << std::endl; } while (0)
#include "walk.h"
#include <iostream>
#include <cassert>

// TODO: Replace string throws with class throws

bool walk::check_bounds(const fs::path& root, const fs::path& p){
    return walk::weak_check_bounds(root, p) ||
        walk::weak_check_bounds(fs::weakly_canonical(root), p);
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
void walk::generate_walk(const fs::path& src, const fs::path& sp, const fs::path& dst){
    // TODO: delete this flush
    std::cout << std::unitbuf; 
    // If dst is INSIDE of src, or VICE VERSA, there is a 
    // chance for infinite loops. We disallow this.
    if(check_bounds(sp, dst) || check_bounds(dst, sp)) 
        throw "walk::generate_walk - src/dst is inside either. This will lead to infinite loops. Disallowed.";

    // Check for symlink cycles
    std::set<fs::path> visited;
    
    // We resolve src.
    DEBUG("sp : " << sp);
    std::vector<fs::path> roots = cp::realpath(sp);
    // Make sure nothing is outside of src.
    for (const auto& root_path : roots){
        DEBUG(root_path);
        if(!check_bounds(src, root_path))
            throw "walk::generate_walk - the sp has symlinks pointing to files outside of src.";
        // TODO: equivalent probably resolves symlinks
        if(fs::equivalent(root_path, sp))
            continue;
        auto dst_path = walk::replace_prefix(root_path, src, dst);
        cp::recursive_copy_entry(root_path, dst_path);
        // TODO: does visited need to also include all incremental paths to dst?
        visited.insert(dst_path);
    }

    DEBUG("recursion starting");
    // Copy all recursive entries. 
    std::function<void(const fs::path&, const fs::path&)> recurse_make = 
    [&](const fs::path& spath, const fs::path& dpath){
        if(!check_bounds(src, spath))
            throw "walk::generate_walk - the sp has symlinks pointing to files outside of src.";

        // Duplicate find
        if(visited.find(dpath) != visited.end())
            return;

        const auto& stat = fs::symlink_status(spath); 
        if(stat.type() == fs::file_not_found){
            throw "walk::generate_walk::recurse_make - error, could not read file";
        }

        cp::copy_entry(spath, dpath);
        visited.insert(dpath);
        // This is a leaf node.
        if(!fs::is_directory(stat))
            return;
        const auto& end = fs::directory_iterator();
        // Recursively call recurse_make
        for(fs::directory_iterator it(spath); it != end; ++it){
            const auto& next_spath = (*it).path();
            const auto& next_dpath = dpath / next_spath.filename();
            DEBUG("From " << dpath << " to " << next_dpath << "from " << spath << " to " << next_spath);
            recurse_make(next_spath, next_dpath);
        }
    };

    auto dp = walk::replace_prefix(sp, src, dst);
    DEBUG("DP:" << dp);
    recurse_make(sp, dp);
}
