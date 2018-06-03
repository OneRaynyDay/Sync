#pragma once
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

// This module is mainly for checking which files need to be sync'd.
// TODO: support md5 hashing checks

namespace check{
// We use this function to determine whether a file has been
// changed or not(we compare to last write time,
//   and we use file sizes to determine)
// 
// In this case, we check whether p1 is the "same" file as p2.
bool diff(const fs::path& p1, const fs::path& p2);
}
