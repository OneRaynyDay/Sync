#include "cp.h"
#include <iostream>
#include <cassert>
#include <unistd.h>

bool cp::copy_entry(const fs::path& from, const fs::path& to, fs::copy_option opt){
    if(!fs::exists(from)){
        return false;
    }
    // We need symlink_status instead of status
    // because status:stat::symlink_status:lstat
    auto stat = fs::symlink_status(from);
    if(fs::is_regular(stat)){
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
        fs::copy_directory(from, to);
    }
    else{
        return false;
    }
    return true;
}
