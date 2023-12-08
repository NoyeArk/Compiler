#include "parser.h"

static bool haveReturn = false;

static int currentUnaryExpCount = 0;
static int preUnaryExpCount = 0;
static std::stack<std::string> bracketNameStack;
static std::queue<int> InputData;

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

// �жϵ�ǰ����У��ֺ�֮ǰ���Ƿ�����Ҫ��wantStr
bool isHaveWantInStmt(const std::string& wantStr) {
	// �ӵ�ǰreadIndex��ʼ������ֱ�������ֺ�
	int _readIndex = readIndex + 1;
	while (_tokenVector[_readIndex] != ";") {
		if (_tokenVector[_readIndex] == wantStr) {
			// ����Ҫ���ַ�
			return true;
		}
		_readIndex++;
		assert(_readIndex < _tokenVector.size());
	}
	// ����ִ�е����˵���������û����Ҫ���ַ�
	return false;
}

// �ж���������һ��,��֮ǰ�м���[
int IdentType() {
	int count = 0;
	int idx = readIndex + 1;
	while (_tokenVector[idx] != "," && _tokenVector[idx] != ")"
		&& _tokenVector[idx] != "]" && _tokenVector[idx] != ";") {
		if (_tokenVector[idx] == "[")
			++count;
		++idx;
	}
	return count;
}

inline std::string GetNextSecondToken() {
	//assert(readIndex + 1 < _tokenNameVector.size());
	if (readIndex + 1 < _tokenNameVector.size()) {
		return _tokenNameVector[readIndex + 1];
	}
	else return "null";
}

inline std::string GetNextNumToken(const int& num) {
	assert(readIndex + num < _tokenNameVector.size());
	return _tokenNameVector[readIndex + num];
}

void init(std::vector<std::string>& tokenVector,
	std::vector<int>& lineNumberVector) {

	// ��input.txt�ļ��ж�ȡ����
	std::ifstream inFile("input.txt"); // ���ļ�

	if (inFile.is_open()) { // ����ļ��Ƿ�ɹ���
		std::string line;
		while (std::getline(inFile, line)) { // ���ж�ȡ�ļ�����
			std::istringstream iss(line); // ʹ���ַ�����������ÿ������
			int num;
			while (iss >> num) { // �����ȡ����
				InputData.push(num);
			}
		}
	}
	inFile.close(); // �ر�input.txt

	outputFile.open("output.txt");

	for (int ii = 0; ii < tokenVector.size(); ii++) {
		bool isFormat = false;
		int start = tokenVector[ii].find('"');
		int end = tokenVector[ii].find('"', start + 1);

		if (start != std::string::npos && end != std::string::npos) {
			isFormat = true;
		}
		if (checkSpecialChar(tokenVector[ii]) && !isFormat) { // ������������ַ�������
			continue;
		}
		std::string classification = transName(tokenVector[ii], isFormat); // �õ��ֱ���
		if (classification == "ERROR") {
			continue;
		}
		_tokenVector.push_back(tokenVector[ii]);
		_tokenNameVector.push_back(classification);
		_lineNumberVector.push_back(lineNumberVector[ii]);
	}
	//_lineNumberVector.insert(_lineNumberVector.end(), lineNumberVector.begin(), lineNumberVector.end());

	readIndex = 0;
	lookHeadName = _tokenNameVector[readIndex];
	lookHead = _tokenVector[readIndex];
}

void end() {
	outputFile.close();
}

void parsering(std::vector<std::string>& tokenVector, std::vector<int>& lineNumberVector) {
	init(tokenVector, lineNumberVector);
	CompUnit();
	end();
}
// 1-���뵥Ԫ
std::shared_ptr<CompUnitNode> CompUnit() {
	auto cur = std::make_shared<CompUnitNode>("CompUnitNode");

	while (GetNextNumToken(2) != "LPARENT" && GetNextSecondToken() != "MAINTK") {
		cur->decl.push_back(Decl());
	}

	while (GetNextSecondToken() != "MAINTK")
		cur->func_def.push_back(FuncDef());
	cur->main_func_def = MainFuncDef();

	outFileParserResult("CompUnit");
	return cur;
}
// 2-����
std::shared_ptr<DeclNode> Decl() {
	auto cur = std::make_shared<DeclNode>("Decl");

	if (lookHeadName == "CONSTTK") 
		cur->const_decl = ConstDecl();
	else if (lookHeadName == "INTTK" && GetNextSecondToken() != "MAINTK") 
		cur->var_decl = VarDecl();

	return cur;
}
// 3-��������
std::shared_ptr<FuncDefNode> FuncDef() {
	auto cur = std::make_shared<FuncDefNode>("FuncDef");

	insertFlag(emInFunc); // ��־��ǰΪ��������
	TableSpace::newScope();
	
	const std::string funcName = _tokenVector[readIndex+1]; // �õ�������
	TableSpace::ReturnType retType = (lookHead == "int") ? TableSpace::Int : TableSpace::Void; // �õ���������ֵ����
	std::vector<std::pair<std::string, std::string>> typeNameVector; // 1:���� 2:������

	cur->func_type = FuncType();
	cur->ident = Ident(true);
	MatchTokenLparent(); // (
	if (lookHeadName != "RPARENT") { // )
		cur->func_f_params = FuncFParams(typeNameVector);
	}
	MatchTokenRparent(J); // )
	TableSpace::funcInsert(funcName, retType, typeNameVector.size(), typeNameVector); // ������ű��¼

	int startIndex = readIndex;
	cur->block = Block();  // �����岿��
	int endIndex = readIndex;
	std::vector<std::string> funcBody(_tokenVector.begin() + startIndex, _tokenVector.begin() + endIndex);
	std::vector<std::string> funcBodyName(_tokenNameVector.begin() + startIndex, _tokenNameVector.begin() + endIndex);

	// �������������ű���
	TableSpace::insertFuncBody(funcBody, funcBodyName);

	// �з���ֵ�ĺ���û��return
	if (retType != TableSpace::Void) {
		if (haveReturn == false) {
			// �ӵ�ǰ�п�ʼ��������������}
			int idx = readIndex;
			while (_tokenVector[--idx] != "}") {
				continue;
			}
			errorHandle(G, _lineNumberVector[idx]);
		}
	}

	TableSpace::remove();
	haveReturn = false;
	eraseFlag();  // ȥ����ǰ��־
	outFileParserResult("FuncDef");

	return cur;
}
// 4-����������
std::shared_ptr<MainFuncDefNode> MainFuncDef() {
	flags.MainFlag = true;
	auto cur = std::make_shared<MainFuncDefNode>("MainFuncDef");

	MatchTokenInt(); // int
	MatchTokenMain(); // main
	MatchTokenLparent(); // (
	MatchTokenRparent(); // )
	cur->block = Block();
	if (haveReturn == false) {
		// �ӵ�ǰ�п�ʼ��������������}
		int idx = readIndex+1;
		while (_tokenVector[--idx] != "}") {
			continue;
		}
		errorHandle(G, _lineNumberVector[idx]);
	}
	outFileParserResult("MainFuncDef");

	flags.MainFlag = false;
	return cur;
}
// 5-��������
std::shared_ptr<ConstDeclNode> ConstDecl() {
	auto cur = std::make_shared<ConstDeclNode>("ConstDecl");
	MatchTokenConst(); // const
	cur->var_decl = VarDecl(true);
	return cur;
}
// 6-��������
std::shared_ptr<VarDeclNode> VarDecl(const bool& isConstDecl) {
	auto cur = std::make_shared<VarDeclNode>("VarDecl");

	cur->btype = BType();
	t = cur->btype->emType;
	w = cur->btype->width;
	cur->vardef.push_back(VarDef(isConstDecl));
	// ��������ݿ����ظ�0�λ���
	while (lookHeadName == "COMMA") { // ,
		MatchTokenComma();
		cur->vardef.push_back(VarDef(isConstDecl));
	}
	MatchTokenSemicn(I); // ;
	if (isConstDecl) outFileParserResult("ConstDecl");
	else outFileParserResult("VarDecl");

	return cur;
}
// 7-��������
std::shared_ptr<FuncTypeNode> FuncType() {
	auto cur = std::make_shared<FuncTypeNode>("FuncType");

	if (lookHeadName == "VOIDTK") { // void
		MatchTokenVoid(); // void
		cur->funcType = "void";
	}
	else {
		MatchTokenInt(); // int
		cur->funcType = "int";
	}
	outFileParserResult("FuncType");
	return cur;
}
// 8-��ʶ��
std::shared_ptr<IdentNode> Ident(const bool& isDecl) {
	auto cur = std::make_shared<IdentNode>("Ident");

	cur->lexeme = lookHead;
	MatchTokenIdent(isDecl);

	return cur;
}
// 9-�����βα�
std::shared_ptr<FuncFParamsNode> FuncFParams(std::vector<std::pair<std::string, std::string>>& typeNameVector) {
	auto cur = std::make_shared<FuncFParamsNode>("FuncFParams");

	std::pair<std::string, std::string> currentTypeNamePair;

	cur->func_f_param.push_back(FuncFParam(currentTypeNamePair));
	typeNameVector.push_back(currentTypeNamePair);

	while (lookHeadName == "COMMA") { // ,
		currentTypeNamePair.first = "";
		currentTypeNamePair.second = "";

		MatchTokenComma(); // ,
		cur->func_f_param.push_back(FuncFParam(currentTypeNamePair));

		typeNameVector.push_back(currentTypeNamePair);
	}

	// ����������� void testFunc(int a, int a)
	std::set<std::string> secondElements;
	for (const auto& pair : typeNameVector) {
		if (!secondElements.insert(pair.second).second) {
			errorHandle(B, _lineNumberVector[readIndex]);
		}
	}

	outFileParserResult("FuncFParams");
	return cur;
}
// 10-����
std::shared_ptr<BlockNode> Block() {
	TableSpace::newScope();
	auto cur = std::make_shared<BlockNode>("Block");

	MatchTokenLbrace(); // {
	while (lookHeadName != "RBRACE") {
		cur->block_item.push_back(BlockItem());
	}
	MatchTokenRbrace(); // }
	outFileParserResult("Block");

	TableSpace::remove();
	return cur;
}
// �����������
std::shared_ptr<BlockNode> FuncBlock(std::vector<std::pair<std::string, std::string>> typeNameVector,
	std::vector<std::shared_ptr<ExpNode>> expVector) {
	TableSpace::newScope();
	// ���뺯���βα���
	for (int ii = 0; ii < expVector.size(); ii++) {
		TableSpace::insert(typeNameVector[ii].second, TableSpace::variable, std::to_string(expVector[ii]->val), ""); // ������ű��¼
	}

	auto cur = std::make_shared<BlockNode>("Block");

	MatchTokenLbrace(); // {
	while (lookHeadName != "RBRACE") {
		cur->block_item.push_back(BlockItem());
	}
	//cur->retVal = cur->block_item.back()->retVal;

	//MatchTokenRbrace(); // }

	TableSpace::remove();
	return cur;
}
// 11-��������
std::shared_ptr<BTypeNode> BType() {
	auto cur = std::make_shared<BTypeNode>("BType");

	EmType type = MatchTokenInt();
	cur->emType = type;
	cur->width = 4;

	return cur;
}
// 12-��������
std::shared_ptr<VarDefNode> VarDef(const bool& isConstDef) {
	std::shared_ptr<VarDefNode> cur = std::make_shared<VarDefNode>("VarDef");
 	std::string identType;

	// ------------------------�ų��ض���------------------------
	TableSpace::TableItem outcome = TableSpace::query(lookHead, true);
	if (outcome.kind != TableSpace::null)
		errorHandle(B, _lineNumberVector[readIndex]);
	// ---------------------------------------------------------

	// ------------------------�õ���ǰ������������------------------------
	const std::string identName = lookHead;
	std::string val = "null";
	TableSpace::Kind kind = (isConstDef == true) ? TableSpace::constant : TableSpace::variable;
	// ---------------------------------------------------------

	if (identName == "k") {
		printf("");
	}

	cur->ident = Ident(true);
	cur->array = Array();

	if (lookHeadName == "ASSIGN") { // ƥ���ս��=
		MatchTokenAssign();
		cur->initval = InitVal(isConstDef);
		val = std::to_string(cur->initval->val);
	}
	TableSpace::insert(identName, kind, val, identType); // ������ű��¼

	if (isConstDef) outFileParserResult("ConstDef");
	else outFileParserResult("VarDef");
	return cur;
}
// 12.5-��ά��������
std::shared_ptr<ArrayNode> Array() {
	auto cur = std::make_shared<ArrayNode>("Array");

	if (lookHeadName == "LBRACK") {  // C -> [num]C1
		MatchTokenLbrack();  // [
		cur->const_exp = ConstExp();  // ��ʱConstExp��ֵ����num��ֵ
		MatchTokenRbrack(K);  // ]
		cur->array = Array();
		cur->dimension.push_back(cur->const_exp->val);  // ��Ӵ�ά�ȴ�С
		cur->width = cur->width * cur->const_exp->val;
	}
	else {  // C -> ��
		cur->dimension.push_back(0);  //  ������������
		cur->width = w;  // ���Ϊȫ�ֱ���
	}
	return cur;
}
// 13-�����β�
std::shared_ptr<FuncFParamNode> FuncFParam(std::pair<std::string, std::string>& currentTypeNamePair) {
	auto cur = std::make_shared<FuncFParamNode>("FuncFParam");
	currentTypeNamePair.second = _tokenVector[readIndex + 1]; // �ڶ����ǲ�����

	cur->btype = BType();
	cur->ident = Ident(true);
	if (lookHeadName == "LBRACK") { // [
		MatchTokenLbrack(); // [
		currentTypeNamePair.first += "[";
		MatchTokenRbrack(K); // ]
		while (lookHeadName == "LBRACK") { // [
			MatchTokenLbrack(); // [
			currentTypeNamePair.first += "[";
			cur->const_exp.push_back(ConstExp());
			MatchTokenRbrack(K); // ]
		}
	}
	outFileParserResult("FuncFParam");
	return cur;
}
// 14-������
std::shared_ptr<BlockItemNode> BlockItem() {
	auto cur = std::make_shared<BlockItemNode>("BlockItem");

	if (lookHeadName == "CONSTTK" || lookHeadName == "INTTK")  // ѡ�����ʽDecl
		cur->decl = Decl();
	else {
		cur->stmt = Stmt();
		cur->retVal = cur->stmt->retVal;
	}
	return cur;
}
// 15-�������ʽ
std::shared_ptr<ConstExpNode> ConstExp() {
	auto cur = std::make_shared<ConstExpNode>("ConstExp");

	cur->add_exp = AddExp();
	outFileParserResult("ConstExp");
	return cur;
}
// 16=������ֵ
std::shared_ptr<InitValNode> InitVal(const bool& isConstInitVal) {
	auto cur = std::make_shared<InitValNode>("InitVal");

	if (lookHeadName == "LBRACE") { // {
		MatchTokenLbrace(); // {
		if (lookHeadName == "RBRACE") { // �ж��Ƿ��� }
			MatchTokenRbrace(); // }
		}
		else {
			cur->init_val.push_back(InitVal(isConstInitVal));
			while (lookHeadName == "COMMA") { // ,
				MatchTokenComma();
				cur->init_val.push_back(InitVal(isConstInitVal));
			}
			MatchTokenRbrace(); // }
		}
	}
	else {
		cur->exp = Exp(isConstInitVal);
		cur->val = cur->exp->val;
	}
	if (isConstInitVal) outFileParserResult("ConstInitVal");
	else outFileParserResult("InitVal");
	return cur;
}
// 17-���
std::shared_ptr<StmtNode> Stmt() {
	auto cur = std::make_shared<StmtNode>("Stmt");
	if (lookHead == "result")
		printf("");

	if (lookHeadName == "PRINTFTK") { // printf
		MatchTokenPrintf();
		MatchTokenLparent(); // (

		cur->format_char = FormatString();  // Ҫ���������
		while (lookHeadName == "COMMA") { // ,
			MatchTokenComma(); // ,
			cur->exp = Exp();  // ��Ҫƥ��Ĳ���
			cur->format_char->print_exp.push_back(cur->exp->val);
			--cur->format_char->d_num;
		}

		if (cur->format_char->d_num) errorHandle(L, _lineNumberVector[readIndex]);
		MatchTokenRparent(); // )
		MatchTokenSemicn(I); // ;
	}
	else if (lookHeadName == "IFTK") { // if
		++IfNum;
		// newlabel()
		bool isFirst;

		for (int ii = 0; ii < 1; ii++) {
			isFirst = ii == 0 ? true : false;

			if (isFirst) {
				MatchTokenIf(); // if
				MatchTokenLparent(); // ( 
				cur->cond = Cond();
				MatchTokenRparent(); // )
				
				//cur->cond->_true = label();  // label(Cond.true)
				//gen(BUFFER, cur->cond->_true);  // ���cond��û��������м����

				cur->stmt1 = Stmt();
				cur->stmt1->next = cur->next;

			}

			//gen(isFirst, GOTO, cur->stmt1->next);    

			if (lookHeadName == "ELSETK") { // else

				if (isFirst) {
					MatchTokenElse(); // else
					//cur->cond->_false = label();  // label(Cond.false)
				}
				
				if (isFirst) cur->stmt2 = Stmt();

					return cur;
				cur->stmt2->next = cur->next;

			}
			//cur->next = label();
			--IfNum;
		}
	}
	else if (lookHeadName == "WHILETK") { // while
		IfNum++;
		insertFlag(emInLoop);
		int cond_false;
		bool isFirst;

		int body_begin = -1, body_end = -1;

		MatchTokenWhile(); // while
		//cur->begin = label();  // label(stmt.begin)
		//cur->cond->_false = tstmt.next;
		MatchTokenLparent(); // (
		body_begin = readIndex;
		cond_false = cur->next;
		cur->cond = Cond(); // cond
		body_end = readIndex;
		cur->cond->_false = cond_false;
		MatchTokenRparent(); // )

		std::vector<std::string> condBody(_tokenVector.begin() + body_begin, _tokenVector.begin() + body_end);
		std::vector<std::string> condNameBody(_tokenNameVector.begin() + body_begin, _tokenNameVector.begin() + body_end);
		//cur->cond->_true = label();  // label(Cond.true)
		//gen(isFirst, BUFFER, cur->cond->_true, "ռλ��", cur->cond->_false);

		body_begin = readIndex;
		cur->stmt1 = Stmt();
		body_end = readIndex;

		std::vector<std::string> whileBody(_tokenVector.begin() + body_begin, _tokenVector.begin() + body_end);
		std::vector<std::string> whileNameBody(_tokenNameVector.begin() + body_begin, _tokenNameVector.begin() + body_end);

		auto while_cond = cur->cond->cond;

		cur->stmt1->next = cur->begin;
			
		//gen(isFirst, GOTO, cur->begin);
		//cur->next = label();
		//flushCodeBuffer(cur->cond->_true, cur->next, IfNum);

		--IfNum;
		eraseFlag();
	}
	else if (lookHeadName == "BREAKTK") { // break
		MatchTokenBreak(); // break
		MatchTokenSemicn(I); // ;
	}
	else if (lookHeadName == "CONTINUETK") { // continue
		MatchTokenContinue(); // continue
		MatchTokenSemicn(I); // ;
	}
	else if (lookHeadName == "RETURNTK") { // return
		MatchTokenReturn(); // return
		if (lookHeadName != "SEMICN") {
			if (currentEmFlag == emInFunc) { // ��ǰ�Ǻ�������
				TableSpace::FuncItem funcItem = TableSpace::getLastestFunc();
				if (funcItem.retType == TableSpace::Void)
					errorHandle(F, _lineNumberVector[readIndex]);
			}
			cur->exp = Exp();
		}
		else cur->retVal = -1;  // -1�����޷���ֵ
		MatchTokenSemicn(I); // ;
		haveReturn = true;
	}
	else if (lookHeadName == "LBRACE") { // Block
		cur->block = Block();
	}
	else if (isHaveWantInStmt("=")) { // ѡ��LVal
		if (TableSpace::isConstIdent(lookHead)) { // �жϳ���
			errorHandle(H, _lineNumberVector[readIndex]);
		}

		cur->lval = LVal();
		MatchTokenAssign(); // =
		if (lookHeadName == "GETINTTK") { // ѡ��getint
			MatchTokenGetint(); // getint
			MatchTokenLparent(); // (
			MatchTokenRparent(); // )
			MatchTokenSemicn(I); // ;
			
		}
		else { // ѡ��Exp
			cur->exp = Exp();
			MatchTokenSemicn(I); // ;
		}
		//gen(true, ASSIGN, cur->exp->lexeme, cur->lval->ident->lexeme);
	}
	else {
		if (lookHeadName == "SEMICN") { // ;
			MatchTokenSemicn(I);
		}
		else {
			cur->exp = Exp();
			MatchTokenSemicn(I);
		}
	}
	outFileParserResult("Stmt");
	return cur;
}
// 18-�Ӽ����ʽ
std::shared_ptr<AddExpNode> AddExp() {
	auto cur = std::make_shared<AddExpNode>("AddExp");

	bool isUnaryOp = true;
	if (lookHeadName != "PLUS" && lookHeadName != "MINU") {
		cur->mul_exp = MulExp(); // �����if��䣬˵�������������ſ�ͷ
		cur->val = cur->mul_exp->val;
		
		isUnaryOp = false;
		outFileParserResult("AddExp");
	}
	else { // �����if��䣬˵���������ſ�ͷ
		if (_tokenNameVector[readIndex - 1] != "PLUS"
			&& _tokenNameVector[readIndex - 1] != "MINU"
			&& currentUnaryExpCount != 0) { // �ж��Ƿ�Ҫ�²���һ���������
			preUnaryExpCount = currentUnaryExpCount;
		}
		currentUnaryExpCount = 1;
		cur->mul_exp = std::make_shared<MulExpNode>("NONE");
		cur->mul_exp->temp_name = "UnaryOp";
		cur->val = 0;
	}
	cur->_add_exp = AddExp_(isUnaryOp);
	if (cur->_add_exp) {
		cur->temp_name = "t" + std::to_string(++TempNum);

		if (cur->mul_exp->temp_name == "UnaryOp") {
			if (cur->_add_exp->_add_exp) {
				if (cur->_add_exp->_add_exp->op_type == "+")  // -e+a
					cur->val = -1 * cur->_add_exp->mul_exp->val + cur->_add_exp->_add_exp->val;
				else cur->val = -1 * cur->_add_exp->mul_exp->val - cur->_add_exp->_add_exp->val;
			}
			else {  // -5
				if (cur->_add_exp->op_type == "+")
					cur->val = cur->_add_exp->mul_exp->val;
				else cur->val = -1 * cur->_add_exp->mul_exp->val;
			}
		}
		else {
			if (cur->_add_exp->op_type == "+") cur->val += cur->_add_exp->val;
			else if (cur->_add_exp->op_type == "-") {
				if (cur->_add_exp->_add_exp) {  // -a%b+c
					if (cur->_add_exp->_add_exp->op_type == "+")
						cur->val += -1 * cur->_add_exp->mul_exp->val + cur->_add_exp->_add_exp->val;
					else if (cur->_add_exp->_add_exp->op_type == "-")
						cur->val = -1 * cur->_add_exp->mul_exp->val - cur->_add_exp->_add_exp->val;
				}
				else cur->val -= cur->_add_exp->val;
			}
			//gen(true, EXP, cur->mul_exp->temp_name, cur->_add_exp->op_type, cur->_add_exp->temp_name, cur->temp_name);
		}

	}
	else cur->temp_name = cur->mul_exp->temp_name;
	return cur;
}
std::shared_ptr<AddExp_Node> AddExp_(const bool& isUnaryOp) {
 	auto cur = std::make_shared<AddExp_Node>("AddExp_");
	cur->op_type = lookHead;

	bool nextIsUnaryOp = false;
	if (lookHeadName == "PLUS") MatchTokenPlus(); // +
	else if (lookHeadName == "MINU") MatchTokenMinu(); // -
	else return nullptr;
	if (isUnaryOp) { // ��ʱ�Ӽ�����������
		outFileParserResult("UnaryOp");
		bracketNameStack.push("PNSign");
	}
	if (lookHeadName != "PLUS"
		&& lookHeadName != "MINU") {
		cur->mul_exp = MulExp();
		cur->val = cur->mul_exp->val;
		
		outFileParserResult("AddExp");
	}
	else { // ���ڴ���+-+5�����0
		nextIsUnaryOp = true;
		++currentUnaryExpCount;

		// ��ֹ����mul_expֵ�쳣
		cur->mul_exp = std::make_shared<MulExpNode>("NONE");
		cur->mul_exp->temp_name = "UnaryOp";
		cur->val = 0;
	}
 	cur->_add_exp = AddExp_(nextIsUnaryOp);
	if (cur->_add_exp) {
		cur->temp_name = "t" + std::to_string(++TempNum);

		if (cur->_add_exp->op_type == "+") cur->val += cur->_add_exp->val;

		//else if (cur->_add_exp->op_type == "-") cur->val -= cur->_add_exp->val;
		else {
			if (cur->_add_exp->_add_exp) {  // -a%b+c
				if (cur->_add_exp->_add_exp->op_type == "+")
					cur->val += -1 * cur->_add_exp->mul_exp->val + cur->_add_exp->_add_exp->val;
				else if (cur->_add_exp->_add_exp->op_type == "-")
					cur->val = -1 * cur->_add_exp->mul_exp->val - cur->_add_exp->_add_exp->val;
			}
			else cur->val -= cur->_add_exp->val;
		}
		//gen(true, EXP, cur->val, cur->_add_exp->op_type, cur->_add_exp->val);
	}
	else {
		cur->temp_name = cur->mul_exp->temp_name;
		cur->val = cur->mul_exp->val;
	}
	
	return cur;
}
// 19-���ʽ
std::shared_ptr<ExpNode> Exp(const bool& isConstExp) {
	auto cur = std::make_shared<ExpNode>("Exp");

	cur->add_exp = AddExp();
	cur->val = cur->add_exp->val;
	cur->lexeme = cur->add_exp->temp_name;
	if (isConstExp) outFileParserResult("ConstExp");
	else outFileParserResult("Exp");

	return cur;
}
// 20-��ֵ���ʽ
std::shared_ptr<LValNode> LVal() {
	auto cur = std::make_shared<LValNode>("LVal");

	cur->ident = Ident(false);
	cur->lexeme = cur->ident->lexeme;
	while (lookHeadName == "LBRACK") { // [
		MatchTokenLbrack(); // [
		bracketNameStack.push("LBRACK");
		cur->exp = Exp();
		MatchTokenRbrack(K); // ]
		bracketNameStack.pop();
	}

	if (cur->lexeme == "i")
		printf("");
	
	// ��ident��Ӧ��ֵȡ����
	auto temp = TableSpace::query(cur->lexeme);
	cur->val = std::atoi((temp.value).c_str());

	outFileParserResult("LVal");
	return cur;
}
// 21-�������ʽ
std::shared_ptr<CondNode> Cond() {
	auto cur = std::make_shared<CondNode>("Cond");

	if (lookHeadName == "NOT") {
		MatchTokenNot();
		outFileParserResult("UnaryOp");
		++currentUnaryExpCount;
		bracketNameStack.push("NOT");
	}
	cur->lor_exp = LOrExp();
	cur->cond = cur->lor_exp->cond;
	//gen("if" + globalCurrentToken + "goto" + std::to_string(tcond._true));
	outFileParserResult("Cond") ;

	return cur;
}
// 22-��ʽ�ַ����ս��
std::shared_ptr<FormatCharNode> FormatString() {
	auto cur = std::make_shared<FormatCharNode>("FormatString");
	std::string searchStr = "%d";
	cur->out_string = lookHead.substr(1, lookHead.size()-2);
	cur->d_num = 0;

	size_t pos = 0;
	while ((pos = lookHead.find(searchStr, pos)) != std::string::npos) {
		++cur->d_num;
		pos += searchStr.length();
	}

	MatchTokenFormat();
	return cur;
}
// 23-�˳�ģ���ʽ
std::shared_ptr<MulExpNode> MulExp() {
	auto cur = std::make_shared<MulExpNode>("MulExp");

	cur->unary_exp = UnaryExp();

	cur->val = cur->unary_exp->val;
	cur->temp_name = cur->unary_exp->lexeme;

	outFileParserResult("MulExp");
	if (lookHead == "/")
		printf("");

	cur->_mul_exp = MulExp_();
	if (cur->_mul_exp) {
		if (cur->_mul_exp->op_type == "*") cur->val = cur->val * cur->_mul_exp->val;
		else if (cur->_mul_exp->op_type == "/") {
			if (cur->_mul_exp->val != 0) 
				cur->val = cur->val / cur->_mul_exp->val;
			else;  // ������Ϊ0��������
		}
		else if (cur->_mul_exp->op_type == "%") cur->val = cur->val % cur->_mul_exp->val;
		//gen(true, EXP, cur->val, cur->_mul_exp->op_type, cur->_mul_exp->val);
	}
	return cur;
}
std::shared_ptr<MulExp_Node> MulExp_() {
	auto cur = std::make_shared<MulExp_Node>("MulExp_");
	cur->op_type = lookHead;

	if (lookHeadName == "MULT") MatchTokenMult(); // *
	else if (lookHeadName == "DIV") MatchTokenDiv(); // /
	else if (lookHeadName == "MOD") MatchTokenMod(); // %
	else return nullptr;
	cur->unary_exp = UnaryExp();
	cur->val = cur->unary_exp->val;
	
	outFileParserResult("MulExp");
	cur->_mul_exp = MulExp_();

	if (cur->_mul_exp) {  // ��Ч�����ݲ�ͬ��������������������
		cur->temp_name = "t" + std::to_string(++TempNum);

		if (cur->_mul_exp->op_type == "*") cur->val = cur->val * cur->_mul_exp->val;
		else if (cur->_mul_exp->op_type == "/") cur->val = cur->val / cur->_mul_exp->val;
		else if (cur->_mul_exp->op_type == "%") cur->val = cur->val % cur->_mul_exp->val;
		//gen(true, EXP, cur->val, cur->_mul_exp->op_type, cur->_mul_exp->val);
	}
	else cur->temp_name = cur->unary_exp->lexeme;
	return cur;
}
// 24-�߼�����ʽ
std::shared_ptr<LOrExpNode> LOrExp() {
	auto cur = std::make_shared<LOrExpNode>("LOrExp");

	cur->land_exp = LAndExp();
	outFileParserResult("LOrExp");
	cur->_lor_exp = LOrExp_();

	if (cur->_lor_exp) {
		if (cur->_lor_exp->op_type == "||" && (cur->land_exp->cond || cur->_lor_exp->cond))
			cur->cond = true;
		else cur->cond = false;
	}
	else cur->cond = cur->land_exp->cond;

	// ����������
	BackPatch(cur->land_exp->falselist, cur->_lor_exp->m->quad);
	cur->truelist = Merge(cur->land_exp->truelist, cur->_lor_exp->truelist);
	cur->falselist = cur->_lor_exp->falselist;

	return cur;
}
std::shared_ptr<LOrExp_Node> LOrExp_() {
	auto cur = std::make_shared<LOrExp_Node>("LOrExp_");
	cur->op_type = lookHead;

	if (lookHeadName == "OR") MatchTokenOr(); // ||
	else return nullptr; // null

	cur->m = M_();
	if (lookHeadName == "NOT") {
		MatchTokenNot();
		outFileParserResult("UnaryOp");
		++currentUnaryExpCount;
		bracketNameStack.push("NOT");
	}
	cur->land_exp = LAndExp();
	cur->cond = cur->land_exp->cond;

	outFileParserResult("LOrExp");
	cur->_lor_exp = LOrExp_();

	return cur;
}
// 25-�߼�����ʽ
std::shared_ptr<LAndExpNode> LAndExp() {
	auto cur = std::make_shared<LAndExpNode>("LAndExp");

	cur->eq_exp = EqExp();
	outFileParserResult("LAndExp");
	cur->_land_exp = LAndExp_();

	if (cur->_land_exp) {
		if (cur->_land_exp->op_type == "&&" && cur->eq_exp->cond && cur->_land_exp->cond)
			cur->cond = true;
		else cur->cond = false;
	}
	else cur->cond = cur->eq_exp->cond;

	// ����������
	BackPatch(cur->eq_exp->falselist, cur->_land_exp->m->quad);
	cur->truelist = cur->_land_exp->truelist;
	cur->falselist = Merge(cur->eq_exp->falselist, cur->_land_exp->falselist);

	return cur;
}
std::shared_ptr<LAndExp_Node> LAndExp_() {
	auto cur = std::make_shared<LAndExp_Node>("LAndExp_");
	cur->op_type = lookHead;

	if (lookHeadName == "AND") MatchTokenAnd(); // &&
	else return nullptr; // null
	cur->m = M_();
	if (lookHeadName == "NOT") {
		MatchTokenNot();
		outFileParserResult("UnaryOp");
		++currentUnaryExpCount;
		bracketNameStack.push("NOT");
	}
	cur->eq_exp = EqExp();
	cur->cond = cur->eq_exp->cond;  // ��ֻ���Ǽ򵥵����

	outFileParserResult("LAndExp");
	cur->_land_exp = LAndExp_();

	return cur;
}
// 26-����Ա��ʽ
std::shared_ptr<EqExpNode> EqExp() {
	auto cur = std::make_shared<EqExpNode>("EqExp");

	cur->rel_exp = RelExp();

	outFileParserResult("EqExp");
	cur->_eq_exp = EqExp_();

	if (cur->_eq_exp) {
		if (cur->_eq_exp->op_type == "==" && cur->rel_exp->val == cur->_eq_exp->rel_exp->val)
			cur->cond = true;
		else if (cur->_eq_exp->op_type == "!=" && cur->rel_exp->val != cur->_eq_exp->rel_exp->val)
			cur->cond = true;
		else cur->cond = false;
	}
	else cur->cond = cur->rel_exp->cond;

	return cur;
}
std::shared_ptr<EqExp_Node> EqExp_() {
	auto cur = std::make_shared<EqExp_Node>("EqExp_");
	cur->op_type = lookHead;

	if (lookHeadName == "EQL") MatchTokenEql(); // ==
	else if (lookHeadName == "NEQ") MatchTokenNeq(); // !=
	else return nullptr;
	if (lookHeadName == "NOT") MatchTokenNot();
	cur->rel_exp = RelExp();
	outFileParserResult("EqExp");
	cur->_eq_exp = EqExp_();

	return cur;
}
// 27-һԪ���ʽ
std::shared_ptr<UnaryExpNode> UnaryExp() {
	auto cur = std::make_shared<UnaryExpNode>("UnaryExp");

	if (lookHead == "fib")
		printf("");

	if (GetNextSecondToken() == "LPARENT") { // (����������
		insertFlag(emFuncUse);
		
		// �õ����õĺ�������
		TableSpace::FuncItem outcome = TableSpace::FuncQuery(lookHead);
		std::vector<std::pair<std::string, std::string>> typeNameVector = outcome.typeNameVector;
		int funParamNum = 0;

		cur->ident = Ident(true); // ��λ�õ�true��Ϊ�˼����鷳�����ǲ�������⣬����������Ƿ���ǰ����
		MatchTokenLparent(); // (
		if (lookHeadName != "RPARENT") { // )
			// ������Ҫ�Ժ����βν��и�ֵ
			cur->func_r_params = FuncRParams(funParamNum, typeNameVector);
		}
		
		// ������-��������������ƥ��
		if (funParamNum != outcome.paramNum)
			errorHandle(D, _lineNumberVector[readIndex]);

		MatchTokenRparent(J); // )
		eraseFlag();
	}
	/*else if (lookHeadName == "PLUS" || lookHeadName == "MIUS"
		|| lookHeadName == "NOT")
		UnaryOp();*/
	else {
		cur->primary_exp = PrimaryExp();
		cur->val = cur->primary_exp->val;
		cur->lexeme = cur->primary_exp->lexeme;
	}

	// ���ڴ���������UnaryExp
	if (!bracketNameStack.empty() 
		&& bracketNameStack.top() != "LPARENT" && bracketNameStack.top() != "LBRACK") {
		for (int ii = 0; ii < currentUnaryExpCount+1; ii++)
			outFileParserResult("UnaryExp");
		if (preUnaryExpCount != 0) {
			currentUnaryExpCount = preUnaryExpCount;
			preUnaryExpCount = 0;
		}
		else currentUnaryExpCount = 0;
		bracketNameStack.pop();
	}
	else outFileParserResult("UnaryExp");

	return cur;
}
// 28-��ϵ���ʽ
std::shared_ptr<RelExpNode> RelExp() {
	auto cur = std::make_shared<RelExpNode>("RelExp");

	cur->add_exp = AddExp();
	
	outFileParserResult("RelExp");
	cur->_rel_exp = RelExp_();

	if (cur->_rel_exp) {
		if (cur->_rel_exp->op_type == "<" && cur->add_exp->val < cur->_rel_exp->add_exp->val)
			cur->cond = true;
		else if (cur->_rel_exp->op_type == ">" && cur->add_exp->val > cur->_rel_exp->add_exp->val)
			cur->cond = true;
		else if (cur->_rel_exp->op_type == "<=" && cur->add_exp->val <= cur->_rel_exp->add_exp->val)
			cur->cond = true;
		else if (cur->_rel_exp->op_type == ">=" && cur->add_exp->val >= cur->_rel_exp->add_exp->val)
			cur->cond = true;
		else cur->cond = false;

		//gen(true, IF, cur->add_exp->temp_name, cur->_rel_exp->op_type, cur->_rel_exp->add_exp->temp_name);
	}
	else {
		cur->cond = cur->add_exp->val == 0 ? false : true;  // ֻ��addexp
		cur->val = cur->add_exp->val;
	}

	cur->truelist = MakeList(LineNum);
	cur->falselist = MakeList(LineNum + 1);
	gen("if" + cur->add_exp->temp_name + cur->_rel_exp->op_type + cur->_rel_exp->add_exp->temp_name + "goto ");
	gen("goto ");

	return cur;
}
std::shared_ptr<RelExp_Node> RelExp_() {
	auto cur = std::make_shared<RelExp_Node>("RelExp_");
	cur->op_type = lookHead;

	if (lookHeadName == "LSS") MatchTokenLss(); // <
	else if (lookHeadName == "GRE") MatchTokenGre(); // >
	else if (lookHeadName == "LEQ") MatchTokenLeq(); // <=
	else if (lookHeadName == "GEQ") MatchTokenGeq(); // >=
	else return nullptr;
	if (lookHeadName == "NOT") MatchTokenNot();
	cur->add_exp = AddExp();
	outFileParserResult("RelExp");
	cur->_rel_exp = RelExp_();

	if (cur->_rel_exp) {
		if (cur->_rel_exp->op_type == "<" && cur->add_exp->val < cur->_rel_exp->add_exp->val)
			cur->cond = true;
		else if (cur->_rel_exp->op_type == ">" && cur->add_exp->val > cur->_rel_exp->add_exp->val)
			cur->cond = true;
		else if (cur->_rel_exp->op_type == "<=" && cur->add_exp->val <= cur->_rel_exp->add_exp->val)
			cur->cond = true;
		else if (cur->_rel_exp->op_type == ">=" && cur->add_exp->val >= cur->_rel_exp->add_exp->val)
			cur->cond = true;
		else cur->cond = false;
	}

	return cur;
}
// 29-<Char>
std::shared_ptr<CharNode> Char() {
	auto cur = std::make_shared<CharNode>("Char");

	if (lookHead[0] == '%') { // ѡ��ForMatchar
		cur->format_char = Formatchar();
	}
	else {
		cur->normal_char = NormalChar();
	}
	return cur;
}
// 30-<ForMatchar>
std::shared_ptr<FormatCharNode> Formatchar() {
	auto cur = std::make_shared<FormatCharNode>("ForMatchar");

	if (lookHead == "%d") {
		GetNextToken();
	}
	return cur;
}
// 31-<NormalChar>
std::shared_ptr<NormalCharNode> NormalChar() {
	return nullptr;
}
// 32-�������ʽ
std::shared_ptr<PrimaryExpNode> PrimaryExp() {
	auto cur = std::make_shared<PrimaryExpNode>("PrimaryExp");

	if (lookHeadName == "LPARENT") { // ƥ���ս��(
		MatchTokenLparent(); // (
		bracketNameStack.push("LPARENT");
		cur->exp = Exp();
		cur->val = cur->exp->val;
		MatchTokenRparent(); // )
		bracketNameStack.pop();
	}
	else if ((lookHead >= "A" && lookHead <= "Z")
		|| (lookHead >= "a" && lookHead <= "z")) { // �ж�Ϊfirst(Ident)
		cur->lval = LVal();
		cur->lexeme = cur->lval->lexeme;
		cur->val = cur->lval->val;
	}
	else {
		cur->number = Number();
		cur->val = cur->number->val;
	}
	outFileParserResult("PrimaryExp");

	return cur;
}
// 33-����ʵ�α�
std::shared_ptr<FuncRParamsNode> FuncRParams(int& funParamNum, const std::vector<std::pair<std::string, std::string>>& typeNameVector) {
	auto cur = std::make_shared<FuncRParamsNode>("FuncRParams");

	int idx = 0;
	std::string identType;
	// ����ǲ��Ǻ�������
	if (_tokenVector[readIndex + 1] == "(") {
		auto funcInfo = TableSpace::FuncQuery(lookHead);
		identType = funcInfo.retType == TableSpace::Int ? "" : "null";
	}
	else {
		// �ӷ��ű���ȡ�ñ������������Ϣ
		TableSpace::TableItem identInfo = TableSpace::query(lookHead);
		identType = identInfo.type;

		if (identInfo.kind == TableSpace::null) { // �ӱ������ű���û�в鵽�����Ϣ
			// �ж��ǲ��ǵ�ǰ�����β�
			std::vector<std::pair<std::string, std::string>> typeNameVector = TableSpace::getLastestFunc().typeNameVector;
			for (const auto& pair : typeNameVector) {
				if (pair.second == lookHead) { // ���βα���
					identType = pair.first;
					break;
				}
			}
		}
	}
	
	if (identType == "null") {
		errorHandle(E, _lineNumberVector[readIndex]);
	}

	else {
		// identType�Ǳ��������ά��
		int count = IdentType(); // count��ָ��������ʵ�ε�ά�������������ά���������ά��-���õ�ά��
		int realType = identType.size() - count;
		// �жϸñ��������Ƿ��뺯����������ƥ��
		if (identType.size() != 0) { // ����������
			if (realType != typeNameVector[idx].first.size())
				errorHandle(E, _lineNumberVector[readIndex]);
		}
		else { // ������������
			if ((!typeNameVector.empty() && count != typeNameVector[idx].first.size()))
				errorHandle(E, _lineNumberVector[readIndex]);
		}
	}
	
	auto exp = Exp();
	cur->exp.push_back(exp);
	++funParamNum;
	while (lookHeadName == "COMMA") { // ,
		MatchTokenComma(); // ,
		auto exp = Exp();
		if (exp->lexeme != "") {  // ˵����һ������
			auto identInfo = TableSpace::query(exp->lexeme);
			exp->val = std::atoi((identInfo.value).c_str());
		}
		cur->exp.push_back(exp);
		++funParamNum;
	}
	outFileParserResult("FuncRParams");

	return cur;
}
// 34-��ֵ
std::shared_ptr<NumberNode> Number() {
	auto cur = std::make_shared<NumberNode>("Number");

	cur->int_const = IntConst();
	cur->val = cur->int_const->val;
	outFileParserResult("Number");
	return cur;
}
// 35-����
std::shared_ptr<IntConstNode> IntConst() {
	auto cur = std::make_shared<IntConstNode>("IntConst");

	if (lookHead == "0") { // ƥ���ս��0
		MatchTokenZero();
	}
	else {
		cur->decimal_const = decimal_const();
		cur->val = cur->decimal_const->val;
	}
	return cur;
}
// 36-ʮ������
std::shared_ptr<DecimalConstNode> decimal_const() {
	auto cur = std::make_shared<DecimalConstNode>("decimal_const");
	cur->val = std::stoi(lookHead);

	int currentTokenLen = lookHead.size();
	if (currentTokenLen == 1) {
		cur->non_zero_digit = nonzero_digit();
	}
	else {
		MatchTokenDigit();
	}

	return cur;
}
// 37-��������
std::shared_ptr<NonzeroDigitNode> nonzero_digit() {
	auto cur = std::make_shared<NonzeroDigitNode>("nonzero_digit");

	MatchTokenNonZeroDigit();
	return cur;
}
// 38-��Ŀ�����
std::shared_ptr<UnaryOpNode> UnaryOp() {
	auto cur = std::make_shared<UnaryOpNode>("UnaryOp");

	if (lookHeadName == "PLUS")
		MatchTokenPlus();
	else if (lookHeadName == "MINU")
		MatchTokenMinu();
	else MatchTokenNot();
	outFileParserResult("UnaryOp");

	return cur;
}
// 39-������ֵ
std::shared_ptr<ConstInitValNode> ConstInitVal() {
	return nullptr;
}
// 40-��������
std::shared_ptr<ConstDefNode> ConstDef() {
	return nullptr;
}
std::shared_ptr<MNode> M_() {
	auto cur = std::make_shared<MNode>("M");
	cur->quad = LineNum;
	return cur;
}



void GetNextToken() { // ͬʱ�����ǰƥ����ս����Ϣ���ļ���
	if (outputFile.is_open() && LexOut) {
		outputFile << lookHeadName << " " << _tokenVector[readIndex] << std::endl;
	}
	if (readIndex + 1 < _tokenNameVector.size()) {
		lookHead = _tokenVector[readIndex + 1];
		lookHeadName = _tokenNameVector[++readIndex];
	}
}

void PrintLookHead() { // �����ӡ10������
	assert(readIndex + 10 < _tokenNameVector.size());
	for (int ii = 0; ii < 10; ii++)
		std::cout << _tokenVector[readIndex + ii] << " ";
	std::cout << std::endl;
}

bool isInfunction() {
	for (const EmFlagIdent flag : flagStack) {
		if (flag == emInFunc)
			return true;
	}
	return false;
}

// ƥ���ս��int
EmType MatchTokenInt() {
	if (lookHeadName == "INTTK") {
		GetNextToken();
		return Int;
	}
	else { // ������
		return emTypeNull;
	}
}
// ƥ���ս��main
void MatchTokenMain() {
	if (lookHeadName == "MAINTK") {
		GetNextToken();
	}
	else { // ������

	}
}
// ƥ���ս��break
void MatchTokenBreak() {
	if (lookHeadName == "BREAKTK" && currentEmFlag == emInLoop) {
		GetNextToken();
	}
	else { // �ڷ�ѭ����ʹ��break
		errorHandle(M, _lineNumberVector[readIndex]);
		GetNextToken();
	}
}
// ƥ���ս��continue
void MatchTokenContinue() {
	if (lookHeadName == "CONTINUETK" && currentEmFlag == emInLoop) {
		GetNextToken();
	}
	else { // �ڷ�ѭ����ʹ��continue
		errorHandle(M, _lineNumberVector[readIndex]);
		GetNextToken();
	}
}
// ƥ���ս��if
void MatchTokenIf() {
	if (lookHeadName == "IFTK") {
		GetNextToken();
	}
	else { // ������

	}
}
// ƥ���ս��else
void MatchTokenElse() {
	if (lookHeadName == "ELSETK") {
		GetNextToken();
	}
	else { // ������

	}
}
// ƥ���ս��while
void MatchTokenWhile() {
	if (lookHeadName == "WHILETK") {
		GetNextToken();
	}
	else { // ������

	}
}
// ƥ���ս��Ident������
void MatchTokenIdent(const bool& isDecl) {

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
				GetNextToken();
				return;
			}
		}
	}

	if ((lookHead[0] >= 'a' && lookHead[0] <= 'z') // �жϺϷ�������
		|| (lookHead[0] >= 'A' && lookHead[0] <= 'Z')) {
		if (lookHead == "change1" && readIndex == 28)
			printf("");
		GetNextToken();
	}
}
// ƥ���ս��getint
void MatchTokenGetint() {
	if (lookHeadName == "GETINTTK") {
		GetNextToken();
	}
	else { // ������
		std::cout << "error: getint" << std::endl;
	}
}
// ƥ���ս��printf
void MatchTokenPrintf() {
	if (lookHeadName == "PRINTFTK") {
		GetNextToken();
	}
	else { // ������
		std::cout << "error: printf" << std::endl;
	}
}
// ƥ���ս��return
void MatchTokenReturn() {
	if (lookHeadName == "RETURNTK") {
		GetNextToken();
	}
	else { // ������
		std::cout << "error: return" << std::endl;
	}
}
// ƥ���ս��void
void MatchTokenVoid() {
	if (lookHeadName == "VOIDTK") {
		GetNextToken();
	}
	else { // ������
		std::cout << "error: void" << std::endl;
	}
}
// ƥ���ս��format
void MatchTokenFormat() {
	size_t found1 = lookHead.find("$");
	size_t found2 = lookHead.find("&");
	if (found1 == std::string::npos && found2 == std::string::npos) {
		GetNextToken();
	}
	else { // ���зǷ��ַ�
		errorHandle(A, _lineNumberVector[readIndex]);
		GetNextToken();
	}
}
// const
void MatchTokenConst() {
	if (lookHeadName == "CONSTTK") {
		GetNextToken();
	}
	else { // ������
		std::cout << "error: const" << std::endl;
	}
}

// ƥ���ս��,
void MatchTokenComma() {
	if (lookHeadName == "COMMA") {
		GetNextToken();
	}
	else { // ������
		PrintLookHead();
		std::cout << "error: ," << std::endl;
	}
}
// ƥ���ս��;
void MatchTokenSemicn(const emErrorType& errorType) {
	if (lookHeadName == "SEMICN") {
		GetNextToken();
	}
	else { // ȱ�ٷֺ�
		errorHandle(errorType, _lineNumberVector[readIndex - 1]); // �����к�Ϊ�ֺ�ǰһ�����ս�������кš�
	}
}
// ƥ���ս��=
void MatchTokenAssign() {
	if (lookHeadName == "ASSIGN") {
		GetNextToken();
	}
	else { // ������
		std::cout << "readIndex:" << readIndex << std::endl;
		std::cout << "error: =" << std::endl;
	}
}
// ƥ���ս��<
void MatchTokenLss() {
	if (lookHeadName == "LSS") {
		GetNextToken();
	}
	else { // ������
		std::cout << "error: <" << std::endl;
		PrintLookHead();
	}
}
// ƥ���ս��<=
void MatchTokenLeq() {
	if (lookHeadName == "LEQ") {
		GetNextToken();
	}
	else { // ������
		std::cout << "error: <=" << std::endl;
	}
}
// ƥ���ս��>
void MatchTokenGre() {
	if (lookHeadName == "GRE") {
		GetNextToken();
	}
	else { // ������
		std::cout << "error: >" << std::endl;
	}
}
// ƥ���ս��>=
void MatchTokenGeq() {
	if (lookHeadName == "GEQ") {
		GetNextToken();
	}
	else { // ������
		std::cout << "error: >=" << std::endl;
	}
}
// ƥ���ս��==
void MatchTokenEql() {
	if (lookHeadName == "EQL") {
		GetNextToken();
	}
	else { // ������
		std::cout << "error: ==" << std::endl;
	}
}
// ƥ���ս��!=
void MatchTokenNeq() {
	if (lookHeadName == "NEQ") {
		GetNextToken();
	}
	else { // ������
		std::cout << "error: !=" << std::endl;
	}
}
// ƥ���ս��!
void MatchTokenNot() {
	if (lookHeadName == "NOT") {
		GetNextToken();
	}
	else { // ������
		std::cout << "error: !" << std::endl;
	}
}
// ƥ���ս��&&
void MatchTokenAnd() {
	if (lookHeadName == "AND") {
		GetNextToken();
	}
	else { // ������
		std::cout << "error: &&" << std::endl;
	}
}
// ƥ���ս��||
void MatchTokenOr() {
	if (lookHeadName == "OR") {
		GetNextToken();
	}
	else { // ������
		std::cout << "error: ||" << std::endl;
	}
}
// ƥ���ս��+
void MatchTokenPlus() {
	if (lookHeadName == "PLUS") {
		GetNextToken();
	}
	else { // ������
		std::cout << "error: +" << std::endl;
	}
}
// ƥ���ս��-
void MatchTokenMinu() {
	if (lookHeadName == "MINU") {
		GetNextToken();
	}
	else { // ������
		std::cout << "error: -" << std::endl;
	}
}
// ƥ���ս��*
void MatchTokenMult() {
	if (lookHeadName == "MULT") {
		GetNextToken();
	}
	else { // ������
		std::cout << "error: -" << std::endl;
	}
}
// ƥ���ս��/
void MatchTokenDiv() {
	if (lookHeadName == "DIV") {
		GetNextToken();
	}
	else { // ������
		std::cout << "error: /" << std::endl;
	}
}
// ƥ���ս��%
void MatchTokenMod() {
	if (lookHeadName == "MOD") {
		GetNextToken();
	}
	else { // ������
		std::cout << "error: %" << std::endl;
	}
}
// ƥ���ս��(
void MatchTokenLparent() {
	if (lookHeadName == "LPARENT") {
		GetNextToken();
	}
	else { // ������
		std::cout << "error: (" << std::endl;
	}
}
// ƥ���ս��)
void MatchTokenRparent(const emErrorType& errorType) {
	if (lookHeadName == "RPARENT") {
		GetNextToken();
	}
	else { // ȱ�� )
		errorHandle(errorType, _lineNumberVector[readIndex - 1]); // ��������Ϊǰһ�����ս������λ��
	}
}
// ƥ���ս��{
void MatchTokenLbrace() {
	if (lookHeadName == "LBRACE") {
		GetNextToken();
	}
	else { // ������
		std::cout << "error: {" << std::endl;
	}
}
// ƥ���ս��}
void MatchTokenRbrace() {
	if (lookHeadName == "RBRACE") {
		GetNextToken();
	}
	else { // ������
		std::cout << "error: }" << std::endl;
	}
}
// ƥ���ս��[
void MatchTokenLbrack() {
	if (lookHeadName == "LBRACK") {
		GetNextToken();
	}
	else { // ������

	}
}
// ƥ���ս��]
void MatchTokenRbrack(const emErrorType& errorType) {
	if (lookHeadName == "RBRACK") {
		GetNextToken();
	}
	else { // ȱ����������
		errorHandle(errorType, _lineNumberVector[readIndex]);
	}
}
// ��������0
void MatchTokenZero() {
	if (lookHeadName == "INTCON" && lookHead >= "0") {
		GetNextToken();
	}
	else { // ������

	}
}
// ��������1,2,...,9
void MatchTokenNonZeroDigit() {
	if (lookHeadName == "INTCON" && lookHead >= "1" && lookHead <= "9") {
		GetNextToken();
	}
	else { // ������

	}
}
// ��������0,1,2,...,9
void MatchTokenDigit() {
	if (lookHeadName == "INTCON" && lookHead >= "0") {
		GetNextToken();
	}
	else { // ������

	}
}

void outFileParserResult(const std::string& str) {
	if (!ParserOut) return;
	outputFile << "<" << str << ">" << std::endl;
}

void outFileSemanticResult(std::string& str, const std::vector<int>& vals) {
	if (vals.empty()) {  // û��%d

	}
	else {  // �滻%d
		for (const int val : vals) {
			str.replace(str.find("%d"), 2, std::to_string(val));
		}
	}

	// �ҵ����з���λ��
	size_t newlinePos = str.find('\\\n');

	// ��ȡ�������ַ���
	std::string part1 = str.substr(0, newlinePos);
	if (part1.size() != str.size()) {
		std::string part2 = str.substr(newlinePos + 2);
		outputFile << part1 << std::endl << part2;
	}
	else outputFile << part1;
}