#pragma once
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

namespace check{
// We use this function to determine whether a file has been
// changed or not(we compare to last write time,
//   and we use file sizes to determine)
bool diff(fs::path p1, fs::path p2);
}
