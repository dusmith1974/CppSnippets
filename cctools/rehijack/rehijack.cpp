#include <iostream>
#include <boost/filesystem.hpp>

using namespace boost::filesystem;
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

    //std::cout << argv[1] << " " << file_size(argv[1]) << '\n';

    path p(argv[1]);

    try
    {
        if (exists(p) && is_directory(p))
        {
            for (auto& x : recursive_directory_iterator(p))
            {
                cout << "   " << x.path().filename().string() << std::endl;
            }
        }
        else
        {
            PrintUsage();
        }
    }
    catch (const filesystem_error& ex)
    {
        cout << ex.what() << std::endl;
    }

    return EXIT_SUCCESS;
}
