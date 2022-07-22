#pragma once
#include <string>
#include <map>

#include "Chameleon.h"

class ConfigFile {
    std::map<std::string, Chameleon> mContent_;

public:
    explicit ConfigFile(std::string const& config_file);

    Chameleon const& value(std::string const& section, std::string const& entry) const;
    bool value_exists(std::string const& section, std::string const& entry);
    bool section_exists(std::string const& section);
    Chameleon value_or_zero(std::string const& section, std::string const& entry); //if no value in config is specified it returns zero

    Chameleon const& value(std::string const& section, std::string const& entry, double value);
    Chameleon const& value(std::string const& section, std::string const& entry, std::string const& value);
};

extern ConfigFile* gConfig_file;
