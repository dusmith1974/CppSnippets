#include <algorithm>
#include <iostream>
#include <set>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

using namespace boost::filesystem;

using boost::algorithm::to_lower_copy;
using std::cout;

namespace
{
    void PrintUsage()
    {
        cout << "Usage: rehijack <root_dir>" << std::endl;
        exit(EXIT_FAILURE);
    }
}  // namespace

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        PrintUsage();
    }

    path p(argv[1]);
    std::set<std::string> setExtensions;

    try
    {
        if (exists(p) && is_directory(p))
        {
            for (auto& x : recursive_directory_iterator(p))
            {
                //cout << "   " << x.path().filename().string() << " " << x.path().extension() << std::endl;
                setExtensions.insert(to_lower_copy(x.path().extension().string()));
            }
        }
        else
        {
            PrintUsage();
        }

        std::for_each(setExtensions.begin(),
                      setExtensions.end(),
                      [](const std::string& strExtension) { cout << strExtension << std::endl; });
    }
    catch (const filesystem_error& ex)
    {
        cout << ex.what() << std::endl;
    }

    return EXIT_SUCCESS;
}
