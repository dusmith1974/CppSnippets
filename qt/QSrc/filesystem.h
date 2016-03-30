#pragma once

#include <memory>
#include <set>
#include <vector>

#include <boost/filesystem.hpp>

using SetString = std::set<std::string>;
using PtrSetString = std::unique_ptr<SetString>;

using VecEntry = std::vector<boost::filesystem::directory_entry>;
using PtrVecEntry = std::unique_ptr<VecEntry>;

using VecPath = std::vector<boost::filesystem::path>;
using PtrVecPath = std::unique_ptr<VecPath>;

using Path = boost::filesystem::path;
using PtrPath = std::unique_ptr<Path>;

std::unique_ptr<boost::filesystem::path> OpenDirectory(const char* szDir);
PtrVecPath FindAllPaths(PtrPath& path);
