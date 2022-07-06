#include"optimize.h"

Optimize::Optimize() {

}
Optimize::~Optimize() {
	Quads.clear();
}

void Optimize::optimization(string filename, Symbol_table* Sym) {
	read_quad(filename);
	constant_optimize();
	same_quad_optimize();
	useless_optimize();
	while_optimize();
	deal_temp(Quads,Sym);
	write_quad("../result/optimize.txt");
}

void Optimize::read_quad(string filename) {
	ifstream infile(filename);
	while (true) {
		char l, r;
		string content;
		int token;
		char split;
		int line;
		string oper;
		string arg1;
		string arg2;
		string res;
		infile >> l;
		infile >> oper;
		infile >> arg1;
		infile >> arg2;
		infile >> res;
		res = res.substr(0, res.size() - 1);
		Quad Q(oper, arg1, arg2, res);
		if (infile.fail()) {
			break;
		}
		Quads.push_back(Q);
	}
}
void Optimize::write_quad(string filename) {
	ofstream outfile(filename);
	for (int i = 0; i < Quads.size(); i++) {
		outfile << "(" << Quads[i].op << " " << Quads[i].arg1 << " " << Quads[i].arg2 << " " << Quads[i].res<<")"<<endl;
	}
}
void Optimize::constant_optimize() {
	vector<Quad>::iterator it = Quads.begin();
	while (it != Quads.end()) {
		if (is_two_opertion(it->op)) {
			if (it->arg1[0] != 'S' && it->arg1[0] != 't' && it->arg1[0] != '_' && it->arg2[0] != 'S' && it->arg2[0] != 't' && it->arg2[0] != '_') {
				int ans = 0;
				if (it->op == "+") {
					ans = stoi(it->arg1) + stoi(it->arg2);
				}
				else if (it->op == "-") {
					ans = stoi(it->arg1) - stoi(it->arg2);
				}
				else if (it->op == "*") {
					ans = stoi(it->arg1) * stoi(it->arg2);
				}
				else if (it->op == "/") {
					ans = stoi(it->arg1) / stoi(it->arg2);
				}
				else if (it->op == "&") {
					ans = stoi(it->arg1) & stoi(it->arg2);
				}
				else if (it->op == "|") {
					ans = stoi(it->arg1) | stoi(it->arg2);
				}
				else if (it->op == "^") {
					ans = stoi(it->arg1) ^ stoi(it->arg2);
				}
				else if (it->op == "||") {
					ans = stoi(it->arg1) || stoi(it->arg2);
				}
				else if (it->op == "&&") {
					ans = stoi(it->arg1) && stoi(it->arg2);
				}
				else if (it->op == "<<") {
					ans = stoi(it->arg1) << stoi(it->arg2);
				}
				else if (it->op == ">>") {
					ans = stoi(it->arg1) >> stoi(it->arg2);
				}
				else if (it->op == "<") {
					ans = stoi(it->arg1) < stoi(it->arg2);
				}
				else if (it->op == "<=") {
					ans = stoi(it->arg1) <= stoi(it->arg2);
				}
				else if (it->op == ">") {
					ans = stoi(it->arg1) > stoi(it->arg2);
				}
				else if (it->op == ">=") {
					ans = stoi(it->arg1) >= stoi(it->arg2);
				}
				else if (it->op == "==") {
					ans = stoi(it->arg1) == stoi(it->arg2);
				}
				substitude(it->res, to_string(ans));
				it = Quads.erase(it);
				continue;
			}
		}
		it++;
	}
}
void Optimize::same_quad_optimize() {
	vector<Quad>::iterator it = Quads.begin();
	while (it != Quads.end()) {
		if (!is_two_opertion(it->op)) {
			it++;
			continue;
		}
		for (vector<Quad>::iterator it2 = it + 1; it2 != Quads.end();) {
			if (it2->res[0] == 'S' && it2->res == it->arg1) {
				break;
			}
			if (it2->res[0] == 'S' && it2->res == it->arg2) {
				break;
			}
			if (it2->op == it->op && it2->arg1 == it->arg1 && it2->arg2 == it->arg2 && it2->res[0] != 'S') {
				substitude(it2->res, it->res);
				it2 = Quads.erase(it2);
				continue;
			}
			it2++;
		}
		it++;
	}
}
void Optimize::useless_optimize() {
	vector<Quad>::iterator it = Quads.begin();
	while (it != Quads.end()) {
		bool flag = false;
		if (it->res[0] == 't') {
			flag = true;
		}
		for (vector<Quad>::iterator it2 = it + 1; it2 != Quads.end();) {
			if (it->res == it2->arg1 || it->res == it2->arg2) {
				flag = false;
				break;
			}
			else if (it->res == it2->res) {
				flag = true;
				break;
			}
			it2++;
		}
		if (flag) {
			Quads.erase(it);
			it = Quads.begin();
			continue;
		}
		it++;
	}
}
void Optimize::while_optimize() {
	vector<Quad>::iterator it = Quads.begin();
	while (it != Quads.end()) {
		if (it->op == "WH") {
			it = deal_while(it);
		}
		else it++;
	}
}
vector<Quad>::iterator Optimize::deal_while(vector<Quad>::iterator begin) {
	bool flag = true;
	vector<Quad>::iterator it = begin+1;
	int count = 0;
	while (it != Quads.end() && it->op != "WE") {
		if (it->op == "WH" && flag) {
			deal_while(it);
			flag = false;
			it = begin;
			continue;
		}
		else if (it->op == "WH" && flag == false) {
			count = 1;
			while (count) {
				it++;
				if (it->op == "WH") {
					count += 1;
				}
				else if (it->op == "WE") {
					count -= 1;
				}
			}
			it++;
		}
		count = 0;
		if (it->op == "IF") {
			count += 1;
			while (count) {
				it++;
				if (it->op == "IF") {
					count += 1;
				}
				else if (it->op == "IE") {
					count -= 1;
				}
			}
			it++;
		}
		vector<Quad>::iterator it2 = it+1;
		count = 1;
		if (it->op == "=") {
			while (it2 != Quads.end() && it != it2) {
				if ((it->arg1 == it2->res && it->arg1 != "_") || (it->arg2 == it2->res && it->arg2 != "_") || (it->res == it2->res && it->res != "_")) {
					break;
				}
				if (it2->op == "WH") {
					count++;
				}
				if (it2->op == "WE") {
					count--;
				}
				if (it2->op == "WE" && count == 0) {
					it2 = begin;
				}
				it2++;
			}
			if (it2 == it) {
				Quad a = *it;
				it = Quads.erase(it);
				begin = Quads.insert(begin, a) + 1;
				it = begin + 1;
				continue;
			}
		}
		it++;
	}
	return it;
}
void Optimize::substitude(string args, string content) {
	for (int i = 0; i < Quads.size(); i++) {
		if (Quads[i].arg1 == args) {
			Quads[i].arg1 = content;
		}
		if (Quads[i].arg2 == args) {
			Quads[i].arg2 = content;
		}
	}
}
bool Optimize::is_two_opertion(string op) {
	if (op == "+" || op == "-" || op == "*" || op == "/"
		|| op == "<" || op == ">" || op == ">>" || op == "<<"
		|| op == "==" || op == "^" || op == "&" || op == "|"
		|| op == "||" || op == "&&"
		)
		return true;
	return false;
}
void Optimize::deal_temp(vector<Quad> Quads,Symbol_table *Sym) {
	for (int i = 0; i < Quads.size(); i++) {
		if (Quads[i].arg1[0] == 't') {
			if (Sym->temp.count(Quads[i].arg1)) {
				Sym->temp[Quads[i].arg1] += 1;
			}
			else {
				Sym->temp[Quads[i].arg1] = 1;
			}
		}
		if (Quads[i].arg2[0] == 't') {
			if (Sym->temp.count(Quads[i].arg2)) {
				Sym->temp[Quads[i].arg2] += 1;
			}
			else {
				Sym->temp[Quads[i].arg2] = 1;
			}
		}
		if (Quads[i].res[0] == 't') {
			if (Sym->temp.count(Quads[i].res)) {
				Sym->temp[Quads[i].res] += 1;
			}
			else {
				Sym->temp[Quads[i].res] = 1;
			}
		}
	}
}