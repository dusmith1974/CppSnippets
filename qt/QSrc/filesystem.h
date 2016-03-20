#pragma once

#include <memory>
#include <set>
#include <vector>

#include <boost/filesystem.hpp>

using SetOfString = std::set<std::string>;
using PtrSetString = std::unique_ptr<SetOfString>;

using VecOfEntry = std::vector<boost::filesystem::directory_entry>;
using PtrVecEntry = std::unique_ptr<VecOfEntry>;

using Path = boost::filesystem::path;
using PtrPath = std::unique_ptr<Path>;

std::unique_ptr<boost::filesystem::path> OpenDirectory(const char* szDir);
PtrVecEntry FindAllEntries(PtrPath& path);
