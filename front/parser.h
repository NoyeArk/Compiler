#ifndef PARSER_H
#define PARSER_H

#include "../define.h"
#include "../utils/utils.h"
#include "../opt/table.h"
#include "error_handle.h"
#include "../mid/ast.h"
#include "match_token.h"

typedef enum {
	emNull = 10,
	emInLoop,   // 循环体
	emInFunc,   // 函数定义
	emFuncUse,  // 函数调用
	emMain      // main函数
}EmFlagIdent;

struct {
	bool MainFlag;
}flags;

static bool haveReturn = false;

static std::vector<EmFlagIdent> flagStack;
static EmFlagIdent currentEmFlag = emNull;

static size_t currentUnaryExpCount = 0;
static size_t preUnaryExpCount = 0;
static std::stack<std::string> bracketNameStack;

static std::queue<int> InputData;
static std::vector<int> _lineNumberVector;  // 源文件中每一个token对应的行号
static std::vector<std::string> _tokenVector;  // 存储源文件中每一个token
static std::vector<std::string> _tokenNameVector;  // 存储每一个token对应的种别码

static size_t readIndex;
static std::string lookHead;
static std::string lookHeadName;

inline void printLookHead();
inline std::string getNextSecondToken();
inline std::string getNextNumToken(const size_t& num);
void getNextToken();
bool isHaveWantInStmt(const std::string& wantStr);
inline void outFileParserResult(const std::string& str);
inline void outFileSemanticResult(std::string& str, const std::vector<int>& val={});
void init(std::vector<std::string>&, std::vector<int>&);
void end();
void parsering(std::vector<std::string>&, std::vector<int>&);

inline void insertFlag(const EmFlagIdent& flag) {
	if (currentEmFlag != emNull)
		flagStack.push_back(currentEmFlag);
	currentEmFlag = flag;
}

inline void eraseFlag() {
	if (flagStack.empty()) 
		currentEmFlag = emNull;
	else {
		currentEmFlag = flagStack.back();
		flagStack.pop_back();
	}
}

inline bool isInfunction() {
	for (const EmFlagIdent flag : flagStack) {
		if (flag == emInFunc)
			return true;
	}
	return false;
}

std::shared_ptr<CompUnitNode> CompUnit();
std::shared_ptr<DeclNode> Decl();
std::shared_ptr<FuncDefNode> FuncDef();
std::shared_ptr<MainFuncDefNode> MainFuncDef();
std::shared_ptr<ConstDeclNode> ConstDecl();
std::shared_ptr<VarDeclNode> VarDecl(const bool& val=false);
std::shared_ptr<FuncTypeNode> FuncType();
std::shared_ptr<IdentNode> Ident(const bool& isDecl=false);
std::shared_ptr<FuncFParamsNode> FuncFParams(std::vector<std::pair<std::string, std::string>>&);
std::shared_ptr<BlockNode> Block();
std::shared_ptr<BTypeNode> BType();
std::shared_ptr<VarDefNode> VarDef(const bool& val = false);
std::shared_ptr<FuncFParamNode> FuncFParam(std::pair<std::string, std::string>&);
std::shared_ptr<BlockItemNode> BlockItem();
std::shared_ptr<ArrayNode> Array();
std::shared_ptr<ConstExpNode> ConstExp();
std::shared_ptr<InitValNode> InitVal(const bool& val = false);
std::shared_ptr<StmtNode> Stmt();
std::shared_ptr<AddExpNode> AddExp();
std::shared_ptr<AddExp_Node> AddExp_(const bool& val = false);
std::shared_ptr<ExpNode> Exp(const bool& val = false);
std::shared_ptr<LValNode> LVal();
std::shared_ptr<CondNode> Cond();
std::shared_ptr<FormatCharNode> FormatString();
std::shared_ptr<MulExpNode> MulExp();
std::shared_ptr<MulExp_Node> MulExp_();
std::shared_ptr<LOrExpNode> LOrExp();
std::shared_ptr<LOrExp_Node> LOrExp_();
std::shared_ptr<LAndExpNode> LAndExp();
std::shared_ptr<LAndExp_Node> LAndExp_();
std::shared_ptr<EqExpNode> EqExp();
std::shared_ptr<EqExp_Node> EqExp_();
std::shared_ptr<UnaryExpNode> UnaryExp();
std::shared_ptr<RelExpNode> RelExp();
std::shared_ptr<RelExp_Node> RelExp_();
std::shared_ptr<CharNode> Char();
std::shared_ptr<FormatCharNode> FormatChar();
std::shared_ptr<NormalCharNode> NormalChar();
std::shared_ptr<PrimaryExpNode> PrimaryExp();
std::shared_ptr<FuncRParamsNode> FuncRParams(size_t&, const std::vector<std::pair<std::string, std::string>>&);
std::shared_ptr<NumberNode> Number();
std::shared_ptr<IntConstNode> IntConst();
std::shared_ptr<DecimalConstNode> decimal_const();
std::shared_ptr<NonzeroDigitNode> nonzero_digit();
std::shared_ptr<UnaryOpNode> UnaryOp();
std::shared_ptr<ConstInitValNode> ConstInitVal();
std::shared_ptr<ConstDefNode> ConstDef();


// ---------------------以下为函数语义分析使用-------------------------------
std::shared_ptr<BlockNode> FuncBlock(std::vector<std::pair<std::string, std::string>> typeNameVector,
	std::vector<std::shared_ptr<ExpNode>> expVector);

void contextSwitch(const std::vector<std::string>& token, 
				   const std::vector<std::string>& tokenName);  // 用于保存现场
void contextReload();  // 恢复现场

#endif // !PARSER_H
