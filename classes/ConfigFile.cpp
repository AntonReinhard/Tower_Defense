#include "ConfigFile.h"
#include <fstream>
#include <string>
#include <map>

ConfigFile* gConfig_file = nullptr;

std::string trim(std::string const& source, char const* delims = " \t\r\n") {
    auto result(source);
    auto index = result.find_last_not_of(delims);
    if (index != std::string::npos)
        result.erase(++index);

    index = result.find_first_not_of(delims);
    if (index != std::string::npos)
        result.erase(0, index);
    else
        result.erase();
    return result;
}

ConfigFile::ConfigFile(std::string const& config_file) {
    std::ifstream file(config_file.c_str());

    std::string line;
    std::string in_section;
    while (std::getline(file, line)) {

        if (!line.length()) continue;

        if (line[0] == '#') continue;
        if (line[0] == ';') continue;

        if (line[0] == '[') {
            in_section = trim(line.substr(1, line.find(']') - 1));
            continue;
        }

        const int pos_equal = line.find('=');
        auto name = trim(line.substr(0, pos_equal));
        auto value = trim(line.substr(pos_equal + 1));

        std::string t = in_section;
        t.append("/").append(name);
        mContent_[t] = Chameleon(value);
    }
}

Chameleon const& ConfigFile::value(std::string const& section, std::string const& entry) const {
    const auto ci = mContent_.find(section + '/' + entry);

    if (ci == mContent_.end())
    {
        throw std::exception("does not exist");
    }
    return ci->second;
}

bool ConfigFile::value_exists(std::string const& section, std::string const& entry)
{
    const auto ci = mContent_.find(section + '/' + entry);
    return ci != mContent_.end();
}

bool ConfigFile::section_exists(std::string const& section)
{
    const auto ci = mContent_.find(section + '/');
    return ci != mContent_.end();
}


Chameleon ConfigFile::value_or_zero(std::string const& section, std::string const& entry)
{
    const auto ci = mContent_.find(section + '/' + entry);

    if (ci == mContent_.end())
    {
        Chameleon c;
        return c;
    }
    return ci->second;
}

Chameleon const& ConfigFile::value(std::string const& section, std::string const& entry, const double value) {
    try {
        return this->value(section, entry);
    }
    catch (const char *) {
        return mContent_.insert(std::make_pair(section + '/' + entry, Chameleon(value))).first->second;
    }
}

Chameleon const& ConfigFile::value(std::string const& section, std::string const& entry, std::string const& value) {
    try {
        return this->value(section, entry);
    }
    catch (const char *) {
        return mContent_.insert(std::make_pair(section + '/' + entry, Chameleon(value))).first->second;
    }
}