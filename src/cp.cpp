#include "cp.h"
#include <iostream>
#include <cassert>
#include <unistd.h>

bool cp::copy_entry(const fs::path& _from, const fs::path& _to, fs::copy_option opt){
    const fs::path& from = fs::absolute(_from);
    const fs::path& to = fs::absolute(_to);
    // We need symlink_status instead of status
    // because status:stat::symlink_status:lstat
    const auto& stat = fs::symlink_status(from);
    // We're not doing exists(from) because it dereferences.
    if(stat.type() == fs::file_not_found || fs::is_other(stat))
        return false;
     
    std::cout << "Copying from " << from << " to " << to << std::endl;
    // If parent folder does not exist, then create it.
    const auto& parent_to = to.parent_path();
    if(!fs::exists(parent_to)){
        std::cout << parent_to << std::endl;
        fs::create_directories(parent_to);
    }

    if(fs::is_regular_file(stat)){
        fs::copy_file(from, to, opt);
    }
    else if(fs::is_symlink(stat)){
        // Change all absolute links into relative links. 
        fs::path ref = fs::read_symlink(from);
        if(ref.is_absolute()) 
            ref = fs::relative(ref, from.parent_path()); 
        // Change symlink referent of new symlink at to
        fs::create_symlink(ref, to);
    }
    else if(fs::is_directory(stat)){
        fs::create_directories(to);
    }
    else{
        throw "cp::copy_entry - the file type is new and caused this crash.";
    }
    return true;
}
