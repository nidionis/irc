//
// Created by n on 17/07/25.
//

#include "main.hpp"
#include "utils_strings.hpp"

#include <algorithm>
#include <string>

std::string trim(const std::string& str)
{
    size_t first = str.find_first_not_of(" \t\n\r");
    size_t last = str.find_last_not_of(" \t\n\r");
    if (first == std::string::npos || last == std::string::npos)
    {
        return "";
    }
    return str.substr(first, last - first + 1);
}

std::string getHead(const std::string& str)
{
    size_t pos = str.find(' ');
    std::string head = (pos != std::string::npos) ? str.substr(0, pos) : str;
    return trim(head);
}

std::string getNextWds(const std::string& str)
{
    size_t pos = str.find(' ');
    if (pos == std::string::npos)
    {
        return "";
    }
    while (pos < str.length() && std::isspace(str[pos]))
    {
        pos++;
    }
    return str.substr(pos);
}

bool isValidName(const std::string& str)
{
    if (str.empty() || str.length() >= LEN_MAX_NAME) {
        return false;
    }
    if (std::isdigit(str[0])) {
        return false;
    }
    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
        if (!std::isalnum(*it)) {
            return false;
        }
    }
    return true;
}

std::string lowerCase(const std::string& str)
{
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::string upperCase(const std::string& str)
{
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}
