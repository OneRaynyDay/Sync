// Set to 0 if turn off debug
#define DEBUG(x) do { if(1) std::cerr << x << std::endl; } while (0)
#include "cp.h"
#include <iostream>
#include <cassert>
#include <unistd.h>

void cp::handle_io_errors( const boost::system::error_code& ec){
    if(!ec.value()){
        return;
    }
    else if(ec.value() == errc::file_exists){
        DEBUG("File exist error found. Ignoring for now.");
    }
    else if(ec.value() == errc::permission_denied){
        DEBUG("Permission denied. Ignoring for now.");
    }
    else{
        DEBUG("Something is seriously wrong. Here's the error code: " << ec.value());
        throw std::system_error(ec.value(), std::system_category());
    }
}

// Returns all ABSOLUTE paths.
std::vector<fs::path> cp::realpath(const fs::path& p){
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
            root = fs::weakly_canonical(root);
        }
    }
    // Add the last file regardless of whether it's a symlink.
    s.insert(root);
    for(auto x : s)
        DEBUG(x);
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
     
    boost::system::error_code ec;
    DEBUG("Copying from " << from << " to " << to);
    if(fs::is_regular_file(stat)){
        // If the file does not yet exist (because of some symlinks)
        fs::copy_file(from, to, opt, ec);
    }
    else if(fs::is_symlink(stat)){
        // TODO: If a symlink points to itself, we should do something.
        // It will break all calls to weakly_canonical.
        // Change all absolute links into relative links. 
        fs::path ref = fs::read_symlink(from);
        if(ref.is_absolute()) 
            ref = fs::relative(ref, from.parent_path()); 
        // Change symlink referent of new symlink at to
        DEBUG("Symlink from : " << to << " pointing to " << ref);
        fs::create_symlink(ref, to, ec);

        // And then, add the referent IFF it is DIRECTORY THAT EXISTS.
        // fs::path canon_from_link = fs::weakly_canonical(from.parent_path() / ref);
        // fs::path canon_to_link = fs::weakly_canonical(to.parent_path() / ref);
        // DEBUG(canon_from_link << canon_to_link);
        // Because our objective is to copy the SYMLINK, we are not necessarily
        // obligated to copy the referent. If the referent already exists, then
        // we should not throw an error, and if it's a file, then it should get
        // picked up anyways later on.
        // TODO: HARD PROBLEM: COPY OR NOT REFERENT? ITS A BANDAID FOR NOW.
        // if(!fs::exists(canon_to_link)
                // && fs::exists(canon_from_link)
                // && fs::is_directory(canon_from_link)){
            // DEBUG("We are copying " << canon_from_link << " to " << canon_to_link);
            // cp::copy_entry(canon_from_link, canon_to_link);
        // }
    }
    else if(fs::is_directory(stat)){
        // If we don't supply a canonical path, then we will try to
        // overwrite directory symlinks that already exist
        // and this will throw.
        // Weakly canonical is used because to does not exist yet.
        fs::create_directories(fs::weakly_canonical(to), ec);
    }
    else{
        throw "cp::copy_entry - the file type is new and caused this crash.";
    }
    cp::handle_io_errors(ec);
    return true;
}

fs::path cp::get_base(const fs::path& from, const fs::path& to, fs::path& from_root, fs::path& to_root){
    // "zip" two paths and check every element is the same.
    fs::path absf = fs::absolute(from);
    fs::path abst = fs::absolute(to);
   
    // boost::filesystem iterators are incomplete; no base() and cannot get length. 
    auto rfit = absf.end(), rtit = abst.end();
    for(; rfit != absf.begin() && rtit != abst.begin(); --rfit, --rtit){
        if(*rfit != *rtit){
            ++rfit;
            ++rtit;
            break;
        }
    }
    // Iterate copies of the riters all the way back.
    auto fit = absf.begin(), tit = abst.begin();
    fs::path curf, curt;
    for(; fit != rfit; curf /= (*fit), fit++);
    for(; tit != rtit; curt /= (*tit), tit++);
    from_root = curf;
    to_root = curt;
    fs::path rest;
    for(; fit != absf.end(); rest /= (*fit), fit++);
    return rest;
}

// WARNING: If this copy operation fails at any point, then resources will be left over on disk.
// For example, we are copying from /a/b/1/2/3 into /x/y/z/1/2/3,
// then we need to back up 3 steps and then copy_entry recursively from there.
bool cp::recursive_copy_entry(const fs::path& from, const fs::path& to, fs::copy_option opt){
    std::cout << std::unitbuf;
    fs::path curf, curt;
    // Copy all contents starting from here
    auto common = cp::get_base(from, to, curf, curt);
    for(const auto& part : common){
        curf /= part;
        curt /= part;
        if(!cp::copy_entry(curf, curt, opt)){
            return false; // early terminate
        }
    } 
    return true;
}
