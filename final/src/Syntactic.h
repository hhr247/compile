#pragma once
#include<vector>
#include<iostream>
#include<map>
#include<unordered_map>
#include<stack>
#include<fstream>
#include<set>
#include"symbol_table.h"
using namespace std;
class Syntactic
{
private:
	bool is_equal(string, string);//判断两个字符是否相等
	bool is_op(string);//判断是否为操作符
	bool is_va(string);//判断是否为变量
	bool is_type(string);//判断是否为类型
	bool is_legal(string, Symbol_table*);//判断该变量是否合法的被使用(先声明在使用）
	void LL1print_stack();//打印LL1的栈
	void SLR1print_stack();//SLR1的栈
	void Init();//LL1初始化
	void SLR1_Init();//SLR1初始化
	void SLR1_Reduction(int num);//SLR1归约过程
	int temp_count;//以使用的临时变量的计数
	string is_same_type(string a, string b, Symbol_table* sym);//判断两个参数是否属于相同类型
	map<string, map<string, int> > LL1_analize_list;//LL1分析表
	unordered_map<int, string> token2terminal;//将token转换成终结符
	vector<vector<string> > LL1Grammar;//LL1的文法
	set<string> LL1_VT;//终结符集合
	set<string> LL1_VN;//非终结符集合
	stack<string> S;//语法栈
	vector<vector<string> > SLR1_Grammar;//SLR1文法
	map<string, map<string, string> > SLR1_analize_list;//SLR1分析表
	stack<string> SEM;//符号栈
	stack<string> Operator_stack;//操作符栈
	vector<string> temp_type;//临时变量的类型
	vector<string> to_special(string terminal);//使得终结符能够被更多文法的识别。
	vector<string> SLR1_to_special(string terminal);//使得终结符能够被更多文法的识别。
	ofstream stack_output;//LL1栈输出位置
	ofstream quadruple_output;//四元式输出位置
	ofstream action_output;//动作函数输出
	ofstream OPS_output;//符号栈输出
	ofstream SEM_output;//变量栈输出
	vector<string> QT;//记录四元式内容
public:
	Syntactic(/* args */);
	~Syntactic();
	void SLR1solution(string file_name);
	void LL1solution(string file_name, Symbol_table* Symbol);
	void print_LL1Grammar();
	
};