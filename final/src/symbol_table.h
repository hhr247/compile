#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<iomanip>
#include<map>
using namespace std;
struct func_table {
	string name;
	string return_type;
	func_table(string n, string t = "");
};
struct constant_table {
	int val;
};
struct variable_table {
	string name;
	string tval;
	int len;
	variable_table(string n, string t = "", int l = 0);
};
struct master_table {
	string name;
	string type;
	int address;
	string character;
	master_table(string n, string t = "", int a= -1, string c="");
};

class Symbol_table {
public:
	Symbol_table();
	~Symbol_table();
	int add_master(string a);//在符号表中添加数据
	bool connect_func(string name, string return_type);//将主符号表和函数表中的变量连接起来
	bool connect_variable( string name, string tval, int len);//将主符号表和变量表中的变量连接起来
	int find(string a);//查找a在主符号表的位置
	int findf(string a);//查找a在函数表的位置
	int findv(string a);//查找a在变量表的位置
	string getname(string id);//能够通过在符号表中的位置查找到对应的变量名
	void print_table();//将符号表打印出来
	
	vector<func_table> func;//函数表
	vector<constant_table> constant;//常数表
	vector<variable_table> variable;//变量表
	vector<master_table> symbol;//符号总表
	map<string, int> temp;//临时变量表
};