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
	void Init();
	void SLR1_Init();
	vector<string> to_special(string terminal);
	vector<string> SLR1_to_special(string terminal);
	void SLR1_Reduction(int num);
public:
	Syntactic(/* args */);
	~Syntactic();
	void SLR1solution(string file_name);
	void LL1solution(string file_name);
	void print_LL1Grammar();
	void print_stack();
};