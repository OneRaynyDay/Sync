#include "cp.h"
#include <cassert>

bool cp::copy_entry(const fs::path& from, const fs::path& to, fs::copy_option opt){
    if(!fs::exists(from)){
        return false;
    }
    if(fs::is_regular(from)){
        fs::copy_file(from, to, opt);
    }
    else if(fs::is_symlink(from)){
        fs::copy_symlink(from, to);
    }
    else if(fs::is_directory(from)){
        fs::copy_directory(from, to);
    }
    else{
        return false;
    }
    return true;
}
