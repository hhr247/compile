#### 文件结构以及代码结构

```
19335052-何鸿荣-编译原理期末作业
    ├──result [结果]
    │   ├──token.txt [词法分析过程结果]
    │   ├──LL1-Stack output.txt [语法分析过程的语法栈]
    │   ├──action.txt [语义分析结果]
    │   ├──quadruple-output.txt [四元式，语义分析结果]
    │   ├──Symbol_table.txt [符号表内容，符号表结果]
    |	├──optimize.txt [优化后的四元式]
    │   └──target.asm [目标代码生成]
    └──src [源代码]
    │   ├──ASS.h [目标代码生成器头文件]
    │   ├──ASS.cpp [目标代码生成器源文件]
    │   ├──Lexical_analyzer.h [词法分析头文件]
    │   ├──Lexical_analyzer.cpp [词法分析源文件]
    │   ├──symbol_table.h [符号表设计头文件]
    │   ├──symbol_table.cpp [符号表设计源文件]
    │   ├──Syntactic.h [语法/语义分析头文件]
    │   ├──Syntactic.cpp [语法/语义分析源文件]
    │   ├──optimize.h [代码优化头文件]
    │   ├──optimize.cpp [代码优化源文件]
    │   ├──demo.cpp [用于测试的c语言文件，测试代码]
    |	├──compilers.exe [运行程序]
    │   └──main.cpp [主函数]
    └──soruce
    │   ├──DFA.png [词法分析自动机图片]
    │   ├──语法.md [markdown形式的语法]
    │   ├──语法.pdf [pdf形式的语法]
    │   └──LL1分析表.xlsx [LL1分析表]
    └──19335052_何鸿荣_期末报告.pdf [实验报告]
```

#### 运行程序

需要运行compilers.exe文件，输入需要编译的c语言代码的文件路径，会在当前文件夹生成result中的内容，因此需要选定文件，生成的内容中，target.asm是汇编形式的语言。