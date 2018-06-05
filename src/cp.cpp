// Set to 0 if turn off debug
#define DEBUG(x) do { if(0) std::cerr << x << std::endl; } while (0)
#include "cp.h"
#include <iostream>
#include <cassert>
#include <unistd.h>

// Returns all ABSOLUTE paths.
std::vector<fs::path> cp::realpath(const fs::path& p){
    // There is a possibility that the symlinks may be
    // cyclic. Therefore we need a set for uniqueness.
    std::set<fs::path> s;
    fs::path absp = fs::absolute(p);
    DEBUG("Absolute root path : " << absp << " from " << p);
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

bool cp::copy_entry(const fs::path& _from, const fs::path& _to, fs::copy_option opt){
    std::cout << std::unitbuf;
    const fs::path& from = fs::absolute(_from);
    const fs::path& to = fs::absolute(_to);
    // We need symlink_status instead of status
    // because status:stat::symlink_status:lstat
    const auto& stat = fs::symlink_status(from);
    // We're not doing exists(from) because it dereferences.
    if(stat.type() == fs::file_not_found || fs::is_other(stat))
        return false;
     
    // DEBUG("Copying from " << from << " to " << to);
    if(fs::is_regular_file(stat)){
        fs::copy_file(from, to, opt);
    }
    else if(fs::is_symlink(stat)){
        DEBUG("It's a symlink" << to);
        // Change all absolute links into relative links. 
        fs::path ref = fs::read_symlink(from);
        if(ref.is_absolute()) 
            ref = fs::relative(ref, from.parent_path()); 
        // Change symlink referent of new symlink at to
        fs::create_symlink(ref, to);
        // And then, add the referent IFF it is DIRECTORY THAT EXISTS.
        fs::path canon_from_link = fs::weakly_canonical(from.parent_path() / ref);
        fs::path canon_to_link = fs::weakly_canonical(to.parent_path() / ref);
        // DEBUG(canon_from_link << canon_to_link);
        // Because our objective is to copy the SYMLINK, we are not necessarily
        // obligated to copy the referent. If the referent already exists, then
        // we should not throw an error, and if it's a file, then it should get
        // picked up anyways later on.
        // TODO: HARD PROBLEM: COPY OR NOT REFERENT? ITS A BANDAID FOR NOW.
        if(!fs::exists(canon_to_link) 
                && fs::exists(canon_from_link) 
                && fs::is_directory(canon_from_link)){
            DEBUG("We are copying " << canon_from_link << " to " << canon_to_link);
            cp::copy_entry(canon_from_link, canon_to_link);
        }
    }
    else if(fs::is_directory(stat)){
        // If we don't supply a canonical path, then we will try to
        // overwrite directory symlinks that already exist
        // and this will throw.
        // Weakly canonical is used because to does not exist yet.
        DEBUG("Before weakly canonicalizing dir" << to);
        fs::create_directories(fs::weakly_canonical(to));
    }
    else{
        throw "cp::copy_entry - the file type is new and caused this crash.";
    }
    return true;
}

// WARNING: If this copy operation fails at any point, then resources will be left over on disk.
// For example, we are copying from /a/b/1/2/3 into /x/y/z/1/2/3,
// then we need to back up 3 steps and then copy_entry recursively from there.
bool cp::recursive_copy_entry(const fs::path& from, const fs::path& to, fs::copy_option opt){
    // "zip" two paths and check every element is the same.
    fs::path absf = fs::absolute(from);
    fs::path abst = fs::absolute(to);
   
    // boost::filesystem iterators are incomplete; no base() and cannot get length. 
    std::size_t backup = 0, len = 0;
    auto rfit = absf.rbegin(), rtit = abst.rbegin();
    for(bool done = false; rfit != absf.rend() && rtit != abst.rend(); ++rfit, ++rtit, len++){
        if(*rfit != *rtit)
            done = true;
        if(!done)
            backup++;
    }

    // Iterate both forward iterators until end - backup
    auto fit = absf.begin(), tit = abst.begin();
    fs::path curf, curt;
    for(std::size_t i = 0; i < len-backup; curf /= (*fit), i++, fit++);
    for(std::size_t i = 0; i < len-backup; curt /= (*tit), i++, tit++);

    // Copy all contents starting from here
    for(std::size_t i = 0; i < backup; i++, fit++, tit++){
        curf /= (*fit);
        curt /= (*tit);
        if(!cp::copy_entry(curf, curt, opt))
            return false; // early terminate
    } 
    return true;
}
