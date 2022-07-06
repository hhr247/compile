#pragma once
#include"symbol_table.h"
#include<stack>
struct Quad {
	string op;
	string arg1;
	string arg2;
	string res;
	Quad(string O, string A1, string A2, string R) :op(O), arg1(A1), arg2(A2), res(R){}
};

class Optimize {
private:
	vector<Quad> Quads;
	void read_quad(string filename);//读入初始四元式
	void write_quad(string filename);//写优化后的四元式
	void constant_optimize();//常数优化
	void same_quad_optimize();//相同子表达式优化
	void useless_optimize();//删除无用赋值
	void while_optimize();//循环优化
	void substitude(string args, string content);//用于替换四元式中的某些值
	bool is_two_opertion(string op);//判断是否属于二元运算符操作
	void deal_temp(vector<Quad> Quads,Symbol_table* Sym);//统计临时变量的使用次数。
	vector<Quad>::iterator deal_while(vector<Quad>::iterator begin);

public:
	void optimization(string filename, Symbol_table* Sym);
	Optimize();
	~Optimize();
};