#include "ASS.h"

ASS::ASS() {
	outfile.open("target.asm");
	for (int i = 0; i < 8; i++) {
		regs_times.push_back(8);
		regs.push_back("empty");
	}
}
ASS::~ASS() {
	outfile.close();
}
void ASS::gen_data(Symbol_table* Sym) {
	outfile << ".data" << endl;
	for (int i = 0; i < Sym->variable.size(); i++) {
		outfile << Sym->variable[i].name << ":\t.space\t" << Sym->variable[i].len << endl;
	}
	outfile << ".text" << endl;
	for (int i = 0; i < Sym->func.size(); i++) {
		outfile << Sym->func[i].name <<":"<< endl;
	}
}

void ASS::solution(string filename, Symbol_table* Sym) {
	infile.open(filename);
	gen_data(Sym);
	gen_text(Sym);
	infile.close();
	cout << "Assemble generate success!" << endl;
	store_data(Sym);
}

void ASS::store_data(Symbol_table* sym) {
	for (int i = 0; i < 8; i++) {
		if (regs[i][0] == 'S') {
			string Sname = regs[i].substr(1);
			int j = sym->findv(Sname);
			int len = sym->variable[j].len;
			if (len == 4) {
				outfile << "\t" << "sw" << "\t" << "$t" << i << "," << Sname << endl;
			}
			if (len == 1) {
				outfile << "\t" << "sb" << "\t" << "$t" << i << "," << Sname << endl;
			}
		}
	}
}

void ASS::gen_text(Symbol_table* Sym) {
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
		if (infile.fail()) {
			break;
		}
		string targ1;
		string targ2;
		if (arg1 != "_" && arg1[0] != 'S' && arg1[0] != 't')
			targ1 = "$s" + to_string(deal_reg(arg1, Sym));
		else
			targ1 = "$t" + to_string(deal_reg(arg1, Sym));

		if (arg2 != "_" && arg2[0] != 'S' && arg2[0] != 't')
			targ2 = "$s" + to_string(deal_reg(arg2, Sym, 1));
		else
			targ2 = "$t" + to_string(deal_reg(arg2, Sym));

		string tres = "$t" + to_string(deal_reg(res, Sym));
		deal(oper, targ1, targ2, tres, Sym);
	}
}
int ASS::deal_reg(string arg, Symbol_table* sym, int tag) {
	if (arg[0] == 'S') {
		string id = arg.substr(1);
		string name = "S" + sym->getname(arg);
		int i = 0;
		int idx = -1;
		for (i = 0; i < 8; i++) {
			if (name == regs[i]) {
				idx = i;
				regs_times[i] = 0;
			}
			regs_times[i]++;
		}
		if (idx != -1)
			return idx;
		int max = -1;
		regs_times[0] += 1;
		for (int i = 0; i < 8; i++) {
			if (regs_times[i] > max) {
				max = regs_times[i];
				idx = i;
			}
			regs_times[i] += 1;
		}
		if (regs[idx][0] == 'S') {
			string Sname = regs[idx].substr(1);
			int j = sym->findv(Sname);
			int len = sym->variable[j].len;
			if (len == 4) {
				outfile << "\t" << "sw" << "\t" << "$t" << idx << "," << Sname << endl;
			}
			if (len == 1) {
				outfile << "\t" << "sb" << "\t" << "$t" << idx << "," << Sname << endl;
			}
		}
		regs[idx] = name;
		regs_times[idx] = 0;
		string Sname = regs[idx].substr(1);
		int j = sym->findv(Sname);
		int len = sym->variable[j].len;
		if (len == 4) {
			outfile << "\t" << "lw" << "\t" << "$t" << idx << "," << Sname << endl;
		}
		if (len == 1) {
			outfile << "\t" << "lb" << "\t" << "$t" << idx << "," << Sname << endl;
		}
		return idx;
	}
	if (arg[0] == 't') {
		int i = 0;
		int idx = -1;
		for (i = 0; i < 8; i++) {
			regs_times[i] += 1;
			if (arg == regs[i]) {
				idx = i;
				regs_times[i] = 0;
			}
		}
		if(idx != -1)
			return idx;
		idx = -1;
		int max = -1;
		for (int i = 0; i < 8; i++) {
			if (regs_times[i] > max) {
				max = regs_times[i];
				idx = i;
			}
		}
		if (regs[idx][0] == 'S') {
			string Sname = regs[idx].substr(1);
			int j = sym->findv(Sname);
			int len = sym->variable[j].len;
			if (len == 4) {
				outfile << "\t" << "sw" << "\t" << "$t" << idx << "," << Sname << endl;
			}
			if (len == 1) {
				outfile << "\t" << "sb" << "\t" << "$t" << idx << "," << Sname << endl;
			}
		}
		regs[idx] = arg;
		regs_times[idx] = 0;
		return idx;
	}
	else if(arg[0] != '_'){
		outfile << "\t" << "ori" << "\t" << "$s"<<tag<<",$0," << arg << endl;
		return tag;
	}
	return -1;
}
void ASS::deal(string oper,string arg1,string arg2,string res, Symbol_table* sym) {
	if (oper == "++") {
		outfile << "\t" << "addi" << "\t" <<res << ',' << arg1 << "," << 1 << endl;
	}
	if (oper == "--") {
		outfile << "\t" << "subi" << "\t" << res << ',' << arg1 << "," << 1 << endl;
	}
	if (oper == "*") {
		outfile << "\t" << "mul" << "\t" << res << ',' << arg1 << "," << arg2 << endl;
	}
	if (oper == "/") {
		outfile << "\t" << "div.d" << "\t" << res << ',' << arg1 << "," << arg2 << endl;
	}
	if (oper == "+") {
		if (arg2 == "$t-1")
			outfile << "\t" << "add" << "\t" << res << ',' << arg1 << "," << "$0" << endl;
		else
			outfile << "\t" << "add" << "\t" << res << ',' << arg1 << "," << arg2 << endl;
	}
	if (oper == "-") {
		if (arg2 == "$t-1") 
			outfile << "\t" << "sub" << "\t" << res << ',' << "$0" << "," <<  arg1 << endl;
		else 
			outfile << "\t" << "sub" << "\t" << res << ',' << arg1 << "," << arg2 << endl;
	}
	if (oper == "<<") {
		outfile << "\t" << "sllv" << "\t" << res << ',' << arg1 << "," << arg2 << endl;
	}
	if (oper == ">>") {
		outfile << "\t" << "srlv" << "\t" << res << ',' << arg1 << "," << arg2 << endl;
	}
	if (oper == "<") {
		outfile << "\t" << "blt" << "\t" << arg1 << "," << arg2 <<",b"<<bi<<0<< endl;
		outfile << "\t" << "ori" << "\t" << res << ",$0,0" << endl;
		outfile << "\t" << "b" << "\tb" << bi << 1 << endl;
		outfile << "b" << bi << 0<<":" << endl;
		outfile << "\t" << "ori" << "\t" << res << ",$0,1" << endl;
		outfile << "b" << bi << 1 << ":" << endl;
		bi++;
	}
	if (oper == "<=") {
		outfile << "\t" << "ble" << "\t" << arg1 << "," << arg2 << ",b" << bi << 0 << endl;
		outfile << "\t" << "ori" << "\t" << res << ",$0,0" << endl;
		outfile << "\t" << "b" << "\tb" << bi << 1 << endl;
		outfile << "b" << bi << 0 << ":" << endl;
		outfile << "\t" << "ori" << "\t" << res << ",$0,1" << endl;
		outfile << "b" << bi << 1 << ":" << endl;
		bi++;
	}
	if (oper == ">") {
		outfile << "\t" << "bgt" << "\t" << arg1 << "," << arg2 << ",b" << bi << 0 << endl;
		outfile << "\t" << "ori" << "\t" << res << ",$0,0" << endl;
		outfile << "\t" << "b" << "\tb" << bi << 1 << endl;
		outfile << "b" << bi << 0 << ":" << endl;
		outfile << "\t" << "ori" << "\t" << res << ",$0,1" << endl;
		outfile << "b" << bi << 1 << ":" << endl;
		bi++;
	}
	if (oper == ">=") {
		outfile << "\t" << "bge" << "\t" << arg1 << "," << arg2 << ",b" << bi << 0 << endl;
		outfile << "\t" << "ori" << "\t" << res << ",$0,0" << endl;
		outfile << "\t" << "b" << "\tb" << bi << 1 << endl;
		outfile << "b" << bi << 0 << ":" << endl;
		outfile << "\t" << "ori" << "\t" << res << ",$0,1" << endl;
		outfile << "b" << bi << 1 << ":" << endl;
		bi++;
	}
	if (oper == "==") {
		outfile << "\t" << "beq" << "\t" << arg1 << "," << arg2 << ",b" << bi << 0 << endl;
		outfile << "\t" << "ori" << "\t" << res << ",$0,0" << endl;
		outfile << "\t" << "b" << "\tb" << bi << 1 << endl;
		outfile << "b" << bi << 0 << ":" << endl;
		outfile << "\t" << "ori" << "\t" << res << ",$0,1" << endl;
		outfile << "b" << bi << 1 << ":" << endl;
		bi++;
	}
	if (oper == "!=") {
		outfile << "\t" << "bne" << "\t" << arg1 << "," << arg2 << ",b" << bi << 0 << endl;
		outfile << "\t" << "ori" << "\t" << res << ",$0,0" << endl;
		outfile << "\t" << "b" << "\tb" << bi << 1 << endl;
		outfile << "b" << bi << 0 << ":" << endl;
		outfile << "\t" << "ori" << "\t" << res << ",$0,1" << endl;
		outfile << "b" << bi << 1 << ":" << endl;
		bi++;
	}
	if (oper == "=") {
		outfile << "\t" << "add" << "\t" << res << ',' << arg1 << ",$0" << endl;
	}
	if (oper == "-=") {
		outfile << "\t" << "sub" << "\t" << res << ',' << res << "," << arg1 << endl;
	}
	if (oper == "+=") {
		outfile << "\t" << "add" << "\t" << res << ',' << res << "," << arg1 << endl;
	}
	if (oper == "*=") {
		outfile << "\t" << "mul" << "\t" << res << ',' << res << "," << arg1 << endl;
	}
	if (oper == "/=") {
		outfile << "\t" << "div.d" << "\t" << res << ',' << res << "," << arg1 << endl;
	}
	if (oper == "^=") {
		outfile << "\t" << "xor" << "\t" << res << ',' << res << "," << arg1 << endl;
	}
	if (oper == "|=") {
		outfile << "\t" << "or" << "\t" << res << ',' << res << "," << arg1 << endl;
	}
	if (oper == "&=") {
		outfile << "\t" << "and" << "\t" << res << ',' << res << "," << arg1 << endl;
	}
	if (oper == "<<=") {
		outfile << "\t" << "sllv" << "\t" << res << ',' << res << "," << arg1 << endl;
	}
	if (oper == ">>=") {
		outfile << "\t" << "srlv" << "\t" << res << ',' << res << "," << arg1 << endl;
	}
	if (oper == "||") {
		outfile << "\t" << "bnez" << "\t" << arg1 << ",b" << bi << 0 << endl;
		outfile << "\t" << "bnez" << "\t" << arg2 << ",b" << bi << 0 << endl;
		outfile << "\t" << "ori" << "\t" << res << ",$0,0" << endl;
		outfile << "\t" << "b" << "\tb" << bi << 1 << endl;
		outfile << "b" << bi << 0 << ":" << endl;
		outfile << "\t" << "ori" << "\t" << res << ",$0,1" << endl;
		outfile << "b" << bi << 1 << ":" << endl;
		bi++;
	}
	if (oper == "&&") {
		outfile << "\t" << "beqz" << "\t" << arg1 << ",b" << bi << 0 << endl;
		outfile << "\t" << "beqz" << "\t" << arg2 << ",b" << bi << 0 << endl;
		outfile << "\t" << "ori" << "\t" << res << ",$0,1" << endl;
		outfile << "\t" << "b" << "\tb" << bi << 1 << endl;
		outfile << "b" << bi << 0 << ":" << endl;
		outfile << "\t" << "ori" << "\t" << res << ",$0,0" << endl;
		outfile << "b" << bi << 1 << ":" << endl;
		bi++;
	}
	if (oper == "|") {
		outfile << "\t" << "or" << "\t" << res << ',' << arg1 << "," << arg2 << endl;
	}
	if (oper == "&") {
		outfile << "\t" << "and" << "\t" << res << ',' << arg1 << "," << arg2 << endl;
	}
	if (oper == "^") {
		outfile << "\t" << "xor" << "\t" << res << ',' << arg1 << "," << arg2 << endl;
	}
	if (oper == "IF") {
		ie.push(is);
		int ei = ie.top();
		is++;
		reg_claer(sym);
		outfile << "\t" << "beqz" << "\t" << arg1 << ", EL" << ei << endl;
	}
	if (oper == "EL") {
		int ei = ie.top();
		reg_claer(sym);
		outfile << "\t" << "b" << "\t" << "IE" << ei << endl;
		outfile << "EL" << ei << ":" << endl;
	}
	if (oper == "IE") {
		int ei = ie.top();
		ie.pop();
		reg_claer(sym);
		outfile << "IE" << ei << ":" << endl;
	}
	if (oper == "WH") {
		we.push(ws);
		ws++;
		int ei = we.top();
		reg_claer(sym);
		outfile << "WH" << ei << ":" << endl;
	}
	if (oper == "DO") {
		int ei = we.top();
		outfile << "\t" << "beqz" << "\t" << arg1 << ", WE" << ei << endl;
	}
	if (oper == "WE") {
		int ei = we.top();
		we.pop();
		reg_claer(sym);
		outfile << "\t" << "b" << "\t" << "WH" << ei << endl;
		outfile <<  "WE" << ei <<":"<< endl;
		
	}
}
void ASS::reg_claer(Symbol_table* sym) {
	store_data(sym);
	for (int i = 0; i < 8; i++) {
		regs[i] = "empty";
		regs_times[i] = 10;
	}
}