#include "core/runner/ptx_extractor.h"

#include <string>
#include <sstring>

std::string Parse(const std::string& text)
{
    std::stringstream stream { text }; 

    bool isVersionGot = false;

    std::string word;
    while (stream >> word)
    {
        if (isVersionGot)
        {
            if (word == ".version" || word == "Fatbin")
                break;

            kernelDataText += word + " ";
        }
        else if (word == ".version")
        {
            ++fileIterator;
            std::string version = ".version " + word;
            std::string target;

            ++fileIterator;
            if (word == ".target")
            {
                ++fileIterator;

                if (word == "sm_86")
                {
                    target = ".target " + word;
                }
                else
                {
                    continue;
                }
            }
            else
            {
                break;
            }

            kernelDataText += version + " ";
            kernelDataText += target + " ";

            isVersionGot = true;
        }
        ++fileIterator;
    }
}