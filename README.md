# 编译原理实验
## 1.词法分析
### 问题描述

请根据给定的文法设计并实现词法分析程序，从源程序中识别出单词，记录其单词类别和单词值，输入输出及处理要求如下：

1. 数据结构和与语法分析程序的接口请自行定义；类别码需按下表格式统一定义；
2. 为了方便进行自动评测，输入的被编译源文件统一命名为 testfile.txt（注意不要写错文件名）；输出的结果文件统一命名为 output.txt（注意不要写错文件名），结果文件中每行按如下方式组织：

单词类别码 单词的字符/字符串形式(中间仅用一个空格间隔)

单词的类别码请统一按如下形式定义：
![](https://secure2.wostatic.cn/static/jRqPCKyyo1mGNU7Rc8C5yJ/image.png?auth_key=1713496648-ckBENgXT7AHmKWpxrbofp1-0-f1ad24b19f04e7e08b7a9fdc317d02d0)
### 输入形式
`testfile.txt`中的符合文法要求的测试程序。  
### 输出形式
要求将词法分析结果输出至`output.txt`中。

### 特别提醒 

1. 读取的字符串要原样保留着便于输出，特别是数字，这里输出的并不是真正的单词值，其实是读入的字符串，单词值需另行记录。
2. 本次作业只考核对正确程序的处理，但需要为今后可能出现的错误情况预留接口。
3. 在今后的错误处理作业中，需要输出错误的行号，在词法分析的时候注意记录该信息。
4. 单词的类别和单词值以及其他关注的信息，在词法分析阶段获取后，后续的分析阶段会使用，请注意记录；

当前要求的输出只是为了便于评测，完成编译器中无需出现这些信息，请设计为方便打开/关闭这些输出的方案。

## 2.语法分析
### 问题描述
请根据给定的文法设计并实现语法分析程序，能基于上次作业的词法分析程序所识别出的单词，识别出各类语法成分。输入输出及处理要求如下：

1. 需按文法规则，用递归子程序法对文法中定义的语法成分进行分析；
2. 为了方便进行自动评测，输入的被编译源文件统一命名为testfile.txt（注意不要写错文件名）；输出的结果文件统一命名为output.txt（注意不要写错文件名）；结果文件中包含如下两种信息：
    1. 按词法分析识别单词的顺序，按行输出每个单词的信息（要求同词法分析作业，对于预读的情况不能输出）。
    2. 在文法中出现（除了`<BlockItem>`,`<Decl>`, `<BType>` 之外）的语法分析成分分析结束前，另起一行输出当前语法成分的名字，形如“`<Stmt>`”（注：未要求输出的语法成分仍需要进行分析，但无需输出）

文法：
![](https://secure2.wostatic.cn/static/enwvcTtDPSEEw1yvG3oN2g/image.png?auth_key=1713496752-uAV6pSgXjJCAvkQiDDihZd-0-6affc726a7d1cfd2e2c40ce1af1a6b3e)

### 输入形式
`testfile.txt`中的符合文法要求的测试程序。  

### 输出形式
按如上要求将语法分析结果输出至output.txt中。

### 特别提醒
  1. 本次作业只考核对正确程序的处理，但需要为今后可能出现的错误情况预留接口。
  2. 当前要求的输出只是为了便于评测，完成编译器中无需出现这些信息，请设计为方便打开/关闭这些输出的方案。

### 样例输入
```C++
int main(){
    int c// <VarDef> 
    ;  // <VarDecl>
    c= getint(); // <Stmt>
    printf("%d",c);
    return c;
}
```

## 3.错误处理
### 问题描述

请根据给定的文法设计并实现错误处理程序，能诊察出常见的语法和语义错误，进行错误局部化处理，并输出错误信息。为了方便自动评测，输入输出及处理要求如下：  

1. 输入的被编译源文件统一命名为 testfile.txt ；错误信息输出到命名为 error.txt 的结果文件中；
2. 结果文件中包含如下两种信息：错误所在的行号 错误的类别码 （行号与类别码之间只有一个空格，类别码严格按照表格中的小写英文字母）

其中错误类别码按下表中的定义输出，行号从1开始计数： 
![](https://secure2.wostatic.cn/static/aN5KxDebYJWvecW9tpJHzj/image.png?auth_key=1713496852-kdnLhPMhYREJkW6gFiDxRY-0-b2fe5b898cd99c5c44f73e567824b73f)

1. **所有错误都不会出现恶意换行的情况**，包括字符、字符串中的换行符、函数调用等等。 
2. 其他类型的错误，错误的行号以能够断定发现出错的第一个符号的行号为准。例如有返回值的函数缺少返回语句的错误，只有当识别到函数末尾的}时仍未出现返回语句，才可以断定出错，报错行号即为}的行号。

为方便对照, 下文给出了文法符号与可能存在的错误的对应关系:

```C++
编译单元    CompUnit → {Decl} {FuncDef} MainFuncDef  
声明       Decl → ConstDecl | VarDecl
常量声明    ConstDecl → 'const' BType ConstDef { ',' ConstDef } ';' // i
基本类型    BType → 'int'
常数定义    ConstDef → Ident { '[' ConstExp ']' } '=' ConstInitVal  // b k
常量初值    ConstInitVal → ConstExp
    | '{' [ ConstInitVal { ',' ConstInitVal } ] '}' 
变量声明    VarDecl → BType VarDef { ',' VarDef } ';' // i
变量定义    VarDef → Ident { '[' ConstExp ']' } // b
    | Ident { '[' ConstExp ']' } '=' InitVal // k
变量初值    InitVal → Exp | '{' [ InitVal { ',' InitVal } ] '}'
函数定义    FuncDef → FuncType Ident '(' [FuncFParams] ')' Block // b g j
主函数定义   MainFuncDef → 'int' 'main' '(' ')' Block // b g j
函数类型    FuncType → 'void' | 'int' 
函数形参表   FuncFParams → FuncFParam { ',' FuncFParam } 
函数形参    FuncFParam → BType Ident ['[' ']' { '[' ConstExp ']' }]  //   b k
语句块     Block → '{' { BlockItem } '}' 
语句块项    BlockItem → Decl | Stmt 
语句  Stmt → LVal '=' Exp ';' | [Exp] ';' | Block // h i
    | 'if' '(' Cond ')' Stmt [ 'else' Stmt ] // j
    | 'while' '(' Cond ')' Stmt // j
    | 'break' ';' | 'continue' ';' // i m
    | 'return' [Exp] ';' // f i
    | LVal '=' 'getint''('')'';' // h i j
    | 'printf''('FormatString{,Exp}')'';' // i j l
表达式 Exp → AddExp 注：SysY 表达式是int 型表达式 
条件表达式   Cond → LOrExp 
左值表达式   LVal → Ident {'[' Exp ']'} // c k
基本表达式   PrimaryExp → '(' Exp ')' | LVal | Number 
数值  Number → IntConst 
一元表达式   UnaryExp → PrimaryExp | Ident '(' [FuncRParams] ')' // c d e j
        | UnaryOp UnaryExp 
单目运算符   UnaryOp → '+' | '−' | '!' 注：'!'仅出现在条件表达式中 
函数实参表   FuncRParams → Exp { ',' Exp } 
乘除模表达式  MulExp → UnaryExp | MulExp ('*' | '/' | '%') UnaryExp 
加减表达式   AddExp → MulExp | AddExp ('+' | '−') MulExp 
关系表达式   RelExp → AddExp | RelExp ('<' | '>' | '<=' | '>=') AddExp
相等性表达式  EqExp → RelExp | EqExp ('==' | '!=') RelExp
逻辑与表达式  LAndExp → EqExp | LAndExp '&&' EqExp
逻辑或表达式  LOrExp → LAndExp | LOrExp '||' LAndExp 
常量表达式   ConstExp → AddExp 注：使用的Ident 必须是常量
格式字符串:
<FormatString> → '"'{<Char>}'"' // a
```

### 输入形式
testfile.txt 中的存在语法或语义错误的测试程序。

### 输出形式
按如上要求将错误处理结果输出至 error.txt 中。

### 样例输入

```C++
const int const1 = 1, const2 = -100;
int change1;
int gets1(int var1,int var2){
   const1 = 999;
   change1 = var1 + var2          return (change1);
}
int main(){
   change1 = 10;
   printf("Hello World$");
   return 0;
}
```

### 样例输出


```C++
4 h
5 i
9 a
```

### 特别提醒
1. 上表中只列举了部分错误类型和报告该错误类型的情况，未包含的错误类型或错误情况，需要自行设计，本次作业考核不涉及；
2. 完成本次作业时，请勿输出词法分析和语法分析作业要求输出的内容；
3. 本次考核之外，发现错误时最好直接输出描述信息，而不是仅给出错误类别码，有助于完善编译器的设计、开发与调试。
4. **每一行中最多只有一个错误。**
5. **本次作业需要建立符号表。**

## 4.中间代码生成
### 问题描述

请在词法分析、语法分析及错误处理作业的基础上，为编译器实现语义分析、代码生成功能。输入输出及处理要求如下：  

- 输入：testfile.txt
- 输出：mips.txt
1. 需根据文法规则及语义约定，采用自顶向下的语法制导翻译技术，进行语义分析并生成目标代码（任选其一）；  
2. 对于选择生成**MIPS**的编译器，约定如下：  

完成编译器，将源文件（统一命名为testfile.txt）编译生成MIPS汇编并输出到文件（统一命名为mips.txt），具体要求包括：
1. 需自行设计四元式中间代码，再从中间代码生成MIPS汇编，请设计实现输出中间代码的有关函数，本次作业不考核，后续会有优化前后中间代码的输出及评判(输出文件命名为学号_姓名_优化前/后中间代码.txt)。
2. 若选择此项任务，后续的作业需参加竞速排序，请提前预留代码优化有关的接口，并设计方便切换开启/关闭优化的模式
3. 自行调试时，可使用Mars仿真器（使用方法见“Mars仿真器使用说明.docx”），提交到平台的编译器只需要能按统一的要求生成MIPS汇编代码文件即可
4. **此类提交的编译器请仅读取testfile.txt文件并生成相应的MIPS代码，编译器自身不要读入标准输入中的内容。**

### 样例输入
testfile.txt 如下：

```C++
int test;  
int main(){  
    printf("Hello World\n");  
    test = getint();  
    printf("%d",test);  
    return 0;  
}  
```

标准输入 input.txt 如下：  

```C++
1906  
```

### 样例输出
output.txt 如下：

```C++
Hello World  
1906  
```

### 评分标准
按与预期结果不一致的行数扣分，每项扣10%。

### 特别说明
1. 本次作业是为了让同学们尽快实现一个完整的编译器，测试程序中仅涉及A级、B级、C级测试样例（15个）。
2. 两种目标代码任选其一完成，若生成PCODE代码，成绩总分为85分；若生成MIPS汇编，成绩总分为100分；请严格按照目标代码的输入输出要求完成，以免造成评判错误。
3. 本次作业仅用正确的测试程序进行考核，但产生的编译器应具有错误处理能力。

### 目标代码说明
1. PCODE代码的定义可参见教材P458, Pascal-S指令代码集，可以进行修改，解释执行程序也可以借鉴Pascal-S编译器源码中的解释执行程序，若PCODE代码修改了请相应修改解释执行程序
2. MIPS代码可以选择基础指令及伪指令，不能选择宏指令； MARS 使用 4.5 版本，关闭延迟槽，内存配置为默认选项
