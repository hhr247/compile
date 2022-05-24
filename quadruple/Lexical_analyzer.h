#pragma once
#include<iostream>
#include<vector>
#include<string>
#include <unordered_map>
#include<fstream>
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
	int state_change(int state, char ch);
	int state_to_code(int state_before, char* token);
	void reset(FILE*& fp, char*& token, int& state, int& i);
	//init keyword table
	void init_kt();
	//init Partition table
	void init_pt();
	bool is_letter(char a);
	bool is_dig(char a);
public:
	Lexical_analyzer();
	~Lexical_analyzer();
	void Solution(char* fname);
};