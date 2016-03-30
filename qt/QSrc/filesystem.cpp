#include "filesystem.h"

#include <algorithm>
#include <iostream>
#include <memory>
#include <sstream>

#include <boost/algorithm/string.hpp>

#include <QDebug>

using namespace std;

using namespace boost::filesystem;
using namespace boost::algorithm;

PtrPath OpenDirectory(const char* szDir)
{
    if (szDir == nullptr)
    {
        return nullptr;
    }

    auto pPath = std::make_unique<path>(szDir);

    try
    {
        if (!(exists(*pPath) && is_directory(*pPath)))
        {
            pPath = nullptr;
        }
    }
    catch (const filesystem_error& ex)
    {
        qInfo() << ex.what() << endl;
    }

    return pPath;
}

PtrVecPath FindAllPaths(PtrPath& pPath)
{
    PtrVecPath pPaths = make_unique<VecPath>();

    try
    {
        for (directory_entry& entry : recursive_directory_iterator(*pPath))
        {
            pPaths->push_back(entry.path());
        }
    }
    catch (const filesystem_error& ex)
    {
        qInfo() << ex.what() << endl;
    }

    return pPaths;
}

template<typename T>
void PrintCollection(const T& coll)
{
    for (const auto& elem : coll)
    {
        stringstream ss;
        ss << elem;
        qInfo() << ss.str().c_str();
    }
}

PtrSetString FindExtensions(path& p)
{
    auto setExtensions = make_unique<SetString>();

    try
    {
        for (auto& x : recursive_directory_iterator(p))
        {
            if (is_regular_file(x.path()))
            {
                setExtensions->insert(to_lower_copy(x.path().extension().string()));
            }
        }
    }
    catch (const filesystem_error& ex)
    {
        qInfo() << ex.what() << endl;
    }

    return setExtensions;
}

PtrVecEntry FindWritableFiles(path& p)
{
    auto vecFiles = make_unique<VecEntry>();

    try
    {
        for (directory_entry& entry : recursive_directory_iterator(p))
        {
            if (is_regular_file(entry.path()))
            {
                if (entry.status().permissions() & owner_write)
                {
                    vecFiles->push_back(entry);
                }
            }
        }
    }
    catch (const filesystem_error& ex)
    {
        qInfo() << ex.what() << endl;
    }

    return vecFiles;
}




