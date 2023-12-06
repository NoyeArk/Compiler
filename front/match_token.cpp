#include "Match_token.h"
#include "parser.h"

// 匹配终结符int
inline EmType MatchTokenInt() {
	if (lookHeadName == "INTTK") {
		getNextToken();
		return Int;
	}
	else { // 错误处理
		return emTypeNull;
	}
}
// 匹配终结符main
inline void MatchTokenMain() {
	if (lookHeadName == "MAINTK") {
		getNextToken();
	}
	else { // 错误处理

	}
}
// 匹配终结符break
inline void MatchTokenBreak() {
	if (lookHeadName == "BREAKTK" && currentEmFlag == emInLoop) {
		getNextToken();
	}
	else { // 在非循环中使用break
		errorHandle(M, _lineNumberVector[readIndex]);
		getNextToken();
	}
}
// 匹配终结符continue
inline void MatchTokenContinue() {
	if (lookHeadName == "CONTINUETK" && currentEmFlag == emInLoop) {
		getNextToken();
	}
	else { // 在非循环中使用continue
		errorHandle(M, _lineNumberVector[readIndex]);
		getNextToken();
	}
}
// 匹配终结符if
inline void MatchTokenIf() {
	if (lookHeadName == "IFTK") {
		getNextToken();
	}
	else { // 错误处理

	}
}
// 匹配终结符else
inline void MatchTokenElse() {
	if (lookHeadName == "ELSETK") {
		getNextToken();
	}
	else { // 错误处理

	}
}
// 匹配终结符while
inline void MatchTokenWhile() {
	if (lookHeadName == "WHILETK") {
		getNextToken();
	}
	else { // 错误处理

	}
}
// 匹配终结符Ident变量名
inline void MatchTokenIdent(const bool& isDecl = false) {

	if (lookHead == "abcde")
		printf("");

	if (!isDecl) { // 不是声明，是引用
		bool isFuncParam = false;

		// 如果在函数声明中，先判断函数的实参
		if (currentEmFlag == emInFunc || isInfunction()) {
			if (TableSpace::isfuncParamExist(lookHead)) { // 可以匹配
				isFuncParam = true; // 是函数形参
			}
		}

		if (!isFuncParam) { // 如果不是函数形参
			TableSpace::TableItem outcome = TableSpace::query(lookHead);
			if (outcome.kind == TableSpace::null) {
				errorHandle(C, _lineNumberVector[readIndex]);
				getNextToken();
				return;
			}
		}
	}

	if ((lookHead[0] >= 'a' && lookHead[0] <= 'z') // 判断合法变量名
		|| (lookHead[0] >= 'A' && lookHead[0] <= 'Z')) {
		if (lookHead == "change1" && readIndex == 28)
			printf("");
		getNextToken();
	}
}
// 匹配终结符getint
inline void MatchTokenGetint() {
	if (lookHeadName == "GETINTTK") {
		getNextToken();
	}
	else { // 错误处理
		std::cout << "error: getint" << std::endl;
	}
}
// 匹配终结符printf
inline void MatchTokenPrintf() {
	if (lookHeadName == "PRINTFTK") {
		getNextToken();
	}
	else { // 错误处理
		std::cout << "error: printf" << std::endl;
	}
}
// 匹配终结符return
inline void MatchTokenReturn() {
	if (lookHeadName == "RETURNTK") {
		getNextToken();
	}
	else { // 错误处理
		std::cout << "error: return" << std::endl;
	}
}
// 匹配终结符void
inline void MatchTokenVoid() {
	if (lookHeadName == "VOIDTK") {
		getNextToken();
	}
	else { // 错误处理
		std::cout << "error: void" << std::endl;
	}
}
// 匹配终结符format
inline void MatchTokenFormat() {
	size_t found1 = lookHead.find("$");
	size_t found2 = lookHead.find("&");
	if (found1 == std::string::npos && found2 == std::string::npos) {
		getNextToken();
	}
	else { // 含有非法字符
		errorHandle(A, _lineNumberVector[readIndex]);
		getNextToken();
	}
}
// const
inline void MatchTokenConst() {
	if (lookHeadName == "CONSTTK") {
		getNextToken();
	}
	else { // 错误处理
		std::cout << "error: const" << std::endl;
	}
}


// 匹配终结符,
inline void MatchTokenComma() {
	if (lookHeadName == "COMMA") {
		getNextToken();
	}
	else { // 错误处理
		printLookHead();
		std::cout << "error: ," << std::endl;
	}
}
// 匹配终结符;
inline void MatchTokenSemicn(const emErrorType& errorType = Null) {
	if (lookHeadName == "SEMICN") {
		getNextToken();
	}
	else { // 缺少分号
		errorHandle(errorType, _lineNumberVector[readIndex - 1]); // 报错行号为分号前一个非终结符所在行号。
	}
}


// 匹配终结符=
inline void MatchTokenAssign() {
	if (lookHeadName == "ASSIGN") {
		getNextToken();
	}
	else { // 错误处理
		std::cout << "readIndex:" << readIndex << std::endl;
		std::cout << "error: =" << std::endl;
	}
}
// 匹配终结符<
inline void MatchTokenLss() {
	if (lookHeadName == "LSS") {
		getNextToken();
	}
	else { // 错误处理
		std::cout << "error: <" << std::endl;
		printLookHead();
	}
}
// 匹配终结符<=
inline void MatchTokenLeq() {
	if (lookHeadName == "LEQ") {
		getNextToken();
	}
	else { // 错误处理
		std::cout << "error: <=" << std::endl;
	}
}
// 匹配终结符>
inline void MatchTokenGre() {
	if (lookHeadName == "GRE") {
		getNextToken();
	}
	else { // 错误处理
		std::cout << "error: >" << std::endl;
	}
}
// 匹配终结符>=
inline void MatchTokenGeq() {
	if (lookHeadName == "GEQ") {
		getNextToken();
	}
	else { // 错误处理
		std::cout << "error: >=" << std::endl;
	}
}
// 匹配终结符==
inline void MatchTokenEql() {
	if (lookHeadName == "EQL") {
		getNextToken();
	}
	else { // 错误处理
		std::cout << "error: ==" << std::endl;
	}
}
// 匹配终结符!=
inline void MatchTokenNeq() {
	if (lookHeadName == "NEQ") {
		getNextToken();
	}
	else { // 错误处理
		std::cout << "error: !=" << std::endl;
	}
}
// 匹配终结符!
inline void MatchTokenNot() {
	if (lookHeadName == "NOT") {
		getNextToken();
	}
	else { // 错误处理
		std::cout << "error: !" << std::endl;
	}
}
// 匹配终结符&&
inline void MatchTokenAnd() {
	if (lookHeadName == "AND") {
		getNextToken();
	}
	else { // 错误处理
		std::cout << "error: &&" << std::endl;
	}
}
// 匹配终结符||
inline void MatchTokenOr() {
	if (lookHeadName == "OR") {
		getNextToken();
	}
	else { // 错误处理
		std::cout << "error: ||" << std::endl;
	}
}


// 匹配终结符+
inline void MatchTokenPlus() {
	if (lookHeadName == "PLUS") {
		getNextToken();
	}
	else { // 错误处理
		std::cout << "error: +" << std::endl;
	}
}
// 匹配终结符-
inline void MatchTokenMinu() {
	if (lookHeadName == "MINU") {
		getNextToken();
	}
	else { // 错误处理
		std::cout << "error: -" << std::endl;
	}
}
// 匹配终结符*
inline void MatchTokenMult() {
	if (lookHeadName == "MULT") {
		getNextToken();
	}
	else { // 错误处理
		std::cout << "error: -" << std::endl;
	}
}
// 匹配终结符/
inline void MatchTokenDiv() {
	if (lookHeadName == "DIV") {
		getNextToken();
	}
	else { // 错误处理
		std::cout << "error: /" << std::endl;
	}
}
// 匹配终结符%
inline void MatchTokenMod() {
	if (lookHeadName == "MOD") {
		getNextToken();
	}
	else { // 错误处理
		std::cout << "error: %" << std::endl;
	}
}


// 匹配终结符(
inline void MatchTokenLparent() {
	if (lookHeadName == "LPARENT") {
		getNextToken();
	}
	else { // 错误处理
		std::cout << "error: (" << std::endl;
	}
}
// 匹配终结符)
inline void MatchTokenRparent(const emErrorType& errorType = Null) {
	if (lookHeadName == "RPARENT") {
		getNextToken();
	}
	else { // 缺少 )
		errorHandle(errorType, _lineNumberVector[readIndex - 1]); // 报错行数为前一个非终结符所在位置
	}
}
// 匹配终结符{
inline void MatchTokenLbrace() {
	if (lookHeadName == "LBRACE") {
		getNextToken();
	}
	else { // 错误处理
		std::cout << "error: {" << std::endl;
	}
}
// 匹配终结符}
inline void MatchTokenRbrace() {
	if (lookHeadName == "RBRACE") {
		getNextToken();
	}
	else { // 错误处理
		std::cout << "error: }" << std::endl;
	}
}
// 匹配终结符[
inline void MatchTokenLbrack() {
	if (lookHeadName == "LBRACK") {
		getNextToken();
	}
	else { // 错误处理

	}
}
// 匹配终结符]
inline void MatchTokenRbrack(const emErrorType& errorType = Null) {
	if (lookHeadName == "RBRACK") {
		getNextToken();
	}
	else { // 缺少右中括号
		errorHandle(errorType, _lineNumberVector[readIndex]);
	}
}


// 非零数字0
inline void MatchTokenZero() {
	if (lookHeadName == "INTCON" && lookHead >= "0") {
		getNextToken();
	}
	else { // 错误处理

	}
}
// 非零数字1,2,...,9
inline void MatchTokenNonZeroDigit() {
	if (lookHeadName == "INTCON" && lookHead >= "1" && lookHead <= "9") {
		getNextToken();
	}
	else { // 错误处理

	}
}
// 非零数字0,1,2,...,9
inline void MatchTokenDigit() {
	if (lookHeadName == "INTCON" && lookHead >= "0") {
		getNextToken();
	}
	else { // 错误处理

	}
}