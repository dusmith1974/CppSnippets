#include <algorithm>
#include <iostream>
#include <memory>
#include <set>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

using namespace std;

using namespace boost::filesystem;
using namespace boost::algorithm;

using SetOfString = set<string>;
using VecOfEntry = vector<directory_entry>;
using PtrSetString = unique_ptr<SetOfString>;
using PtrVecEntry = unique_ptr<VecOfEntry>;

namespace
{
void PrintUsage()
{
    cout << "Usage: rehijack <root_dir>" << endl;
    exit(EXIT_FAILURE);
}
}  // namespace

unique_ptr<set<string>> FindExtensions(path& p)
{
    auto setExtensions = make_unique<set<string>>();

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
        cout << ex.what() << endl;
    }

    return setExtensions;
}

unique_ptr<vector<directory_entry>> FindWritableFiles(path& p)
{
    auto vecFiles = make_unique<vector<directory_entry>>();

    try
    {
        file_status rw(regular_file, owner_write);

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
        cout << ex.what() << endl;
    }

    return vecFiles;
}

template<typename T>
void PrintCollection(const T& coll)
{
    for (const auto& elem : coll)
    {
        std::cout << elem << endl;
    }
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        PrintUsage();
    }

    path p(argv[1]);
    try
    {
        if (!(exists(p) && is_directory(p)))
        {
            PrintUsage();
        }
    }
    catch (const filesystem_error& ex)
    {
        cout << ex.what() << endl;
    }

    PtrSetString setExtensions = FindExtensions(p);
    PtrVecEntry vecWritableFiles = FindWritableFiles(p);

    PrintCollection<SetOfString>(*setExtensions);
    PrintCollection<VecOfEntry>(*vecWritableFiles);

    return EXIT_SUCCESS;
}