#include "code_gen.h"

// 保存当前程序中所有的tac，最后进行输出
std::vector <std::string> tac;

// 给condlist中所在行填上mquad标号
void BackPatch(std::vector<int>& condlist, const int mquad) {
	for (auto targetLine : condlist) {
		tac[targetLine - 1] += std::to_string(mquad);
	}
}

std::vector<int> Merge(const std::vector<int>& condlist1, const std::vector<int>& condlist2) {
	std::vector<int> concatenatedVector;
	concatenatedVector.reserve(condlist1.size() + condlist2.size()); // 预分配空间以提高性能
	for (int value : condlist1) 
		concatenatedVector.push_back(value);
	for (int value : condlist2)
		concatenatedVector.push_back(value);
	return concatenatedVector;
}


std::vector<int> MakeList(const int& nextquad) {
	std::vector<int> list = { nextquad };
	return list;
}


void gen(const std::string& code) {
	tac.push_back(code);
}