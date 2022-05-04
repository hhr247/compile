#include<vector>
#include<iostream>
#include<map>
#include<unordered_map>
#include<stack>
#include<fstream>
#include<set>
#include<string>
#include"Syntactic.h"
using namespace std;


Syntactic::Syntactic(/* args */)
{
	Init();
	SLR1_Init();
}

Syntactic::~Syntactic()
{
}
void Syntactic::print_stack() {
	stack<string> Scopy = S;
	stack<string> Scopy1;
	cout << "Stack: ";
	while (!Scopy.empty()) {
		Scopy1.push(Scopy.top());
		Scopy.pop();
	}
	while (!Scopy1.empty()) {
		cout << Scopy1.top() << ' ';
		Scopy1.pop();
	}
	cout << endl;
}

void Syntactic::LL1solution(string file_name) {
	S.push("#");
	S.push("S");
	ifstream infile(file_name);
	// infile.open(file_name);
	while (true) {
		char l, r;
		string content;
		int token;
		char split;
		int line;
		infile >> l;
		infile >> content;
		infile >> token;
		infile >> split;
		infile >> line;
		infile >> r;
		string next = token2terminal[token];
		if (infile.fail()) {
			// break;
			next = "#";
		}
	label:
		print_stack();
		string S_top = S.top();
		S.pop();
		if (LL1_VT.count(S_top)) {
			if (is_equal(S_top, next)) {
				continue;
			}
			else {
				cout << "error: terminals \"" << S_top << "\" and \"" << next << "\" isn't match." << endl;
				break;
			}
		}
		else if (LL1_VN.count(S_top)) {
			vector<string> special_terminal;
			special_terminal = to_special(next);
			for (int j = 0; j < special_terminal.size(); j++) {
				if (LL1_analize_list[S_top].count(special_terminal[j])) {
					int grammer = LL1_analize_list[S_top][special_terminal[j]];
					if (grammer == 6) {

					}
					for(int i = LL1Grammar[grammer].size() - 1; i >= 0; i--) {
						S.push(LL1Grammar[grammer][i]);
					}
					goto label;
				}
			}
			cout << "Error: \"" << S_top << "\" and \"" << next << "\" can't be find in LL1 analize table." << endl;
			break;	
		}
		else if (next == "#" && S_top == "#") {
			cout << "LL(1) Success!" << endl;
			break;
		}
	}
}

void Syntactic::SLR1solution(string file_name) {
	stack<string> em;
	S = em;
	S.push("#");
	S.push("0");
	ifstream infile(file_name);
	while (true) {
		char l, r;
		string content;
		int token;
		char split;
		int line;
	SLR1_label1:
		infile >> l;
		infile >> content;
		infile >> token;
		infile >> split;
		infile >> line;
		infile >> r;
		string next = token2terminal[token];
		if (infile.fail()) {
			// break;
			next = "#";
		}
	SLR1_label2:
		print_stack();
		string S_top = S.top();
		vector<string> special_terminal;
		special_terminal = SLR1_to_special(next);
		for (int j = 0; j < special_terminal.size(); j++) {
			if (SLR1_analize_list[S_top].count(special_terminal[j])) {
				if (SLR1_analize_list[S_top][special_terminal[j]] == "Ok") {
					cout << "SLR(1) Success!" << endl;
					return;
				}
				else if (SLR1_analize_list[S_top][special_terminal[j]][0] == 'R') {
					string tmp = SLR1_analize_list[S_top][special_terminal[j]].substr(1);
					int Num = std::stoi(tmp);
					SLR1_Reduction(Num);
					goto SLR1_label2;
				}
				else {
					S.push(special_terminal[j]);
					S.push(SLR1_analize_list[S_top][special_terminal[j]]);
					goto SLR1_label1;
				}
			}
		}
		cout << "Error: " << S_top << " and " << next <<" can't be found in SLR1 List";
		break;
	}
}

void Syntactic::print_LL1Grammar() {
	for (int i = 0; i < LL1Grammar.size(); i++) {
		\
			cout << i << ":\t";
		for (int j = 0; j < LL1Grammar[i].size(); j++) {
			cout << LL1Grammar[i][j] << ' ';
		}
		cout << endl;
	}
}

vector<string> Syntactic::to_special(string name) {
	vector<string> re;
	if (name == "++" || name == "--" || name == "!" || name == "~" || name == "+" || name == "-") {
		re.push_back("w0");
		re.push_back("w");
	}
	if (name == "*" || name == "/" || name == "%") {
		re.push_back("w1");
		re.push_back("w");
	}
	if (name == "+" || name == "-") {
		re.push_back("w2");
		re.push_back("w");
	}
	if (name == "<<" || name == ">>") {
		re.push_back("w3");
		re.push_back("w");
	}
	if (name == "<" || name == "<=" || name == ">" || name == ">=") {
		re.push_back("w4");
		re.push_back("w");
	}
	if (name == "==" || name == "!=") {
		re.push_back("w5");
		re.push_back("w");
	}
	if (name == "=" || name == "-=" || name == "+=" || name == "*=" || name == "/=" || name == "%=" || name == "^=" || name == "|=" || name == "&=" || name == "<<=" || name == ">>=") {
		re.push_back("w6");
	}
	if (name == "c" || name == "v" || name == "ch" || name == "s") {
		re.push_back("obj");
	}
	if (name == "||" || name == "&&" || name == "|" || name == "^" || name == "&") {
		re.push_back("w");
	}
	re.push_back(name);
	return re;
}

vector<string> Syntactic::SLR1_to_special(string name) {
	vector<string> re;
	if (name == "++" || name == "--" || name == "!" || name == "~" || name == "+" || name == "-") {
		re.push_back("w0");
	}
	if (name == "*" || name == "/" || name == "%") {
		re.push_back("w");
	}
	if (name == "+" || name == "-") {
		re.push_back("w");
	}
	if (name == "<<" || name == ">>") {
		re.push_back("w");
	}
	if (name == "<" || name == "<=" || name == ">" || name == ">=") {
		re.push_back("w");
	}
	if (name == "==" || name == "!=") {
		re.push_back("w");
	}
	if (name == "=" || name == "-=" || name == "+=" || name == "*=" || name == "/=" || name == "%=" || name == "^=" || name == "|=" || name == "&=" || name == "<<=" || name == ">>=") {
		re.push_back("w6");
	}
	if (name == "c" || name == "v" || name == "ch" || name == "s") {
		re.push_back("obj");
	}
	if (name == "||" || name == "&&" || name == "|" || name == "^" || name == "&") {
		re.push_back("w");
	}
	re.push_back(name);
	return re;
}

void Syntactic::SLR1_Reduction(int num) {
	vector<string> grammer = SLR1_Grammar[num];
	string A = grammer[0];
	for (int i = grammer.size() - 1; i >= 1; i-- ) {
		if (S.top() == grammer[i]) {
			S.pop();
		}
		else {
			cout << "Reduction Error: Stack_top is \"" << S.top() << "\" but grammer needed is \"" << grammer[i] << "\"." << endl;
			return;
		}
	}
	string S_top = S.top();
	if (SLR1_analize_list[S_top].count(A)) {
		S.push(A);
		S.push(SLR1_analize_list[S_top][A]);
	}
	return;
}


bool Syntactic::is_equal(string input, string name) {
	if (input == "w0") {
		if (name == "++" || name == "--" || name == "!" || name == "~" || name == "+" || name == "-") {
			return true;
		}
		else return false;
	}
	if (input == "w1") {
		if (name == "*" || name == "/" || name == "%") {
			return true;
		}
		else return false;
	}
	if (input == "w2") {
		if (name == "+" || name == "-") {
			return true;
		}
		else return false;
	}
	if (input == "w3") {
		if (name == "<<" || name == ">>") {
			return true;
		}
		else return false;
	}
	if (input == "w4") {
		if (name == "<" || name == "<=" || name == ">" || name == ">=") {
			return true;
		}
		else return false;
	}
	if (input == "w5") {
		if (name == "==" || name == "!=") {
			return true;
		}
		else return false;
	}
	if (input == "w6") {
		if (name == "=" || name == "-=" || name == "+=" || name == "*=" || name == "/=" || name == "%=" || name == "^=" || name == "|=" || name == "&=" || name == "<<=" || name == ">>=") {
			return true;
		}
		else return false;
	}
	if (input == "obj") {
		if (name == "c" || name == "v" || name == "ch" || name == "s") {
			return true;
		}
		else return false;
	}
	if (input == "w") {
		if (name == "++" || name == "--" || name == "!" || name == "~" || name == "+" || name == "-") {
			return true;
		}
		if (name == "*" || name == "/" || name == "%") {
			return true;
		}
		if (name == "<<" || name == ">>") {
			return true;
		}
		if (name == "<" || name == "<=" || name == ">" || name == ">=") {
			return true;
		}
		if (name == "==" || name == "!=") {
			return true;
		}
		if (name == "||" || name == "&&" || name == "|" || name == "^" || name == "&") {
			return true;
		}
		return false;
	}
	return name == input;
}

void Syntactic::Init() {
	//文法初始化
	vector<string> tmp;
	//0
	tmp.push_back("E");
	tmp.push_back("S1");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//1
	tmp.push_back("E");
	tmp.push_back("S1");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//2
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//3
	tmp.push_back("AsE");
	tmp.push_back(";");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//4
	tmp.push_back("UnE");
	tmp.push_back("AsE1");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//5
	tmp.push_back("w6");
	tmp.push_back("AsE");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//6
	tmp.push_back("w");
	tmp.push_back("LoE");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//7
	tmp.push_back("LaE");
	tmp.push_back("LoE1");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//8
	tmp.push_back("||");
	tmp.push_back("LaE");
	tmp.push_back("LoE1");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//9
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//10
	tmp.push_back("OrE");
	tmp.push_back("LaE1");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//11
	tmp.push_back("&&");
	tmp.push_back("OrE");
	tmp.push_back("LaE1");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//12
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//13
	tmp.push_back("XoE");
	tmp.push_back("OrE1");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//14
	tmp.push_back("|");
	tmp.push_back("XoE");
	tmp.push_back("OrE1");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//15
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//16
	tmp.push_back("AnE");
	tmp.push_back("XoE1");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//17
	tmp.push_back("^");
	tmp.push_back("AnE");
	tmp.push_back("XoE1");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//18
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//19
	tmp.push_back("EqE");
	tmp.push_back("AnE1");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//20
	tmp.push_back("&");
	tmp.push_back("EqE");
	tmp.push_back("AnE1");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//21
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//22
	tmp.push_back("ReE");
	tmp.push_back("EqE1");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//23
	tmp.push_back("w5");
	tmp.push_back("ReE");
	tmp.push_back("EqE1");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//24
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//25
	tmp.push_back("ShE");
	tmp.push_back("ReE1");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//26
	tmp.push_back("w4");
	tmp.push_back("ShE");
	tmp.push_back("ReE1");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//27
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//28
	tmp.push_back("AdE");
	tmp.push_back("ShE1");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//29
	tmp.push_back("w3");
	tmp.push_back("AdE");
	tmp.push_back("ShE1");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//30
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//31
	tmp.push_back("MuE");
	tmp.push_back("AdE1");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//32
	tmp.push_back("w2");
	tmp.push_back("MuE");
	tmp.push_back("AdE1");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//33
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//34
	tmp.push_back("UnE");
	tmp.push_back("MuE1");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//35
	tmp.push_back("w1");
	tmp.push_back("UnE");
	tmp.push_back("MuE1");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//36
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//37
	tmp.push_back("w0");
	tmp.push_back("UnE");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//38
	tmp.push_back("T");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//39
	tmp.push_back("obj");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//40
	tmp.push_back("(");
	tmp.push_back("AsE");
	tmp.push_back(")");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//41
	tmp.push_back(";");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//42
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//43
	LL1Grammar.push_back(tmp);
	tmp.clear();

	//LL1分析表
	LL1_analize_list["S"]["obj"] = 0;
	LL1_analize_list["S"]["w0"] = 0;
	LL1_analize_list["S"]["("] = 0;
	LL1_analize_list["S"]["#"] = 43;
	LL1_analize_list["S"][";"] = 0;

	LL1_analize_list["S1"]["obj"] = 1;
	LL1_analize_list["S1"]["w0"] = 1;
	LL1_analize_list["S1"]["("] = 1;
	LL1_analize_list["S1"]["#"] = 2;
	LL1_analize_list["S1"][";"] = 1;

	LL1_analize_list["E"]["obj"] = 3;
	LL1_analize_list["E"]["w0"] = 3;
	LL1_analize_list["E"]["("] = 3;
	LL1_analize_list["E"]["#"] = 41;
	LL1_analize_list["E"][";"] = 41;

	LL1_analize_list["AsE"]["obj"] = 4;
	LL1_analize_list["AsE"]["w0"] = 4;
	LL1_analize_list["AsE"]["("] = 4;

	LL1_analize_list["AsE1"]["w6"] = 5;
	LL1_analize_list["AsE1"]["w"] = 6;
	LL1_analize_list["AsE1"][")"] = 42;
	LL1_analize_list["AsE1"]["#"] = 42;
	LL1_analize_list["AsE1"][";"] = 42;

	LL1_analize_list["LoE"]["obj"] = 7;
	LL1_analize_list["LoE"]["w0"] = 7;
	LL1_analize_list["LoE"]["("] = 7;

	LL1_analize_list["LoE1"]["||"] = 8;
	LL1_analize_list["LoE1"][")"] = 9;
	LL1_analize_list["LoE1"][";"] = 9;
	LL1_analize_list["LoE1"]["#"] = 9;

	LL1_analize_list["LaE"]["obj"] = 10;
	LL1_analize_list["LaE"]["w0"] = 10;
	LL1_analize_list["LaE"]["("] = 10;

	LL1_analize_list["LaE1"]["&&"] = 11;
	LL1_analize_list["LaE1"]["||"] = 12;
	LL1_analize_list["LaE1"][")"] = 12;
	LL1_analize_list["LaE1"][";"] = 12;
	LL1_analize_list["LaE1"]["#"] = 12;

	LL1_analize_list["OrE"]["obj"] = 13;
	LL1_analize_list["OrE"]["w0"] = 13;
	LL1_analize_list["OrE"]["("] = 13;

	LL1_analize_list["OrE1"]["|"] = 14;
	LL1_analize_list["OrE1"]["&&"] = 15;
	LL1_analize_list["OrE1"]["||"] = 15;
	LL1_analize_list["OrE1"][")"] = 15;
	LL1_analize_list["OrE1"][";"] = 15;
	LL1_analize_list["OrE1"]["#"] = 15;

	LL1_analize_list["XoE"]["obj"] = 16;
	LL1_analize_list["XoE"]["w0"] = 16;
	LL1_analize_list["XoE"]["("] = 16;

	LL1_analize_list["XoE1"]["^"] = 17;
	LL1_analize_list["XoE1"]["|"] = 18;
	LL1_analize_list["XoE1"]["&&"] = 18;
	LL1_analize_list["XoE1"]["||"] = 18;
	LL1_analize_list["XoE1"][")"] = 18;
	LL1_analize_list["XoE1"][";"] = 18;
	LL1_analize_list["XoE1"]["#"] = 18;

	LL1_analize_list["AnE"]["obj"] = 19;
	LL1_analize_list["AnE"]["w0"] = 19;
	LL1_analize_list["AnE"]["("] = 19;

	LL1_analize_list["AnE1"]["&"] = 20;
	LL1_analize_list["AnE1"]["^"] = 21;
	LL1_analize_list["AnE1"]["|"] = 21;
	LL1_analize_list["AnE1"]["&&"] = 21;
	LL1_analize_list["AnE1"]["||"] = 21;
	LL1_analize_list["AnE1"][")"] = 21;
	LL1_analize_list["AnE1"][";"] = 21;
	LL1_analize_list["AnE1"]["#"] = 21;

	LL1_analize_list["EqE"]["obj"] = 22;
	LL1_analize_list["EqE"]["w0"] = 22;
	LL1_analize_list["EqE"]["("] = 22;

	LL1_analize_list["EqE1"]["w5"] = 23;
	LL1_analize_list["EqE1"]["&"] = 24;
	LL1_analize_list["EqE1"]["^"] = 24;
	LL1_analize_list["EqE1"]["|"] = 24;
	LL1_analize_list["EqE1"]["&&"] = 24;
	LL1_analize_list["EqE1"]["||"] = 24;
	LL1_analize_list["EqE1"][")"] = 24;
	LL1_analize_list["EqE1"][";"] = 24;
	LL1_analize_list["EqE1"]["#"] = 24;

	LL1_analize_list["ReE"]["obj"] = 25;
	LL1_analize_list["ReE"]["w0"] = 25;
	LL1_analize_list["ReE"]["("] = 25;

	LL1_analize_list["ReE1"]["w4"] = 26;
	LL1_analize_list["ReE1"]["w5"] = 27;
	LL1_analize_list["ReE1"]["&"] = 27;
	LL1_analize_list["ReE1"]["^"] = 27;
	LL1_analize_list["ReE1"]["|"] = 27;
	LL1_analize_list["ReE1"]["&&"] = 27;
	LL1_analize_list["ReE1"]["||"] = 27;
	LL1_analize_list["ReE1"][")"] = 27;
	LL1_analize_list["ReE1"][";"] = 27;
	LL1_analize_list["ReE1"]["#"] = 27;

	LL1_analize_list["ShE"]["obj"] = 28;
	LL1_analize_list["ShE"]["w0"] = 28;
	LL1_analize_list["ShE"]["("] = 28;

	LL1_analize_list["ShE1"]["w3"] = 29;
	LL1_analize_list["ShE1"]["w4"] = 30;
	LL1_analize_list["ShE1"]["w5"] = 30;
	LL1_analize_list["ShE1"]["&"] = 30;
	LL1_analize_list["ShE1"]["^"] = 30;
	LL1_analize_list["ShE1"]["|"] = 30;
	LL1_analize_list["ShE1"]["&&"] = 30;
	LL1_analize_list["ShE1"]["||"] = 30;
	LL1_analize_list["ShE1"][")"] = 30;
	LL1_analize_list["ShE1"][";"] = 30;
	LL1_analize_list["ShE1"]["#"] = 30;

	LL1_analize_list["AdE"]["obj"] = 31;
	LL1_analize_list["AdE"]["w0"] = 31;
	LL1_analize_list["AdE"]["("] = 31;

	LL1_analize_list["AdE1"]["w2"] = 32;
	LL1_analize_list["AdE1"]["w3"] = 33;
	LL1_analize_list["AdE1"]["w4"] = 33;
	LL1_analize_list["AdE1"]["w5"] = 33;
	LL1_analize_list["AdE1"]["&"] = 33;
	LL1_analize_list["AdE1"]["^"] = 33;
	LL1_analize_list["AdE1"]["|"] = 33;
	LL1_analize_list["AdE1"]["&&"] = 33;
	LL1_analize_list["AdE1"]["||"] = 33;
	LL1_analize_list["AdE1"][")"] = 33;
	LL1_analize_list["AdE1"][";"] = 33;
	LL1_analize_list["AdE1"]["#"] = 33;

	LL1_analize_list["MuE"]["obj"] = 34;
	LL1_analize_list["MuE"]["w0"] = 34;
	LL1_analize_list["MuE"]["("] = 34;

	LL1_analize_list["MuE1"]["w1"] = 35;
	LL1_analize_list["MuE1"]["w2"] = 36;
	LL1_analize_list["MuE1"]["w3"] = 36;
	LL1_analize_list["MuE1"]["w4"] = 36;
	LL1_analize_list["MuE1"]["w5"] = 36;
	LL1_analize_list["MuE1"]["&"] = 36;
	LL1_analize_list["MuE1"]["^"] = 36;
	LL1_analize_list["MuE1"]["|"] = 36;
	LL1_analize_list["MuE1"]["&&"] = 36;
	LL1_analize_list["MuE1"]["||"] = 36;
	LL1_analize_list["MuE1"][")"] = 36;
	LL1_analize_list["MuE1"][";"] = 36;
	LL1_analize_list["MuE1"]["#"] = 36;

	LL1_analize_list["UnE"]["w0"] = 37;
	LL1_analize_list["UnE"]["obj"] = 38;
	LL1_analize_list["UnE"]["("] = 38;

	LL1_analize_list["T"]["obj"] = 39;
	LL1_analize_list["T"]["("] = 40;

	//token到终结符
	token2terminal[56] = "++";
	token2terminal[59] = "--";
	token2terminal[82] = "!";
	token2terminal[83] = "~";
	token2terminal[55] = "+";
	token2terminal[58] = "-";

	token2terminal[61] = "*";
	token2terminal[63] = "/";
	token2terminal[65] = "%";

	token2terminal[67] = "<<";
	token2terminal[69] = ">>";

	token2terminal[48] = "<";
	token2terminal[49] = "<=";
	token2terminal[50] = ">";
	token2terminal[51] = ">=";

	token2terminal[52] = "==";
	token2terminal[53] = "!=";

	token2terminal[54] = "=";
	token2terminal[60] = "-=";
	token2terminal[57] = "+=";
	token2terminal[62] = "*=";
	token2terminal[64] = "/=";
	token2terminal[66] = "%=";
	token2terminal[68] = "<<=";
	token2terminal[70] = ">>=";
	token2terminal[72] = "&=";
	token2terminal[75] = "|=";
	token2terminal[78] = "^=";

	token2terminal[76] = "||";
	token2terminal[73] = "&&";
	token2terminal[74] = "|";
	token2terminal[77] = "^";
	token2terminal[71] = "&";

	token2terminal[39] = "(";
	token2terminal[40] = ")";

	token2terminal[84] = ";";

	token2terminal[1] = "c";
	token2terminal[2] = "ch";
	token2terminal[3] = "s";
	token2terminal[4] = "v";

	//终结符表
	LL1_VT.insert("obj");
	LL1_VT.insert("w0");
	LL1_VT.insert("w1");
	LL1_VT.insert("w2");
	LL1_VT.insert("w3");
	LL1_VT.insert("w4");
	LL1_VT.insert("w5");
	LL1_VT.insert("w6");
	LL1_VT.insert("w");
	LL1_VT.insert("||");
	LL1_VT.insert("&&");
	LL1_VT.insert("^");
	LL1_VT.insert("|");
	LL1_VT.insert("&");
	LL1_VT.insert(";");
	LL1_VT.insert("(");
	LL1_VT.insert(")");

	//中间符号表
	LL1_VN.insert("S");
	LL1_VN.insert("S1");
	LL1_VN.insert("E");
	LL1_VN.insert("AsE");
	LL1_VN.insert("AsE1");
	LL1_VN.insert("LoE");
	LL1_VN.insert("LoE1");
	LL1_VN.insert("LaE");
	LL1_VN.insert("LaE1");
	LL1_VN.insert("OrE");
	LL1_VN.insert("OrE1");
	LL1_VN.insert("XoE");
	LL1_VN.insert("XoE1");
	LL1_VN.insert("AnE");
	LL1_VN.insert("AnE1");
	LL1_VN.insert("EqE");
	LL1_VN.insert("EqE1");
	LL1_VN.insert("ReE");
	LL1_VN.insert("ReE1");
	LL1_VN.insert("ShE");
	LL1_VN.insert("ShE1");
	LL1_VN.insert("AdE");
	LL1_VN.insert("AdE1");
	LL1_VN.insert("MuE");
	LL1_VN.insert("MuE1");
	LL1_VN.insert("UnE");
	LL1_VN.insert("T");
}

void Syntactic::SLR1_Init() {
	vector<string> tmp;
	//0
	tmp.push_back("S");
	tmp.push_back("S");
	tmp.push_back("1");
	tmp.push_back("E");
	tmp.push_back("2");
	SLR1_Grammar.push_back(tmp);
	tmp.clear();
	//1
	tmp.push_back("S");
	tmp.push_back("E");
	tmp.push_back("3");
	SLR1_Grammar.push_back(tmp);
	tmp.clear();
	//2
	tmp.push_back("E");
	tmp.push_back("AsE");
	tmp.push_back("4");
	tmp.push_back(";");
	tmp.push_back("5");
	SLR1_Grammar.push_back(tmp);
	tmp.clear();
	//3
	tmp.push_back("E");
	tmp.push_back(";");
	tmp.push_back("6");
	SLR1_Grammar.push_back(tmp);
	tmp.clear();
	//4
	tmp.push_back("AsE");
	tmp.push_back("UnE");
	tmp.push_back("7");
	tmp.push_back("AsE1");
	tmp.push_back("8");
	SLR1_Grammar.push_back(tmp);
	tmp.clear();
	//5
	tmp.push_back("AsE1");
	tmp.push_back("w6");
	tmp.push_back("9");
	tmp.push_back("AsE");
	tmp.push_back("10");
	SLR1_Grammar.push_back(tmp);
	tmp.clear();
	//6
	tmp.push_back("AsE1");
	tmp.push_back("w");
	tmp.push_back("11");
	tmp.push_back("OpE");
	tmp.push_back("12");
	SLR1_Grammar.push_back(tmp);
	tmp.clear();
	//7
	tmp.push_back("AsE1");
	SLR1_Grammar.push_back(tmp);
	tmp.clear();
	//8
	tmp.push_back("OpE");
	tmp.push_back("UnE");
	tmp.push_back("13");
	tmp.push_back("OpE1");
	tmp.push_back("14");
	SLR1_Grammar.push_back(tmp);
	tmp.clear();
	//9
	tmp.push_back("OpE1");
	tmp.push_back("w");
	tmp.push_back("15");
	tmp.push_back("UnE");
	tmp.push_back("16");
	tmp.push_back("OpE1");
	tmp.push_back("17");
	SLR1_Grammar.push_back(tmp);
	tmp.clear();
	//10
	tmp.push_back("OpE1");
	SLR1_Grammar.push_back(tmp);
	tmp.clear();
	//11
	tmp.push_back("UnE");
	tmp.push_back("w0");
	tmp.push_back("18");
	tmp.push_back("UnE");
	tmp.push_back("19");
	SLR1_Grammar.push_back(tmp);
	tmp.clear();
	//12
	tmp.push_back("UnE");
	tmp.push_back("T");
	tmp.push_back("20");
	SLR1_Grammar.push_back(tmp);
	tmp.clear();
	//13
	tmp.push_back("T");
	tmp.push_back("obj");
	tmp.push_back("21");
	SLR1_Grammar.push_back(tmp);
	tmp.clear();
	//14
	tmp.push_back("T");
	tmp.push_back("(");
	tmp.push_back("22");
	tmp.push_back("AsE");
	tmp.push_back("23");
	tmp.push_back(")");
	tmp.push_back("24");
	SLR1_Grammar.push_back(tmp);
	tmp.clear();


	SLR1_analize_list["0"]["obj"] = "21";
	SLR1_analize_list["0"]["w0"] = "18";
	SLR1_analize_list["0"]["("] = "22";
	SLR1_analize_list["0"][";"] = "6";
	SLR1_analize_list["0"]["S"] = "1";
	SLR1_analize_list["0"]["E"] = "3";
	SLR1_analize_list["0"]["AsE"] = "4";
	SLR1_analize_list["0"]["UnE"] = "7";
	SLR1_analize_list["0"]["T"] = "20";
	SLR1_analize_list["0"]["#"] = "Ok";

	SLR1_analize_list["1"]["obj"] = "21";
	SLR1_analize_list["1"]["w0"] = "18";
	SLR1_analize_list["1"]["("] = "22";
	SLR1_analize_list["1"][";"] = "6";
	SLR1_analize_list["1"]["E"] = "2";
	SLR1_analize_list["1"]["AsE"] = "4";
	SLR1_analize_list["1"]["UnE"] = "7";
	SLR1_analize_list["1"]["T"] = "20";
	SLR1_analize_list["1"]["#"] = "Ok";

	SLR1_analize_list["2"]["obj"] = "R0";
	SLR1_analize_list["2"]["w"] = "R0";
	SLR1_analize_list["2"]["w0"] = "R0";
	SLR1_analize_list["2"]["w6"] = "R0";
	SLR1_analize_list["2"]["("] = "R0";
	SLR1_analize_list["2"][")"] = "R0";
	SLR1_analize_list["2"][";"] = "R0";
	SLR1_analize_list["2"]["#"] = "R0";

	SLR1_analize_list["3"]["obj"] = "R1";
	SLR1_analize_list["3"]["w"] = "R1";
	SLR1_analize_list["3"]["w0"] = "R1";
	SLR1_analize_list["3"]["w6"] = "R1";
	SLR1_analize_list["3"]["("] = "R1";
	SLR1_analize_list["3"][")"] = "R1";
	SLR1_analize_list["3"][";"] = "R1";
	SLR1_analize_list["3"]["#"] = "R1";

	SLR1_analize_list["4"][";"] = "5";

	SLR1_analize_list["5"]["obj"] = "R2";
	SLR1_analize_list["5"]["w"] = "R2";
	SLR1_analize_list["5"]["w0"] = "R2";
	SLR1_analize_list["5"]["w6"] = "R2";
	SLR1_analize_list["5"]["("] = "R2";
	SLR1_analize_list["5"][")"] = "R2";
	SLR1_analize_list["5"][";"] = "R2";
	SLR1_analize_list["5"]["#"] = "R2";

	SLR1_analize_list["6"]["obj"] = "R3";
	SLR1_analize_list["6"]["w"] = "R3";
	SLR1_analize_list["6"]["w0"] = "R3";
	SLR1_analize_list["6"]["w6"] = "R3";
	SLR1_analize_list["6"]["("] = "R3";
	SLR1_analize_list["6"][")"] = "R3";
	SLR1_analize_list["6"][";"] = "R3";
	SLR1_analize_list["6"]["#"] = "R3";

	SLR1_analize_list["7"]["w6"] = "9";
	SLR1_analize_list["7"][")"] = "R7";
	SLR1_analize_list["7"][";"] = "R7";
	SLR1_analize_list["7"]["AsE1"] = "8";
	SLR1_analize_list["7"]["w"] = "11";

	SLR1_analize_list["8"]["obj"] = "R4";
	SLR1_analize_list["8"]["w"] = "R4";
	SLR1_analize_list["8"]["w0"] = "R4";
	SLR1_analize_list["8"]["w6"] = "R4";
	SLR1_analize_list["8"]["("] = "R4";
	SLR1_analize_list["8"][")"] = "R4";
	SLR1_analize_list["8"][";"] = "R4";
	SLR1_analize_list["8"]["#"] = "R4";

	SLR1_analize_list["9"]["obj"] = "21";
	SLR1_analize_list["9"]["w0"] = "18";
	SLR1_analize_list["9"]["("] = "22";
	SLR1_analize_list["9"]["AsE"] = "10";
	SLR1_analize_list["9"]["UnE"] = "7";
	SLR1_analize_list["9"]["T"] = "20";

	SLR1_analize_list["10"]["obj"] = "R5";
	SLR1_analize_list["10"]["w"] = "R5";
	SLR1_analize_list["10"]["w0"] = "R5";
	SLR1_analize_list["10"]["w6"] = "R5";
	SLR1_analize_list["10"]["("] = "R5";
	SLR1_analize_list["10"][")"] = "R5";
	SLR1_analize_list["10"][";"] = "R5";
	SLR1_analize_list["10"]["#"] = "R5";

	SLR1_analize_list["11"]["obj"] = "21";
	SLR1_analize_list["11"]["w0"] = "18";
	SLR1_analize_list["11"]["("] = "22";
	SLR1_analize_list["11"]["OpE"] = "12";
	SLR1_analize_list["11"]["UnE"] = "13";
	SLR1_analize_list["11"]["T"] = "20";

	SLR1_analize_list["12"]["obj"] = "R6";
	SLR1_analize_list["12"]["w"] = "R6";
	SLR1_analize_list["12"]["w0"] = "R6";
	SLR1_analize_list["12"]["w6"] = "R6";
	SLR1_analize_list["12"]["("] = "R6";
	SLR1_analize_list["12"][")"] = "R6";
	SLR1_analize_list["12"][";"] = "R6";
	SLR1_analize_list["12"]["#"] = "R6";

	SLR1_analize_list["13"]["w"] = "15";
	SLR1_analize_list["13"][")"] = "R10";
	SLR1_analize_list["13"][";"] = "R10";
	SLR1_analize_list["13"]["OpE1"] = "14";

	SLR1_analize_list["14"]["obj"] = "R8";
	SLR1_analize_list["14"]["w"] = "R8";
	SLR1_analize_list["14"]["w0"] = "R8";
	SLR1_analize_list["14"]["w6"] = "R8";
	SLR1_analize_list["14"]["("] = "R8";
	SLR1_analize_list["14"][")"] = "R8";
	SLR1_analize_list["14"][";"] = "R8";
	SLR1_analize_list["14"]["#"] = "R8";

	SLR1_analize_list["15"]["obj"] = "21";
	SLR1_analize_list["15"]["w0"] = "18";
	SLR1_analize_list["15"]["("] = "22";
	SLR1_analize_list["15"]["UnE"] = "16";
	SLR1_analize_list["15"]["T"] = "20";

	SLR1_analize_list["16"]["w"] = "15";
	SLR1_analize_list["16"][")"] = "R10";
	SLR1_analize_list["16"][";"] = "R10";
	SLR1_analize_list["16"]["OpE1"] = "17";

	SLR1_analize_list["17"]["obj"] = "R9";
	SLR1_analize_list["17"]["w"] = "R9";
	SLR1_analize_list["17"]["w0"] = "R9";
	SLR1_analize_list["17"]["w6"] = "R9";
	SLR1_analize_list["17"]["("] = "R9";
	SLR1_analize_list["17"][")"] = "R9";
	SLR1_analize_list["17"][";"] = "R9";
	SLR1_analize_list["17"]["#"] = "R9";

	SLR1_analize_list["18"]["obj"] = "21";
	SLR1_analize_list["18"]["w0"] = "18";
	SLR1_analize_list["18"]["("] = "22";
	SLR1_analize_list["18"]["UnE"] = "19";
	SLR1_analize_list["18"]["T"] = "20";

	SLR1_analize_list["19"]["obj"] = "R11";
	SLR1_analize_list["19"]["w"] = "R11";
	SLR1_analize_list["19"]["w0"] = "R11";
	SLR1_analize_list["19"]["w6"] = "R11";
	SLR1_analize_list["19"]["("] = "R11";
	SLR1_analize_list["19"][")"] = "R11";
	SLR1_analize_list["19"][";"] = "R11";
	SLR1_analize_list["19"]["#"] = "R11";

	SLR1_analize_list["20"]["obj"] = "R12";
	SLR1_analize_list["20"]["w"] = "R12";
	SLR1_analize_list["20"]["w0"] = "R12";
	SLR1_analize_list["20"]["w6"] = "R12";
	SLR1_analize_list["20"]["("] = "R12";
	SLR1_analize_list["20"][")"] = "R12";
	SLR1_analize_list["20"][";"] = "R12";
	SLR1_analize_list["20"]["#"] = "R12";

	SLR1_analize_list["21"]["obj"] = "R13";
	SLR1_analize_list["21"]["w"] = "R13";
	SLR1_analize_list["21"]["w0"] = "R13";
	SLR1_analize_list["21"]["w6"] = "R13";
	SLR1_analize_list["21"]["("] = "R13";
	SLR1_analize_list["21"][")"] = "R13";
	SLR1_analize_list["21"][";"] = "R13";
	SLR1_analize_list["21"]["#"] = "R13";
	
	SLR1_analize_list["22"]["obj"] = "21";
	SLR1_analize_list["22"]["w0"] = "18";
	SLR1_analize_list["22"]["("] = "22";
	SLR1_analize_list["22"]["AsE"] = "23";
	SLR1_analize_list["22"]["UnE"] = "7";
	SLR1_analize_list["22"]["T"] = "20";

	SLR1_analize_list["23"][")"] = "24";

	SLR1_analize_list["24"]["obj"] = "R14";
	SLR1_analize_list["24"]["w"] = "R14";
	SLR1_analize_list["24"]["w0"] = "R14";
	SLR1_analize_list["24"]["w6"] = "R14";
	SLR1_analize_list["24"]["("] = "R14";
	SLR1_analize_list["24"][")"] = "R14";
	SLR1_analize_list["24"][";"] = "R14";
	SLR1_analize_list["24"]["#"] = "R14";
}
