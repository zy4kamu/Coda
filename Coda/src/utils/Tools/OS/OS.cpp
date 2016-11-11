#include "OS.h"
#include <fstream>

namespace Tools
{

static const string srcFolder = TOOLS_OS_SCRIPT_FOLDER;

size_t GetNumberOfFiles(const string& folder)
{
    string toRun = "sh ";
    toRun += srcFolder;
    toRun += "number-of-files.sh ";
    toRun += folder;
    toRun += " > /tmp/del";
    system(toRun.c_str());
    std::ifstream in("/tmp/del");
    size_t numFiles;
    in >> numFiles;
    in.close();
    return numFiles;
}

vector<string> GetFiles(const string& folder)
{
    string toRun = "sh ";
    toRun += srcFolder;
    toRun += "get-files.sh ";
    toRun += folder;
    toRun += " > /tmp/del";
    system(toRun.c_str());
    vector<string> files;
    string line;
    std::ifstream in("/tmp/del");
    while (getline(in, line))
    {
        files.emplace_back(std::move(line));
    }
    in.close();
    return files;
}

bool FileExists(const string& file)
{
    std::ifstream in(file);
    if (in) {
        in.close();
        return true;
    } else {
        return false;
    }
}

int RemoveFile(const string& file)
{
    string toRun = "sh ";
    toRun += srcFolder;
    toRun += "rm.sh ";
    toRun += file;
    toRun += " > /dev/null";
    return system(toRun.c_str());
}

}
