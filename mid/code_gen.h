#ifndef CODE_GEN_H
#define CODE_GEN_H

#include <iostream>
#include <vector>
#include <string>

// ´úÂë»ØÌî
void BackPatch(std::vector<int>& condlist, const int mquad);
std::vector<int> Merge(const std::vector<int>& condlist1, const std::vector<int>& condlist2);
std::vector<int> MakeList(const int& nextquad);

void gen(const std::string& code);

#endif // !CODE_GEN_H