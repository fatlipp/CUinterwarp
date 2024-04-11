#include "core/runner/ptx_extractor.h"

#include <dlfcn.h>
#include <string>
#include <iostream>
#include <filesystem>

void SetPreload(const std::string& root)
{
    const char *ldPreload = std::getenv("LD_PRELOAD");
    std::string newEnv = root + "/libinterwarp_lib.so";

    if (ldPreload != nullptr)
    {
        newEnv += ldPreload;
    }
    setenv("LD_PRELOAD", newEnv.c_str(), true);
}

int main(int argc, char *argv[])
{
    std::cout << "Interwarp runner start()" << std::endl;

    if (argc < 2)
    {
        std::cout << "Set app to run. Example: `./interwarp_runner ./example_app`" << std::endl;

        return -1;
    }

    const auto exeDir = std::filesystem::weakly_canonical(std::filesystem::path(argv[0])).parent_path();
    SetPreload(exeDir);

    // parse args
    std::string app = argv[1];
    for (int i = 2; i < argc; ++i)
    {
        app += " ";
        app += argv[i];
    }

    // start app (TODO: a vulnerable code)
    const auto proc = popen(app.c_str(), "r");
    if (proc == nullptr)
    {
        std::cout << "App launch is failed\n";
        return -1;
    }

    // app log:
    char ch;
    while ((ch = fgetc(proc)) != EOF)
    {
        putchar(ch);
    }
    pclose(proc);

    std::cout << "Interwarp runner end()\n";

    return 0;
}