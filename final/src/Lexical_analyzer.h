#pragma once
#include<iostream>
#include<vector>
#include<string>
#include <unordered_map>
#include<fstream>
#include"symbol_table.h"
#define IT_CODE 5
using namespace std;
class Lexical_analyzer {
private:
	//constant table
	vector<string> CT;
	//char table
	vector<string> cT;
	//string table
	vector<string> sT;
	//Keyword Table
	unordered_map<string, int> KT;
	//Partition Table
	unordered_map<string, int> PT;
	//Identify Table
	vector<string> IT;
	int state_change(int state, char ch);//状态转换函数
	int state_to_code(int state_before, char* token, Symbol_table* Symbol);//将状态转换成对应的代号
	void reset(FILE*& fp, char*& token, int& state, int& i);//重置状态并且调整fp指针
	//init keyword table
	void init_kt();
	//init Partition table
	void init_pt();
	bool is_letter(char a);//判断是否为字母
	bool is_dig(char a);//判断是否为数字
public:
	Lexical_analyzer();
	~Lexical_analyzer();
	void Solution(char* fname, Symbol_table* Symbol);
};