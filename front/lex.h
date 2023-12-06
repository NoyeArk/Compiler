#ifndef LEX_H
#define LEX_H

#include "../define.h"
#include "../utils/utils.h"

inline int maxNumIdx(const std::string& str);
std::vector<std::string> stepSlice(const std::vector<std::string>& str);
bool splitQuotedText(const std::string& lineInput, std::string& preFormatLineInput, std::string& FormatLineInput, std::string& afterFormatLineInput);
std::vector<std::string> splitEveryToken(const std::string& str);

#endif // !LEX_H