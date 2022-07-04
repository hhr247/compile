#include "symbol_table.h"


master_table::master_table(string n, string t, int a, string c) {
	name = n;
	type = t;
	address = a;
	character = c;
}

func_table::func_table(string n, string t) {
	name = n;
	return_type = t;
}
variable_table::variable_table(string n, string t, int l) {
	name = n;
	tval = t;
	len = l;
}
Symbol_table::Symbol_table() {

}
Symbol_table::~Symbol_table() {

}
int Symbol_table::add_master(string a) {
	int i = find(a);
	if (i == -1) {
		master_table m(a);
		symbol.push_back(m);
		return symbol.size() - 1;
	}
	else return i;
}
bool Symbol_table::connect_func( string name, string return_type) {
	int i = find(name);
	if (i == -1) {
		return false;
	}
	else {
		if (findf(name) != -1) {
			return false;
		}
		func_table n(name, return_type);
		func.push_back(n);
		symbol[i].address = func.size() - 1;
		symbol[i].type = "f";
		symbol[i].character = "id";
	}
	return true;
}
bool Symbol_table::connect_variable(string name, string tval, int len) {
	int i = find(name);
	if (i == -1) {
		return false;
	}
	else {
		if (findf(name) != -1) {
			return false;
		}
		variable_table n(name, tval, len);
		variable.push_back(n);
		symbol[i].address = variable.size() - 1;
		symbol[i].type = "v";
		symbol[i].character = "id";
	}
	return true;
}
int Symbol_table::find(string a) {
	for (int i = 0; i < symbol.size(); i++) {
		if (symbol[i].name == a) {
			return i;
		}
	}
	return -1;
}
int Symbol_table::findf(string a) {
	for (int i = 0; i < func.size(); i++) {
		if (func[i].name == a) {
			return i;
		}
	}
	return -1;
}
int Symbol_table::findv(string a) {
	for (int i = 0; i < variable.size(); i++) {
		if (variable[i].name == a) {
			return i;
		}
	}
	return -1;
}
string Symbol_table::getname(string a) {
	string id = a.substr(1);
	int i = stoi(id);
	return symbol[i].name;
}

void Symbol_table::print_table() {
	ofstream outfile("Symbol_table.txt");
	
	outfile << "SYMBOL TABLE" << endl;
	outfile << setiosflags(ios::right) << setw(15);
	outfile << setw(15) << "name" << setw(15) << "type" << setw(15) << "character" << setw(15) << "address"<<endl;
	for (int i = 0; i < symbol.size(); i++) {
		outfile << setiosflags(ios::right) << setw(15);
		outfile << setw(15) << symbol[i].name << setw(15) << symbol[i].type << setw(15) << symbol[i].character << setw(15) << symbol[i].address << endl;
	}
	outfile << endl;
	outfile << endl;
	outfile << "VARIABLE TABLE" << endl;
	outfile << setw(15) << "name" << setw(15) << "tval" << setw(15) << "len" << endl;
	for (int i = 0; i < variable.size(); i++) {
		outfile << setw(15) <<  variable[i].name << setw(15) << variable[i].tval << setw(15) << variable[i].len << endl;
	}
	outfile << endl;
	outfile << endl;
	outfile << "FUNCTION TABLE" << endl;
	outfile << setw(15) << "name" << setw(15) << "return_type" << endl;
	for (int i = 0; i < func.size(); i++) {
		outfile << setiosflags(ios::right) << setw(15);
		outfile << setw(15) << func[i].name << setw(15) << func[i].return_type << endl;
	}
	outfile.close();
}