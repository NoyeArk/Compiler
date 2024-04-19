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
