#include "walk.h"
#include <iostream>

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
