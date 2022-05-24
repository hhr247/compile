#pragma once
#include<vector>
#include<iostream>
#include<map>
#include<unordered_map>
#include<stack>
#include<fstream>
#include<set>
using namespace std;
class Syntactic
{
private:
	map<string, map<string, int> > LL1_analize_list;
	unordered_map<int, string> token2terminal;
	vector<vector<string> > LL1Grammar;
	bool is_equal(string, string);
	set<string> LL1_VT;
	set<string> LL1_VN;
	stack<string> S;
	vector<vector<string> > SLR1_Grammar;
	map<string, map<string, string> > SLR1_analize_list;
	stack<string> SEM;
	stack<string> Operator_stack;
	void Init();
	void SLR1_Init();
	vector<string> to_special(string terminal);
	vector<string> SLR1_to_special(string terminal);
	void SLR1_Reduction(int num);
	int temp_count;
	bool is_op(string);
	bool is_va(string);
	ofstream stack_output;
	ofstream quadruple_output;
	ofstream action_output;
	ofstream OPS_output;
	ofstream SEM_output;
	vector<string> QT;
	void LL1print_stack();
	void SLR1print_stack();
public:
	Syntactic(/* args */);
	~Syntactic();
	void SLR1solution(string file_name);
	void LL1solution(string file_name);
	void print_LL1Grammar();
	
};