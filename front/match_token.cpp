#include "Match_token.h"
#include "parser.h"

// ƥ���ս��int
inline EmType MatchTokenInt() {
	if (lookHeadName == "INTTK") {
		getNextToken();
		return Int;
	}
	else { // ������
		return emTypeNull;
	}
}
// ƥ���ս��main
inline void MatchTokenMain() {
	if (lookHeadName == "MAINTK") {
		getNextToken();
	}
	else { // ������

	}
}
// ƥ���ս��break
inline void MatchTokenBreak() {
	if (lookHeadName == "BREAKTK" && currentEmFlag == emInLoop) {
		getNextToken();
	}
	else { // �ڷ�ѭ����ʹ��break
		errorHandle(M, _lineNumberVector[readIndex]);
		getNextToken();
	}
}
// ƥ���ս��continue
inline void MatchTokenContinue() {
	if (lookHeadName == "CONTINUETK" && currentEmFlag == emInLoop) {
		getNextToken();
	}
	else { // �ڷ�ѭ����ʹ��continue
		errorHandle(M, _lineNumberVector[readIndex]);
		getNextToken();
	}
}
// ƥ���ս��if
inline void MatchTokenIf() {
	if (lookHeadName == "IFTK") {
		getNextToken();
	}
	else { // ������

	}
}
// ƥ���ս��else
inline void MatchTokenElse() {
	if (lookHeadName == "ELSETK") {
		getNextToken();
	}
	else { // ������

	}
}
// ƥ���ս��while
inline void MatchTokenWhile() {
	if (lookHeadName == "WHILETK") {
		getNextToken();
	}
	else { // ������

	}
}
// ƥ���ս��Ident������
inline void MatchTokenIdent(const bool& isDecl = false) {

	if (lookHead == "abcde")
		printf("");

	if (!isDecl) { // ����������������
		bool isFuncParam = false;

		// ����ں��������У����жϺ�����ʵ��
		if (currentEmFlag == emInFunc || isInfunction()) {
			if (TableSpace::isfuncParamExist(lookHead)) { // ����ƥ��
				isFuncParam = true; // �Ǻ����β�
			}
		}

		if (!isFuncParam) { // ������Ǻ����β�
			TableSpace::TableItem outcome = TableSpace::query(lookHead);
			if (outcome.kind == TableSpace::null) {
				errorHandle(C, _lineNumberVector[readIndex]);
				getNextToken();
				return;
			}
		}
	}

	if ((lookHead[0] >= 'a' && lookHead[0] <= 'z') // �жϺϷ�������
		|| (lookHead[0] >= 'A' && lookHead[0] <= 'Z')) {
		if (lookHead == "change1" && readIndex == 28)
			printf("");
		getNextToken();
	}
}
// ƥ���ս��getint
inline void MatchTokenGetint() {
	if (lookHeadName == "GETINTTK") {
		getNextToken();
	}
	else { // ������
		std::cout << "error: getint" << std::endl;
	}
}
// ƥ���ս��printf
inline void MatchTokenPrintf() {
	if (lookHeadName == "PRINTFTK") {
		getNextToken();
	}
	else { // ������
		std::cout << "error: printf" << std::endl;
	}
}
// ƥ���ս��return
inline void MatchTokenReturn() {
	if (lookHeadName == "RETURNTK") {
		getNextToken();
	}
	else { // ������
		std::cout << "error: return" << std::endl;
	}
}
// ƥ���ս��void
inline void MatchTokenVoid() {
	if (lookHeadName == "VOIDTK") {
		getNextToken();
	}
	else { // ������
		std::cout << "error: void" << std::endl;
	}
}
// ƥ���ս��format
inline void MatchTokenFormat() {
	size_t found1 = lookHead.find("$");
	size_t found2 = lookHead.find("&");
	if (found1 == std::string::npos && found2 == std::string::npos) {
		getNextToken();
	}
	else { // ���зǷ��ַ�
		errorHandle(A, _lineNumberVector[readIndex]);
		getNextToken();
	}
}
// const
inline void MatchTokenConst() {
	if (lookHeadName == "CONSTTK") {
		getNextToken();
	}
	else { // ������
		std::cout << "error: const" << std::endl;
	}
}


// ƥ���ս��,
inline void MatchTokenComma() {
	if (lookHeadName == "COMMA") {
		getNextToken();
	}
	else { // ������
		printLookHead();
		std::cout << "error: ," << std::endl;
	}
}
// ƥ���ս��;
inline void MatchTokenSemicn(const emErrorType& errorType = Null) {
	if (lookHeadName == "SEMICN") {
		getNextToken();
	}
	else { // ȱ�ٷֺ�
		errorHandle(errorType, _lineNumberVector[readIndex - 1]); // �����к�Ϊ�ֺ�ǰһ�����ս�������кš�
	}
}


// ƥ���ս��=
inline void MatchTokenAssign() {
	if (lookHeadName == "ASSIGN") {
		getNextToken();
	}
	else { // ������
		std::cout << "readIndex:" << readIndex << std::endl;
		std::cout << "error: =" << std::endl;
	}
}
// ƥ���ս��<
inline void MatchTokenLss() {
	if (lookHeadName == "LSS") {
		getNextToken();
	}
	else { // ������
		std::cout << "error: <" << std::endl;
		printLookHead();
	}
}
// ƥ���ս��<=
inline void MatchTokenLeq() {
	if (lookHeadName == "LEQ") {
		getNextToken();
	}
	else { // ������
		std::cout << "error: <=" << std::endl;
	}
}
// ƥ���ս��>
inline void MatchTokenGre() {
	if (lookHeadName == "GRE") {
		getNextToken();
	}
	else { // ������
		std::cout << "error: >" << std::endl;
	}
}
// ƥ���ս��>=
inline void MatchTokenGeq() {
	if (lookHeadName == "GEQ") {
		getNextToken();
	}
	else { // ������
		std::cout << "error: >=" << std::endl;
	}
}
// ƥ���ս��==
inline void MatchTokenEql() {
	if (lookHeadName == "EQL") {
		getNextToken();
	}
	else { // ������
		std::cout << "error: ==" << std::endl;
	}
}
// ƥ���ս��!=
inline void MatchTokenNeq() {
	if (lookHeadName == "NEQ") {
		getNextToken();
	}
	else { // ������
		std::cout << "error: !=" << std::endl;
	}
}
// ƥ���ս��!
inline void MatchTokenNot() {
	if (lookHeadName == "NOT") {
		getNextToken();
	}
	else { // ������
		std::cout << "error: !" << std::endl;
	}
}
// ƥ���ս��&&
inline void MatchTokenAnd() {
	if (lookHeadName == "AND") {
		getNextToken();
	}
	else { // ������
		std::cout << "error: &&" << std::endl;
	}
}
// ƥ���ս��||
inline void MatchTokenOr() {
	if (lookHeadName == "OR") {
		getNextToken();
	}
	else { // ������
		std::cout << "error: ||" << std::endl;
	}
}


// ƥ���ս��+
inline void MatchTokenPlus() {
	if (lookHeadName == "PLUS") {
		getNextToken();
	}
	else { // ������
		std::cout << "error: +" << std::endl;
	}
}
// ƥ���ս��-
inline void MatchTokenMinu() {
	if (lookHeadName == "MINU") {
		getNextToken();
	}
	else { // ������
		std::cout << "error: -" << std::endl;
	}
}
// ƥ���ս��*
inline void MatchTokenMult() {
	if (lookHeadName == "MULT") {
		getNextToken();
	}
	else { // ������
		std::cout << "error: -" << std::endl;
	}
}
// ƥ���ս��/
inline void MatchTokenDiv() {
	if (lookHeadName == "DIV") {
		getNextToken();
	}
	else { // ������
		std::cout << "error: /" << std::endl;
	}
}
// ƥ���ս��%
inline void MatchTokenMod() {
	if (lookHeadName == "MOD") {
		getNextToken();
	}
	else { // ������
		std::cout << "error: %" << std::endl;
	}
}


// ƥ���ս��(
inline void MatchTokenLparent() {
	if (lookHeadName == "LPARENT") {
		getNextToken();
	}
	else { // ������
		std::cout << "error: (" << std::endl;
	}
}
// ƥ���ս��)
inline void MatchTokenRparent(const emErrorType& errorType = Null) {
	if (lookHeadName == "RPARENT") {
		getNextToken();
	}
	else { // ȱ�� )
		errorHandle(errorType, _lineNumberVector[readIndex - 1]); // ��������Ϊǰһ�����ս������λ��
	}
}
// ƥ���ս��{
inline void MatchTokenLbrace() {
	if (lookHeadName == "LBRACE") {
		getNextToken();
	}
	else { // ������
		std::cout << "error: {" << std::endl;
	}
}
// ƥ���ս��}
inline void MatchTokenRbrace() {
	if (lookHeadName == "RBRACE") {
		getNextToken();
	}
	else { // ������
		std::cout << "error: }" << std::endl;
	}
}
// ƥ���ս��[
inline void MatchTokenLbrack() {
	if (lookHeadName == "LBRACK") {
		getNextToken();
	}
	else { // ������

	}
}
// ƥ���ս��]
inline void MatchTokenRbrack(const emErrorType& errorType = Null) {
	if (lookHeadName == "RBRACK") {
		getNextToken();
	}
	else { // ȱ����������
		errorHandle(errorType, _lineNumberVector[readIndex]);
	}
}


// ��������0
inline void MatchTokenZero() {
	if (lookHeadName == "INTCON" && lookHead >= "0") {
		getNextToken();
	}
	else { // ������

	}
}
// ��������1,2,...,9
inline void MatchTokenNonZeroDigit() {
	if (lookHeadName == "INTCON" && lookHead >= "1" && lookHead <= "9") {
		getNextToken();
	}
	else { // ������

	}
}
// ��������0,1,2,...,9
inline void MatchTokenDigit() {
	if (lookHeadName == "INTCON" && lookHead >= "0") {
		getNextToken();
	}
	else { // ������

	}
}