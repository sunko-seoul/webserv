#pragma once

#ifndef PARSE_UTILS_HPP
# define PARSE_UTILS_HPP

# include <iostream>

void exitWithError(const std::string& msg);
std::string seperateMetaChar(std::string& line, std::string set);

#endif