#pragma once
#include<vector>
#include<iostream>
#include<map>
#include<unordered_map>
#include<stack>
#include<fstream>
#include<set>
#include"symbol_table.h"
class ASS{
public:
	ofstream outfile;
	ifstream infile;
	vector<int> regs_times;//记录寄存器的上次访问实践
	vector<string> regs;//记录寄存器使用情况
	void solution(string filename, Symbol_table * Sym);
	void gen_data(Symbol_table* Sym);//在.data中将变量声明
	void gen_text(Symbol_table* Sym);//写代码段
	void deal(string oper, string arg1, string arg2, string res, Symbol_table* sym);//处理四元式序列
	void store_data(Symbol_table* sym);//将当前寄存器中的变量的值全部存入地址空间中
	void reg_claer(Symbol_table* sym);//将当前寄存器的值全部清空
	int deal_reg(string arg, Symbol_table* sym, int tag = 0);//为四元式中的参数赋予寄存器。
	ASS();
	~ASS();
	int is = 0, ws = 0;
	stack<int> ie;
	stack<int> we;
	map<string, int> SP;
	int bi = 0;
};