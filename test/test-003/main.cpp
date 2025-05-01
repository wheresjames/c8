/**
    This is the sample program from the V8 documentation.
*/

#include <iostream>
#include <cxxopts.hpp>


std::string printMap(const std::string& title, const std::map<std::string, std::string>& map)
{
    std::ostringstream oss;
    oss << title << ":\n";
    for (const auto& pair : map)
    {
        oss << "  " << pair.first << ": " << pair.second << "\n";
    }
    return oss.str();
}

int main(int argc, char* argv[])
{
    //----------------------------------------------------
    // Set up command line options

    cxxopts::Options options(APPNAME, APPDESCRIPTION);
    options.add_options()
        ("v,version",   "Print version" )
        ("h,help",      "Print usage"   )
        ;

    auto result = options.parse(argc, argv);
    std::map<std::string, std::string> args;
    for (auto& arg : result.arguments())
        args[arg.key()] = arg.value();

    if (args.count("help"))
    {   std::cout << options.help() << std::endl;
        return 0;
    }

    if (args.count("version"))
    {   std::cout << APPVERSION << " (" << APPBUILD << ")" << std::endl;
        return 0;
    }

    std::cout << printMap(APPNAME " Arguments", args) << std::endl;

    std::cout << "The END" << std::endl;

    return 0;
}
