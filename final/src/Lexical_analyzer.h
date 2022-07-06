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
	int state_change(int state, char ch);//״̬ת������
	int state_to_code(int state_before, char* token, Symbol_table* Symbol);//��״̬ת���ɶ�Ӧ�Ĵ���
	void reset(FILE*& fp, char*& token, int& state, int& i);//����״̬���ҵ���fpָ��
	//init keyword table
	void init_kt();
	//init Partition table
	void init_pt();
	bool is_letter(char a);//�ж��Ƿ�Ϊ��ĸ
	bool is_dig(char a);//�ж��Ƿ�Ϊ����
public:
	Lexical_analyzer();
	~Lexical_analyzer();
	void Solution(char* fname, Symbol_table* Symbol);
};