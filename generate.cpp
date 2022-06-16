#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <stdio.h>
#include <algorithm>
#include <locale>
#include <codecvt>
#include <cassert>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

void replace_all(
    std::string& s,
    std::string const& toReplace,
    std::string const& replaceWith
) {
    std::ostringstream oss;
    std::size_t pos = 0;
    std::size_t prevPos = pos;

    while (true) {
        prevPos = pos;
        pos = s.find(toReplace, pos);
        if (pos == std::string::npos)
            break;
        oss << s.substr(prevPos, pos - prevPos);
        oss << replaceWith;
        pos += toReplace.size();
    }

    oss << s.substr(prevPos);
    s = oss.str();
}

int main(void) {
    json icons;

    std::ifstream file("icons.json");

    try {
        file >> icons;
        file.close();
    } catch (nlohmann::detail::exception &e) {
        printf("Error: %s\n", e.what());
        return 1;
    }

    std::string header = "#pragma once\n\n#define ICON_MIN_FA 0xE000\n#define ICON_MAX_FA 0xF8FF\n\n";
    std::string line_prefix = "#define ICON_FA_";
    std::string output = header;

    for (auto& [key, val] : icons.items()) {
        std::string name = key;
        std::string unicode = val["unicode"];
        std::transform(name.begin(), name.end(), name.begin(), ::toupper);
        replace_all(name, "-", "_");

        unsigned long unicode_value = strtoul(unicode.c_str(), nullptr, 16);

        // convert unsigned long to utf8
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
        std::string u8 = converter.to_bytes(unicode_value);
        const char* u8_ptr = u8.c_str();

        // convert char to byte array
        char buffer[32];
        int offset = 0;
        for (int i = 0; i < u8.size(); i++) {
            offset += snprintf(buffer + offset, 32 - offset, "\\x%hhx", (unsigned int)u8_ptr[i]);
        }

        // add to output
        output += line_prefix + name + " \"" + std::string(buffer) + "\"" + " //" + " U+" + unicode + "\n";
    }

    std::ofstream out("IconsFontAwesome6Pro.h");
    out << output;
    out.close();

    return 0;
}