//
// Created by n on 17/07/25.
//

#ifndef UTILS_STRINGS_H
#define UTILS_STRINGS_H

# include <string>
# define LEN_MAX_NAME 16

std::string trim(const std::string& str);
std::string trim(const std::string& str, const std::string& chars);
std::string getHead(const std::string& str);
std::string getNextWds(const std::string& str);
std::string lowerCase(const std::string& str);
std::string upperCase(const std::string& str);
bool isValidName(const std::string& str);

#endif //UTILS_STRINGS_H