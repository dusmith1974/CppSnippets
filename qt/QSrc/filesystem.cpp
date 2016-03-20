#include "filesystem.h"

#include <algorithm>
#include <iostream>
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

    std::unique_ptr<boost::filesystem::path> pPath = make_unique<boost::filesystem::path>(szDir);

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

PtrVecEntry FindAllEntries(PtrPath& pPath)
{
    PtrVecEntry pEntries = make_unique<VecOfEntry>();

    try
    {
        for (directory_entry& entry : recursive_directory_iterator(*pPath))
        {
            pEntries->push_back(entry);
        }
    }
    catch (const filesystem_error& ex)
    {
        qInfo() << ex.what() << endl;
    }

    return pEntries;
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
    auto setExtensions = make_unique<SetOfString>();

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
    auto vecFiles = make_unique<VecOfEntry>();

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




