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
    bool is_equal(string,string);
    set<string> LL1_VT;
    set<string> LL1_VN;
public:
	Syntactic(/* args */);
	~Syntactic();
	void Init();
	void LL1solution(string file_name);
	int reduction(int state, int);
	bool find(int state);
	void print_LL1Grammar();
};

Syntactic::Syntactic(/* args */)
{
	Init();
}

Syntactic::~Syntactic()
{
}
void Syntactic::LL1solution(string file_name) {
	stack<string> S;
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
        string S_top = S.top();
        S.pop();
        if(LL1_VT.count(S_top)){
            if(is_equal(S_top, next)){
                continue;
            }
            else{
                cout<<"error: terminals \""<<S_top<<"\" and \""<<next<<"\" isn't match."<<endl;
                break;
            }
        }
        else if(LL1_VN.count(S_top)){
            if(LL1_analize_list[S_top].count(next)){
                int grammer = LL1_analize_list[S_top][next]; 
                for(int i = LL1Grammar[grammer].size()-1; i >= 0 ; i++){
                    S.push(LL1Grammar[grammer][i]);
                }
                goto label;
            }
            else{
                cout<<"Error: \""<<S_top<<"\" and \""<<next<<"\" can't be find in LL1 analize table."<<endl;
                break;
            }
        }
        else if(next == "#" && S_top == "#"){
            cout<<"Success!"<<endl;
            break;
        }
	}
}

void Syntactic::print_LL1Grammar() {
	for (int i = 0; i < LL1Grammar.size(); i++) {\
        cout << i<<":\t";
		for (int j = 0; j < LL1Grammar[i].size(); j++) {
			cout << LL1Grammar[i][j] << ' ';
		}
		cout << endl;
	}
}

bool Syntactic::is_equal(string input,string name){
    if(input == "w0"){
        if(name == "++" || name == "--" || name == "!" || name == "~" || name == "+" || name == "-"){
            return true;
        } 
        else return false;
    }
    if(input == "w1"){
        if(name == "*" || name == "/" || name == "%"){
            return true;
        }
        else return false;
    }
    if(input == "w2"){
        if(name == "+" || name == "-"){
            return true;
        }
        else return false;
    }
    if(input == "w3"){
        if(name == "<<" || name == ">>"){
            return true;
        }
        else return false;
    }
    if(input == "w4"){
        if(name == "<" || name == "<=" || name == ">" || name == ">="){
            return true;
        }
        else return false;
    }
    if(input == "w5"){
        if(name == "==" || name == "!="){
            return true;
        }
        else return false;
    }
    if(input == "w6"){
        if(name == "=" || name == "-=" || name == "+=" || name == "*=" || name == "/=" || name == "%=" || name == "^=" || name == "|=" ||name == "&=" || name == "<<=" || name == ">>="){
            return true;
        }
        else return false;
    }
    if(input == "obj"){
        if(name == "c" || name == "v" || name == "ch" || name == "s"){
            return true;
        }
        else return false;
    }
    return name == input;
        
}

void Syntactic::Init() {
	//文法初始化
	vector<string> tmp;
	//0
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//1
	tmp.push_back("E");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//2
	tmp.push_back("AsE");
	tmp.push_back(";");
	tmp.push_back("E");
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
	tmp.push_back("LoE");
	tmp.push_back("AsE1");
	LL1Grammar.push_back(tmp);
	tmp.clear();
	//6
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
	tmp.push_back("T");
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

    //LL1分析表
    LL1_analize_list["S"]["obj"]=1;
    LL1_analize_list["S"]["w0"]=1;
    LL1_analize_list["S"]["("]=1;
    LL1_analize_list["S"]["#"]=1;

    LL1_analize_list["E"]["obj"]=2;
    LL1_analize_list["E"]["w0"]=2;
    LL1_analize_list["E"]["("]=2;
    LL1_analize_list["E"]["#"]=3;

    LL1_analize_list["AsE"]["obj"]=4;
    LL1_analize_list["AsE"]["w0"]=4;
    LL1_analize_list["AsE"]["("]=4;

    LL1_analize_list["AsE1"]["w6"]=5;
    LL1_analize_list["AsE1"][")"]=6;
    LL1_analize_list["AsE1"][";"]=6;
    LL1_analize_list["AsE1"]["#"]=6;

    LL1_analize_list["LoE"]["obj"]=7;
    LL1_analize_list["LoE"]["w0"]=7;
    LL1_analize_list["LoE"]["("]=7;

    LL1_analize_list["LoE1"]["||"]=8;
    LL1_analize_list["LoE1"]["w6"]=9;
    LL1_analize_list["LoE1"][")"]=9;
    LL1_analize_list["LoE1"][";"]=9;
    LL1_analize_list["LoE1"]["#"]=9;

    LL1_analize_list["LaE"]["obj"]=10;
    LL1_analize_list["LaE"]["w0"]=10;
    LL1_analize_list["LaE"]["("]=10;

    LL1_analize_list["LaE1"]["&&"]=11;
    LL1_analize_list["LaE1"]["||"]=12;
    LL1_analize_list["LaE1"]["w6"]=12;
    LL1_analize_list["LaE1"][")"]=12;
    LL1_analize_list["LaE1"][";"]=12;
    LL1_analize_list["LaE1"]["#"]=12;

    LL1_analize_list["OrE"]["obj"]=13;
    LL1_analize_list["OrE"]["w0"]=13;
    LL1_analize_list["OrE"]["("]=13;

    LL1_analize_list["OrE1"]["|"]=14;
    LL1_analize_list["OrE1"]["&&"]=15;
    LL1_analize_list["OrE1"]["||"]=15;
    LL1_analize_list["OrE1"]["w6"]=15;
    LL1_analize_list["OrE1"][")"]=15;
    LL1_analize_list["OrE1"][";"]=15;
    LL1_analize_list["OrE1"]["#"]=15;

    LL1_analize_list["XoE"]["obj"]=16;
    LL1_analize_list["XoE"]["w0"]=16;
    LL1_analize_list["XoE"]["("]=16;

    LL1_analize_list["XoE1"]["^"]=17;
    LL1_analize_list["XoE1"]["|"]=18;
    LL1_analize_list["XoE1"]["&&"]=18;
    LL1_analize_list["XoE1"]["||"]=18;
    LL1_analize_list["XoE1"]["w6"]=18;
    LL1_analize_list["XoE1"][")"]=18;
    LL1_analize_list["XoE1"][";"]=18;
    LL1_analize_list["XoE1"]["#"]=18;

    LL1_analize_list["AnE"]["obj"]=19;
    LL1_analize_list["AnE"]["w0"]=19;
    LL1_analize_list["AnE"]["("]=19;

    LL1_analize_list["AnE1"]["&"]=20;
    LL1_analize_list["AnE1"]["^"]=21;
    LL1_analize_list["AnE1"]["|"]=21;
    LL1_analize_list["AnE1"]["&&"]=21;
    LL1_analize_list["AnE1"]["||"]=21;
    LL1_analize_list["AnE1"]["w6"]=21;
    LL1_analize_list["AnE1"][")"]=21;
    LL1_analize_list["AnE1"][";"]=21;
    LL1_analize_list["AnE1"]["#"]=21;

    LL1_analize_list["EqE"]["obj"]=22;
    LL1_analize_list["EqE"]["w0"]=22;
    LL1_analize_list["EqE"]["("]=22;

    LL1_analize_list["EqE1"]["w5"]=23;
    LL1_analize_list["EqE1"]["&"]=24;
    LL1_analize_list["EqE1"]["^"]=24;
    LL1_analize_list["EqE1"]["|"]=24;
    LL1_analize_list["EqE1"]["&&"]=24;
    LL1_analize_list["EqE1"]["||"]=24;
    LL1_analize_list["EqE1"]["w6"]=24;
    LL1_analize_list["EqE1"][")"]=24;
    LL1_analize_list["EqE1"][";"]=24;
    LL1_analize_list["EqE1"]["#"]=24;

    LL1_analize_list["ReE"]["obj"]=25;
    LL1_analize_list["ReE"]["w0"]=25;
    LL1_analize_list["ReE"]["("]=25;

    LL1_analize_list["ReE1"]["w4"]=26;
    LL1_analize_list["ReE1"]["w5"]=27;
    LL1_analize_list["ReE1"]["&"]=27;
    LL1_analize_list["ReE1"]["^"]=27;
    LL1_analize_list["ReE1"]["|"]=27;
    LL1_analize_list["ReE1"]["&&"]=27;
    LL1_analize_list["ReE1"]["||"]=27;
    LL1_analize_list["ReE1"]["w6"]=27;
    LL1_analize_list["ReE1"][")"]=27;
    LL1_analize_list["ReE1"][";"]=27;
    LL1_analize_list["ReE1"]["#"]=27;

    LL1_analize_list["ShE"]["obj"]=28;
    LL1_analize_list["ShE"]["w0"]=28;
    LL1_analize_list["ShE"]["("]=28;

    LL1_analize_list["ShE1"]["w3"]=29;
    LL1_analize_list["ShE1"]["w4"]=30;
    LL1_analize_list["ShE1"]["w5"]=30;
    LL1_analize_list["ShE1"]["&"]=30;
    LL1_analize_list["ShE1"]["^"]=30;
    LL1_analize_list["ShE1"]["|"]=30;
    LL1_analize_list["ShE1"]["&&"]=30;
    LL1_analize_list["ShE1"]["||"]=30;
    LL1_analize_list["ShE1"]["w6"]=30;
    LL1_analize_list["ShE1"][")"]=30;
    LL1_analize_list["ShE1"][";"]=30;
    LL1_analize_list["ShE1"]["#"]=30;

    LL1_analize_list["AdE"]["obj"]=31;
    LL1_analize_list["AdE"]["w0"]=31;
    LL1_analize_list["AdE"]["("]=31;

    LL1_analize_list["AdE1"]["w2"]=32;
    LL1_analize_list["AdE1"]["w3"]=33;
    LL1_analize_list["AdE1"]["w4"]=33;
    LL1_analize_list["AdE1"]["w5"]=33;
    LL1_analize_list["AdE1"]["&"]=33;
    LL1_analize_list["AdE1"]["^"]=33;
    LL1_analize_list["AdE1"]["|"]=33;
    LL1_analize_list["AdE1"]["&&"]=33;
    LL1_analize_list["AdE1"]["||"]=33;
    LL1_analize_list["AdE1"]["w6"]=33;
    LL1_analize_list["AdE1"][")"]=33;
    LL1_analize_list["AdE1"][";"]=33;
    LL1_analize_list["AdE1"]["#"]=33;

    LL1_analize_list["MuE"]["obj"]=34;
    LL1_analize_list["MuE"]["w0"]=34;
    LL1_analize_list["MuE"]["("]=34;

    LL1_analize_list["MuE1"]["w1"]=35;
    LL1_analize_list["MuE1"]["w2"]=36;
    LL1_analize_list["MuE1"]["w3"]=36;
    LL1_analize_list["MuE1"]["w4"]=36;
    LL1_analize_list["MuE1"]["w5"]=36;
    LL1_analize_list["MuE1"]["&"]=36;
    LL1_analize_list["MuE1"]["^"]=36;
    LL1_analize_list["MuE1"]["|"]=36;
    LL1_analize_list["MuE1"]["&&"]=36;
    LL1_analize_list["MuE1"]["||"]=36;
    LL1_analize_list["MuE1"]["w6"]=36;
    LL1_analize_list["MuE1"][")"]=36;
    LL1_analize_list["MuE1"][";"]=36;
    LL1_analize_list["MuE1"]["#"]=36;

    LL1_analize_list["UnE"]["w0"]=37;
    LL1_analize_list["UnE"]["obj"]=38;
    LL1_analize_list["UnE"]["("]=38;

    LL1_analize_list["T"]["obj"]=39;
    LL1_analize_list["T"]["("]=40;

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
int main() {
	Syntactic Syn;
	// Syn.print_LL1Grammar();
    Syn.LL1solution("G:\\学习\\大三下\\Compilers\\hw1\\tokens.txt");
}