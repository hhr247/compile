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
	vector<int> regs_times;//��¼�Ĵ������ϴη���ʵ��
	vector<string> regs;//��¼�Ĵ���ʹ�����
	void solution(string filename, Symbol_table * Sym);
	void gen_data(Symbol_table* Sym);//��.data�н���������
	void gen_text(Symbol_table* Sym);//д�����
	void deal(string oper, string arg1, string arg2, string res, Symbol_table* sym);//������Ԫʽ����
	void store_data(Symbol_table* sym);//����ǰ�Ĵ����еı�����ֵȫ�������ַ�ռ���
	void reg_claer(Symbol_table* sym);//����ǰ�Ĵ�����ֵȫ�����
	int deal_reg(string arg, Symbol_table* sym, int tag = 0);//Ϊ��Ԫʽ�еĲ�������Ĵ�����
	ASS();
	~ASS();
	int is = 0, ws = 0;
	stack<int> ie;
	stack<int> we;
	map<string, int> SP;
	int bi = 0;
};