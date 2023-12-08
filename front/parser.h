#ifndef PARSER_H
#define PARSER_H

#include "../define.h"
#include "../utils/utils.h"
#include "../opt/table.h"
#include "error_handle.h"
#include "../mid/ast.h"
#include "code_gen.h"
#include "match_token.h"

#include <set>

struct {
	bool MainFlag;
}flags;

static EmFlagIdent currentEmFlag = emNull;

static std::string lookHead;
static std::string lookHeadName;
static int readIndex;

static std::vector<EmFlagIdent> flagStack;

static std::vector<int> _lineNumberVector;		   // 源文件中每一个token对应的行号
static std::vector<std::string> _tokenVector;      // 存储源文件中每一个token
static std::vector<std::string> _tokenNameVector;  // 存储每一个token对应的种别码

inline std::string GetNextSecondToken();
inline std::string GetNextNumToken(const int& num);
bool isHaveWantInStmt(const std::string& wantStr);
void init(std::vector<std::string>&, std::vector<int>&);
void end();
void parsering(std::vector<std::string>&, std::vector<int>&);

// 输出语法分析结果
void outFileParserResult(const std::string& str);
// 输出解释执行结果
void outFileSemanticResult(std::string& str, const std::vector<int>& val = {});

void GetNextToken();
void PrintLookHead();
bool isInfunction();


// 当前是处于函数、还是循环中...
inline void insertFlag(const EmFlagIdent& flag);
inline void eraseFlag();

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
std::shared_ptr<FormatCharNode> Formatchar();
std::shared_ptr<NormalCharNode> NormalChar();
std::shared_ptr<PrimaryExpNode> PrimaryExp();
std::shared_ptr<FuncRParamsNode> FuncRParams(int&, const std::vector<std::pair<std::string, std::string>>&);
std::shared_ptr<NumberNode> Number();
std::shared_ptr<IntConstNode> IntConst();
std::shared_ptr<DecimalConstNode> decimal_const();
std::shared_ptr<NonzeroDigitNode> nonzero_digit();
std::shared_ptr<UnaryOpNode> UnaryOp();
std::shared_ptr<ConstInitValNode> ConstInitVal();
std::shared_ptr<ConstDefNode> ConstDef();
std::shared_ptr<MNode> M_();



EmType MatchTokenInt();
void MatchTokenMain();
void MatchTokenBreak();
void MatchTokenContinue();
void MatchTokenIf();
void MatchTokenElse();
void MatchTokenWhile();
void MatchTokenIdent(const bool& isDecl = false);
void MatchTokenGetint();
void MatchTokenPrintf();
void MatchTokenReturn();
void MatchTokenVoid();
void MatchTokenFormat();
void MatchTokenConst();
void MatchTokenComma();
void MatchTokenSemicn(const emErrorType& errorType = Null);
void MatchTokenAssign();
void MatchTokenLss();
void MatchTokenLeq();
void MatchTokenGre();
void MatchTokenGeq();
void MatchTokenEql();
void MatchTokenNeq();
void MatchTokenNot();
void MatchTokenAnd();
void MatchTokenOr();
void MatchTokenPlus();
void MatchTokenMinu();
void MatchTokenMult();
void MatchTokenDiv();
void MatchTokenMod();
void MatchTokenLparent();
void MatchTokenRparent(const emErrorType& errorType = Null);
void MatchTokenLbrace();
void MatchTokenRbrace();
void MatchTokenLbrack();
void MatchTokenRbrack(const emErrorType& errorType = Null);
void MatchTokenZero();
void MatchTokenNonZeroDigit();
void MatchTokenDigit();

// ---------------------以下为函数语义分析使用-------------------------------
std::shared_ptr<BlockNode> FuncBlock(std::vector<std::pair<std::string, std::string>> typeNameVector,
	std::vector<std::shared_ptr<ExpNode>> expVector);

#endif // !PARSER_H
