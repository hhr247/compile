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
	void read_quad(string filename);//�����ʼ��Ԫʽ
	void write_quad(string filename);//д�Ż������Ԫʽ
	void constant_optimize();//�����Ż�
	void same_quad_optimize();//��ͬ�ӱ��ʽ�Ż�
	void useless_optimize();//ɾ�����ø�ֵ
	void while_optimize();//ѭ���Ż�
	void substitude(string args, string content);//�����滻��Ԫʽ�е�ĳЩֵ
	bool is_two_opertion(string op);//�ж��Ƿ����ڶ�Ԫ���������
	void deal_temp(vector<Quad> Quads,Symbol_table* Sym);//ͳ����ʱ������ʹ�ô�����
	vector<Quad>::iterator deal_while(vector<Quad>::iterator begin);

public:
	void optimization(string filename, Symbol_table* Sym);
	Optimize();
	~Optimize();
};