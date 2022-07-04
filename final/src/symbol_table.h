#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<iomanip>
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
	int add_master(string a);
	bool connect_func(string name, string return_type);
	bool connect_variable( string name, string tval, int len);
	int find(string a);
	int findf(string a);
	int findv(string a);
	string getname(string id);
	void print_table();
	vector<func_table> func;
	vector<constant_table> constant;
	vector<variable_table> variable;
	vector<master_table> symbol;
};