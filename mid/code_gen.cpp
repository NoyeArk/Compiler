#include "code_gen.h"

// ���浱ǰ���������е�tac�����������
std::vector <std::string> tac;

// ��condlist������������mquad���
void BackPatch(std::vector<int>& condlist, const int mquad) {
	for (auto targetLine : condlist) {
		tac[targetLine - 1] += std::to_string(mquad);
	}
}

std::vector<int> Merge(const std::vector<int>& condlist1, const std::vector<int>& condlist2) {
	std::vector<int> concatenatedVector;
	concatenatedVector.reserve(condlist1.size() + condlist2.size()); // Ԥ����ռ����������
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