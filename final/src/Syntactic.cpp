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
void Syntactic::LL1print_stack() {
	stack<string> Scopy = S;
	stack<string> Scopy1;
	stack_output << "Stack: ";
	while (!Scopy.empty()) {
		Scopy1.push(Scopy.top());
		Scopy.pop();
	}
	while (!Scopy1.empty()) {
		stack_output << Scopy1.top() << ' ';
		Scopy1.pop();
	}
	stack_output << endl;

	Scopy = Operator_stack;
	OPS_output << "OPS: ";
	while (!Scopy.empty()) {
		Scopy1.push(Scopy.top());
		Scopy.pop();
	}
	while (!Scopy1.empty()) {
		OPS_output << Scopy1.top() << ' ';
		Scopy1.pop();
	}
	OPS_output << endl;

	Scopy = SEM;
	SEM_output << "SEM: ";
	while (!Scopy.empty()) {
		Scopy1.push(Scopy.top());
		Scopy.pop();
	}
	while (!Scopy1.empty()) {
		SEM_output << Scopy1.top() << ' ';
		Scopy1.pop();
	}
	SEM_output << endl;

}
void Syntactic::SLR1print_stack() {
	stack<string> Scopy = S;
	stack<string> Scopy1;
	stack_output << "Stack: ";
	while (!Scopy.empty()) {
		Scopy1.push(Scopy.top());
		Scopy.pop();
	}
	while (!Scopy1.empty()) {
		stack_output << Scopy1.top() << ' ';
		Scopy1.pop();
	}
	stack_output << endl;
}
void Syntactic::LL1solution(string file_name, Symbol_table* Symbol) {
	S.push("#");
	S.push("P");
	ifstream infile(file_name);
	stack_output.open("LL1-Stack output.txt");
	quadruple_output.open("quadruple-output.txt");
	SEM_output.open("SEM.txt");
	action_output.open("action.txt");
	OPS_output.open("OPS.txt");
	string now_type;
	vector<string> now_var;

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

		content = content.substr(0, content.size() - 1);
	label:
		LL1print_stack();
		string S_top = S.top();
		S.pop();
		action_output << "action: ";
		if (S_top == "GEQ(op1)") {
			if (SEM.empty() || Operator_stack.empty()) {
				cout << "Stack error" << endl;
				break;
			}
			string right = SEM.top();
			SEM.pop();
			string left = SEM.top();
			SEM.pop();
			SEM.push(left);
			if (!is_legal(left, Symbol)) {
				int id = stoi(left.substr(1));
				cout << "the variable"<< Symbol->symbol[id].name<<"isn't defined" << endl;
				break;
			}
			if (!is_legal(right, Symbol)) {
				int id = stoi(right.substr(1));
				cout << "the variable" << Symbol->symbol[id].name << "isn't defined" << endl;
				break;
			}
			string type = is_same_type(left, right, Symbol);
			if (type == "error") {
				cout << "Error: Different type can't be calculate." << endl;
				break;
			}
			string op = Operator_stack.top();
			Operator_stack.pop();
			string q = '(' + op + ' ' + right + ' ' + "_" + ' ' + left + ")";
			quadruple_output << q << endl;
			QT.push_back(q);
			action_output << "GEQ(op1)" << endl;
			goto label;
		}
		else if (S_top == "GEQ(op2)") {
			if (SEM.empty() || Operator_stack.empty()) {
				cout << "Stack error" << endl;
				break;
			}
			string right = SEM.top();
			SEM.pop();
			string left = SEM.top();
			SEM.pop();
			if (!is_legal(left, Symbol)) {
				int id = stoi(left.substr(1));
				cout << "the variable" << Symbol->symbol[id].name << "isn't defined" << endl;
				break;
			}
			if (!is_legal(right, Symbol)) {
				int id = stoi(right.substr(1));
				cout << "the variable" << Symbol->symbol[id].name << "isn't defined" << endl;
				break;
			}
			string type = is_same_type(left, right, Symbol);
			if (type == "error") {
				cout << "Error: Different type can't be calculate." << endl;
				break;
			}
			string name = "t" + to_string(temp_count);
			temp_count++;
			SEM.push(name);
			string op = Operator_stack.top();
			Operator_stack.pop();
			string q = '(' + op + ' ' + left + ' ' + right + ' ' + name + ")";
			quadruple_output << q << endl;
			QT.push_back(q);
			temp_type.push_back(type);
			action_output << "GEQ(op2)" << endl;
			goto label;
		}
		else if (S_top == "GEQ(op0)") {
			if (SEM.empty() || Operator_stack.empty()) {
				cout << "Stack error" << endl;
				break;
			}
			string right = SEM.top();
			SEM.pop();
			if (!is_legal(right, Symbol)) {
				int id = stoi(right.substr(1));
				cout << "the variable" << Symbol->symbol[id].name << "isn't defined" << endl;
				break;
			}
			string op = Operator_stack.top();
			Operator_stack.pop();
			string name;
			if (op == "++" || op == "--") {
				name = right;
			}
			else {
				name = "t" + to_string(temp_count);
				temp_count++;
				string type = is_same_type(right, right, Symbol);
				temp_type.push_back(type);
			}
			SEM.push(name);
			action_output << "GEQ(op0)" << endl;
			string q = '(' + op + ' ' + right + ' ' + "_" + ' ' + name + ")";
			quadruple_output << q << endl;
			QT.push_back(q);
			goto label;
		}
		else if (S_top == "{IDF()}") {
			
			string id = SEM.top();
			SEM.pop();
			string name = Symbol->getname(id);
			if (!Symbol->connect_func(name, now_type)) {
				cout << "Error: \"" << name << "\" can't be redefined." << endl;
				break;
			}
			goto label;
		}
		else if (S_top == "{IDV()}") {
			string id = SEM.top();
			string name = Symbol->getname(id);
			int len = 0;
			if (now_type == "int") len = 4;
			if (now_type == "float") len = 4;
			if (now_type == "char") len = 1;
			if (!Symbol->connect_variable(name, now_type, len)) {
				cout << "Error: \"" << name << "\" can't be redefined." << endl;
				break;
			}
			goto label;
		}
		else if (S_top == "{IF()}") {
			string id = SEM.top();
			action_output << "IF()" << endl;
			string q = "(IF " + id + " _" + " _)";
			quadruple_output << q << endl;
			stack<string> em;
			Operator_stack = em;
			SEM = em;
			goto label;
		}
		else if (S_top == "{EL()}") {
			action_output << "EL()" << endl;
			string q = "(EL _ _ _)";
			quadruple_output << q << endl;
			stack<string> em;
			Operator_stack = em;
			SEM = em;
			goto label;
		}
		else if (S_top == "{IE()}") {
			action_output << "IE()" << endl;
			string q = "(IE _ _ _)";
			quadruple_output << q << endl;
			stack<string> em;
			Operator_stack = em;
			SEM = em;
			goto label;
		}
		else if (S_top == "{WH()}") {
			action_output << "WH()" << endl;
			string q = "(WH _ _ _)";
			quadruple_output << q << endl;
			stack<string> em;
			Operator_stack = em;
			SEM = em;
			goto label;
		}
		else if (S_top == "{DO()}") {
			string id = SEM.top();
			action_output << "DO()" << endl;
			string q = "(DO "+id+"  _ _)";
			quadruple_output << q << endl;
			stack<string> em;
			Operator_stack = em;
			SEM = em;
			goto label;
		}
		else if (S_top == "{WE()}") {
			action_output << "WE()" << endl;
			string q = "(WE _ _ _)";
			quadruple_output << q << endl;
			stack<string> em;
			Operator_stack = em;
			SEM = em;
			goto label;
		}
		if (LL1_VT.count(S_top)) {
			if (is_equal(S_top, next)) {
				if (is_op(next)) {
					Operator_stack.push(next);
				}
				if (is_va(next)) {
					if (next == "v") {
						content = "S" + content;
					}
					SEM.push(content);
				}
				if (next == ";") {
					stack<string> em;
					Operator_stack = em;
					SEM = em;
				}
				if (is_type(next)) {
					now_type = next;
				}
				action_output << "next(w)" << endl;
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
					for(int i = LL1Grammar[grammer].size() - 1; i >= 0; i--) {
						S.push(LL1Grammar[grammer][i]);
					}
					action_output << endl;
					goto label;
				}
			}
			cout << "Error: \"" << S_top << "\" and \"" << next << "\" can't be find in LL1 analize table." << endl;
			break;	
		}
		else if (next == "#" && S_top == "#") {
			cout << "LL(1) Success!" << endl;
			action_output << "OK" << endl;
			break;
		}
	}
	stack_output.close();
	quadruple_output.close();
	infile.close();
	OPS_output.close();
	action_output.close();
	SEM_output.close();
}
bool Syntactic::is_op(string next) {
	if (next == "++" || next == "--" || next == "!" || next == "~" || next == "+" || next == "-") {
		return true;
	}
	if (next == "*" || next == "/" || next == "%" ) {
		return true;
	}
	if (next == "<<" || next == ">>") {
		return true;
	}
	if (next == "<" || next == "<=" || next == ">" || next == ">=") {
		return true;
	}
	if (next == "==" || next == "!=") {
		return true;
	}
	if (next == "=" || next == "-=" || next == "+=" || next == "*=" || next == "/=" || next == "%=" || next == "&=" || next == "|=" || next == "^=" || next == "<<=" || next == ">>=") {
		return true;
	}
	if (next == "||" || next == "&&" || next == "|" || next == "^" || next == "&" || next == "^") {
		return true;
	}
	return false;
}
bool Syntactic::is_va(string next) {
	if (next == "c" || next == "v" || next == "s") {
		return true;
	}
	return false;
}
bool Syntactic::is_type(string next) {
	if (next == "int" || next == "float" || next == "char") {
		return true;
	}
	return false;
}
void Syntactic::SLR1solution(string file_name) {
	stack<string> em;
	S = em;
	S.push("#");
	S.push("0");
	ifstream infile(file_name);
	stack_output.open("SLR1-Stack output.txt");
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
		SLR1print_stack();
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
	infile.close();
	stack_output.close();
}

void Syntactic::print_LL1Grammar() {
	for (int i = 0; i < LL1Grammar.size(); i++) {
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
	if (name == "int" || name == "void" || name == "float" || name == "char") {
		re.push_back("type");
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
	if (input == "type") {
		if (name == "int" || name == "void" || name == "float" || name == "char") {
			return true;
		}
		return false;
	}
	return name == input;
}

string Syntactic::is_same_type(string a, string b, Symbol_table* sym) {
	string type1, type2;
	if (a[0] == 'S') {
		string id = a.substr(1);
		int i = stoi(id);
		type1 = sym->variable[sym->symbol[i].address].tval;
	}
	else if (a[0] == 't') {
		string id = a.substr(1);
		int i = stoi(id);
		type1 = temp_type[i];
	}
	else if (a[0] == '\'') {
		type1 = "char";
	}
	else if (a.find('.') != a.npos) {
		type1 = "float";
	}
	else {
		type1 = "int";
	}
	if (b[0] == 'S') {
		string id = b.substr(1);
		int i = stoi(id);
		type2 = sym->variable[sym->symbol[i].address].tval;
	}
	else if (b[0] == 't') {
		string id = b.substr(1);
		int i = stoi(id);
		type2 = temp_type[i];
	}
	else if (b.find('.') != b.npos) {
		type2 = "float";
	}
	else if (b[0] == '\'') {
		type2 = "char";
	}
	else {
		type2 = "int";
	}
	if (type1 == type2) {
		return type1;
	}
	else return "error";
}

bool Syntactic::is_legal(string arg, Symbol_table* sym) {
	if (arg[0] == 'S') {
		string sname = arg.substr(1);
		int i = stoi(sname);
		if (sym->symbol[i].type != "v") {
			return false;
		}
	}
	return true;
}

void Syntactic::Init() {
	temp_count = 0;
	//文法初始化
	vector<string> tmp;
	//0
	tmp.push_back("S");
	tmp.push_back("Ss");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//1
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//2
	tmp.push_back("AsE");
	tmp.push_back(";");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//3
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//4
	tmp.push_back("LoE");
	tmp.push_back("AsE1");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//5
	tmp.push_back("w6");
	tmp.push_back("AsE");
	tmp.push_back("GEQ(op1)");
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
	tmp.push_back("GEQ(op2)");
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
	tmp.push_back("GEQ(op2)");
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
	tmp.push_back("GEQ(op2)");
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
	tmp.push_back("GEQ(op2)");
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
	tmp.push_back("GEQ(op2)");
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
	tmp.push_back("GEQ(op2)");
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
	tmp.push_back("GEQ(op2)");
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
	tmp.push_back("GEQ(op2)");
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
	tmp.push_back("GEQ(op2)");
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
	tmp.push_back("GEQ(op2)");
	tmp.push_back("MuE1");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//36
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//37
	tmp.push_back("w0");
	tmp.push_back("UnE");
	tmp.push_back("GEQ(op0)");
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
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//42
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//43
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//44
	tmp.push_back("IFS");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//45
	tmp.push_back("Declaration");
	tmp.push_back(";");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//46
	tmp.push_back("Wh");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//47
	tmp.push_back(";");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//48
	tmp.push_back("CS");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//49
	tmp.push_back("if");
	tmp.push_back("(");
	tmp.push_back("AsE");
	tmp.push_back(")");
	tmp.push_back("{IF()}");
	tmp.push_back("S");
	tmp.push_back("{EL()}");
	tmp.push_back("IFS1");
	tmp.push_back("{IE()}");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//50
	tmp.push_back("else");
	tmp.push_back("S");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//51
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//52
	tmp.push_back("type");
	tmp.push_back("VL");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//53
	tmp.push_back("v");
	tmp.push_back("{IDV()}");
	tmp.push_back("VL1");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//54
	tmp.push_back("Declaration1");
	tmp.push_back("VL1");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//55
	tmp.push_back(",");
	tmp.push_back("v");
	tmp.push_back("{IDV()}");
	tmp.push_back("VL1");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//56
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//57
	tmp.push_back("while");
	tmp.push_back("{WH()}");
	tmp.push_back("(");
	tmp.push_back("AsE");
	tmp.push_back(")");
	tmp.push_back("{DO()}");
	tmp.push_back("S");
	tmp.push_back("{WE()}");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//58
	tmp.push_back("ExD");
	tmp.push_back("P1");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//59
	tmp.push_back("ExD");
	tmp.push_back("P1");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//60
	tmp.push_back("type");
	tmp.push_back("v");
	tmp.push_back("ExD1");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//61
	tmp.push_back("Func1");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//62
	tmp.push_back("{IDV()}");
	tmp.push_back("Declaration1");
	tmp.push_back(";");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//63
	tmp.push_back("(");
	tmp.push_back(")");
	tmp.push_back("{IDF()}");
	tmp.push_back("CS");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//64
	tmp.push_back("w6");
	tmp.push_back("AsE");
	tmp.push_back("GEQ(op1)");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//65
	tmp.push_back("{");
	tmp.push_back("CS1");
	tmp.push_back("}");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//66
	tmp.push_back("Ss");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//67
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//68
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//69
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//70
	LL1Grammar.push_back(tmp);
	tmp.clear();



	//LL1分析表
	LL1_analize_list["P"]["type"] = 58;
	LL1_analize_list["P"]["#"] = 67;

	LL1_analize_list["P1"]["type"] = 59;
	LL1_analize_list["P1"]["#"] = 68;

	LL1_analize_list["ExD"]["type"] = 60;
	
	LL1_analize_list["ExD1"]["w6"] = 62;
	LL1_analize_list["ExD1"][";"] = 62;
	LL1_analize_list["ExD1"]["("] = 61;

	LL1_analize_list["ExD1"]["w6"] = 62;
	LL1_analize_list["ExD1"][";"] = 62;
	LL1_analize_list["ExD1"]["("] = 61;

	LL1_analize_list["Func1"]["("] = 63;

	LL1_analize_list["Declaration1"]["w6"] = 64;
	LL1_analize_list["Declaration1"][";"] = 69;
	LL1_analize_list["Declaration1"][","] = 69;

	LL1_analize_list["CS"]["{"] = 65;

	LL1_analize_list["CS1"]["type"] = 66;
	LL1_analize_list["CS1"]["obj"] = 66;
	LL1_analize_list["CS1"]["w0"] = 66;
	LL1_analize_list["CS1"]["("] = 66;
	LL1_analize_list["CS1"]["if"] = 66;
	LL1_analize_list["CS1"]["while"] = 66;
	LL1_analize_list["CS1"]["{"] = 66;
	LL1_analize_list["CS1"]["}"] = 70;

	LL1_analize_list["Ss"]["type"] = 0;
	LL1_analize_list["Ss"]["obj"] = 0;
	LL1_analize_list["Ss"]["w0"] = 0;
	LL1_analize_list["Ss"]["("] = 0;
	LL1_analize_list["Ss"]["if"] = 0;
	LL1_analize_list["Ss"]["while"] = 0;
	LL1_analize_list["Ss"]["{"] = 0;
	LL1_analize_list["Ss"]["}"] = 41;

	LL1_analize_list["S"]["type"] = 45;
	LL1_analize_list["S"]["v"] = 2;
	LL1_analize_list["S"]["s"] = 2;
	LL1_analize_list["S"]["c"] = 2;
	LL1_analize_list["S"]["w0"] = 2;
	LL1_analize_list["S"]["("] = 2;
	LL1_analize_list["S"][";"] = 47;
	LL1_analize_list["S"]["if"] = 44;
	LL1_analize_list["S"]["while"] = 46;
	LL1_analize_list["S"]["{"] = 48;


	LL1_analize_list["AsE"]["obj"] = 4;
	LL1_analize_list["AsE"]["w0"] = 4;
	LL1_analize_list["AsE"]["("] = 4;

	LL1_analize_list["AsE1"]["w6"] = 5;
	LL1_analize_list["AsE1"][")"] = 42;
	LL1_analize_list["AsE1"][","] = 42;
	LL1_analize_list["AsE1"][";"] = 42;

	LL1_analize_list["LoE"]["obj"] = 7;
	LL1_analize_list["LoE"]["w0"] = 7;
	LL1_analize_list["LoE"]["("] = 7;

	LL1_analize_list["LoE1"]["||"] = 8;
	LL1_analize_list["LoE1"][")"] = 9;
	LL1_analize_list["LoE1"][";"] = 9;
	LL1_analize_list["LoE1"]["#"] = 9;
	LL1_analize_list["LoE1"][","] = 9;
	LL1_analize_list["LoE1"]["w6"] = 9;

	LL1_analize_list["LaE"]["obj"] = 10;
	LL1_analize_list["LaE"]["w0"] = 10;
	LL1_analize_list["LaE"]["("] = 10;

	LL1_analize_list["LaE1"]["&&"] = 11;
	LL1_analize_list["LaE1"]["||"] = 12;
	LL1_analize_list["LaE1"][")"] = 12;
	LL1_analize_list["LaE1"][";"] = 12;
	LL1_analize_list["LaE1"]["#"] = 12;
	LL1_analize_list["LaE1"]["w6"] = 12;
	LL1_analize_list["LaE1"][","] = 12;

	LL1_analize_list["OrE"]["obj"] = 13;
	LL1_analize_list["OrE"]["w0"] = 13;
	LL1_analize_list["OrE"]["("] = 13;

	LL1_analize_list["OrE1"]["|"] = 14;
	LL1_analize_list["OrE1"]["&&"] = 15;
	LL1_analize_list["OrE1"]["||"] = 15;
	LL1_analize_list["OrE1"][")"] = 15;
	LL1_analize_list["OrE1"][";"] = 15;
	LL1_analize_list["OrE1"]["#"] = 15;
	LL1_analize_list["OrE1"][","] = 15;
	LL1_analize_list["OrE1"]["w6"] = 15;

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
	LL1_analize_list["XoE1"][","] = 18;
	LL1_analize_list["XoE1"]["w6"] = 18;

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
	LL1_analize_list["AnE1"][","] = 21;
	LL1_analize_list["AnE1"]["w6"] = 21;

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
	LL1_analize_list["EqE1"][","] = 24;
	LL1_analize_list["EqE1"]["w6"] = 24;

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
	LL1_analize_list["ReE1"][","] = 27;
	LL1_analize_list["ReE1"]["w6"] = 27;

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
	LL1_analize_list["ShE1"][","] = 30;
	LL1_analize_list["ShE1"]["w6"] = 30;

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
	LL1_analize_list["AdE1"][","] = 33;
	LL1_analize_list["AdE1"]["w6"] = 33;

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
	LL1_analize_list["MuE1"][","] = 36;
	LL1_analize_list["MuE1"]["w6"] = 36;

	LL1_analize_list["UnE"]["w0"] = 37;
	LL1_analize_list["UnE"]["obj"] = 38;
	LL1_analize_list["UnE"]["("] = 38;

	LL1_analize_list["T"]["obj"] = 39;
	LL1_analize_list["T"]["("] = 40;

	LL1_analize_list["IFS"]["if"] = 49;

	LL1_analize_list["IFS1"]["type"] = 51;
	LL1_analize_list["IFS1"]["v"] = 51;
	LL1_analize_list["IFS1"]["s"] = 51;
	LL1_analize_list["IFS1"]["c"] = 51;
	LL1_analize_list["IFS1"]["w0"] = 51;
	LL1_analize_list["IFS1"]["("] = 51;
	LL1_analize_list["IFS1"][";"] = 51;
	LL1_analize_list["IFS1"]["if"] = 51;
	LL1_analize_list["IFS1"]["while"] = 51;
	LL1_analize_list["IFS1"]["{"] = 51;
	LL1_analize_list["IFS1"]["}"] = 51;
	LL1_analize_list["IFS1"]["else"] = 50;

	LL1_analize_list["Declaration"]["type"] = 52;

	LL1_analize_list["VL"]["v"] = 53;

	LL1_analize_list["VL1"]["w6"] = 54;
	LL1_analize_list["VL1"][";"] = 56;
	LL1_analize_list["VL1"][","] = 55;

	LL1_analize_list["Wh"]["while"] = 57;

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
	token2terminal[2] = "s";
	token2terminal[3] = "s";
	token2terminal[4] = "v";

	token2terminal[23] = "void";
	token2terminal[7] = "int";
	token2terminal[9] = "float";
	token2terminal[11] = "char";

	token2terminal[24] = "if";
	token2terminal[25] = "else";
	token2terminal[30] = "while";
	token2terminal[79] = ",";

	token2terminal[37] = "{";
	token2terminal[38] = "}";

	//终结符表
	LL1_VT.insert("obj");
	LL1_VT.insert("type");
	LL1_VT.insert("v");
	LL1_VT.insert("s");
	LL1_VT.insert("c");
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
	LL1_VT.insert("if");
	LL1_VT.insert("else");
	LL1_VT.insert("while");
	LL1_VT.insert("{");
	LL1_VT.insert("}");
	LL1_VT.insert(",");

	//中间符号表
	LL1_VN.insert("P");
	LL1_VN.insert("P1");
	LL1_VN.insert("ExD");
	LL1_VN.insert("ExD1");
	LL1_VN.insert("Func1");
	LL1_VN.insert("Declaration1");
	LL1_VN.insert("CS");
	LL1_VN.insert("CS1");
	LL1_VN.insert("Ss");
	LL1_VN.insert("S");
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
	LL1_VN.insert("IFS");
	LL1_VN.insert("IFS1");
	LL1_VN.insert("Declaration");
	LL1_VN.insert("VL");
	LL1_VN.insert("VL1");
	LL1_VN.insert("Wh");
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
