#include<iostream>
#include<vector>
#include<string>
#include <unordered_map>
#include<fstream>
using namespace std;


#define IT_CODE 5

/*
注释 0
常数 1
字符 2
字符串 3
标识符 4
关键字 5-36
界符+运算符等 37 - 83;
*/
//Constant Table
vector<string> CT;

//char table
vector<string> cT;

//string table
vector<string> sT;


//Keyword Table
unordered_map<string,int> KT;
//Partition Table
unordered_map<string,int> PT;

//Identify Table
vector<string> IT;

//
int state_change(int state,char ch);

//
int state_to_code(int state_before,char*token);

void reset(FILE* &fp, char* &token, int &state,int &i);
void deal_code(int code,ofstream outfile);

//init keyword table
void init_kt(){
    KT.insert(std::make_pair("auto",5));
    KT.insert(std::make_pair("short",6));
    KT.insert(std::make_pair("int",7));
    KT.insert(std::make_pair("long",8));
    KT.insert(std::make_pair("float",9));
    KT.insert(std::make_pair("double",10));
    KT.insert(std::make_pair("char",11));
    KT.insert(std::make_pair("struct",12));
    KT.insert(std::make_pair("union",13));
    KT.insert(std::make_pair("enum",14));
    KT.insert(std::make_pair("typedef",15));
    KT.insert(std::make_pair("const",16));
    KT.insert(std::make_pair("unsigned",17));
    KT.insert(std::make_pair("signed",18));
    KT.insert(std::make_pair("externed",19));
    KT.insert(std::make_pair("register",20));
    KT.insert(std::make_pair("static",21));
    KT.insert(std::make_pair("volatile",22));
    KT.insert(std::make_pair("void",23));
    KT.insert(std::make_pair("if",24));
    KT.insert(std::make_pair("else",25));
    KT.insert(std::make_pair("switch",26));
    KT.insert(std::make_pair("case",27));
    KT.insert(std::make_pair("for",28));
    KT.insert(std::make_pair("do",29));
    KT.insert(std::make_pair("while",30));
    KT.insert(std::make_pair("goto",31));
    KT.insert(std::make_pair("continue",32));
    KT.insert(std::make_pair("break",33));
    KT.insert(std::make_pair("default",34));
    KT.insert(std::make_pair("sizeof",35));
    KT.insert(std::make_pair("return",36));
}
//init Partition table
void init_pt(){
    PT.insert(std::make_pair("{",37));
    PT.insert(std::make_pair("}",38));
    PT.insert(std::make_pair("(",39));
    PT.insert(std::make_pair(")",40));
    PT.insert(std::make_pair("[",41));
    PT.insert(std::make_pair("]",42));

    PT.insert(std::make_pair("/*",43));
    PT.insert(std::make_pair("*/",44));
    PT.insert(std::make_pair("//",45));

    PT.insert(std::make_pair("\'",46));
    PT.insert(std::make_pair("\"",47));

    PT.insert(std::make_pair("<",48));
    PT.insert(std::make_pair("<=",49));
    PT.insert(std::make_pair(">",50));
    PT.insert(std::make_pair(">=",51));
    PT.insert(std::make_pair("==",52));
    PT.insert(std::make_pair("!=",53));

    PT.insert(std::make_pair("=",54));
    PT.insert(std::make_pair("+",55));
    PT.insert(std::make_pair("++",56));
    PT.insert(std::make_pair("+=",57));
    PT.insert(std::make_pair("-",58));
    PT.insert(std::make_pair("--",59));
    PT.insert(std::make_pair("-=",60));
    PT.insert(std::make_pair("*",61));
    PT.insert(std::make_pair("*=",62));
    PT.insert(std::make_pair("/",63));
    PT.insert(std::make_pair("/=",64));
    PT.insert(std::make_pair("%",65));
    PT.insert(std::make_pair("%=",66));
    PT.insert(std::make_pair("<<",67));
    PT.insert(std::make_pair("<<=",68));
    PT.insert(std::make_pair(">>",69));
    PT.insert(std::make_pair(">>=",70));
    PT.insert(std::make_pair("&",71));
    PT.insert(std::make_pair("&=",72));
    PT.insert(std::make_pair("&&",73));
    PT.insert(std::make_pair("|",74));
    PT.insert(std::make_pair("|=",75));
    PT.insert(std::make_pair("||",76));
    PT.insert(std::make_pair("^",77));
    PT.insert(std::make_pair("^=",78));

    PT.insert(std::make_pair(",",79));
    PT.insert(std::make_pair("?",80));
    PT.insert(std::make_pair(":",81));
    PT.insert(std::make_pair("!",82));
    PT.insert(std::make_pair("~",83));
    PT.insert(std::make_pair(";",84));
    PT.insert(std::make_pair(".",85));
}

int main(void){
    init_kt();
    init_pt();
    char* fname;
    fname = (char*)(malloc(sizeof(char) * 20));
    char* token;
    token = (char*)(malloc(sizeof(char) * 256));
    cin>>fname;
    FILE * fp = fopen((char*)(fname),"r");
    ofstream outfile;
    outfile.open("output.txt");
    int state = 1;
    char ch;
    int i = 0;
    int line = 1;
    int flag = 1;
    while(true){
        int line_before = line;
        ch = getc(fp);
        int state_before = state;
        state = state_change(state,ch);
        // cout<<state<<endl;
        if(ch == '\n' && flag){
            line++;
        }
        flag = 1;
        if(state == -1){
            printf("ERROR\n");
            outfile<<"Error\n";
            break;
        }
        if(state == 1){
            continue;
        }
        else if(state){
            token[i++] = ch;
        }
        else{
            token[i++] = '\0';
            string str = token;
            int code = state_to_code(state_before, token); 
            if(code == 3){
                str = str.substr(1,str.size()-2);
                token[0] = '\"';
                token[1] = '\0';
                int code1 = state_to_code(4,token);
                outfile<<"<"<<token<<","<<code1<<","<<line_before<<">\n";
            }
            if(code == 2){
                str = str.substr(1,str.size()-2);
                token[0] = '\'';
                token[1] = '\0';
                int code1 = state_to_code(4,token);
                outfile<<"<"<<token<<","<<code1<<","<<line_before<<">\n";
            }
            reset(fp,token,state,i);
            flag = 0;
            if(code){
                outfile<<"<"<<str<<","<<code<<","<<line_before<<">\n";
            }
            else{
                outfile<<"<,"<<code<<','<<line_before<<">\n";
            }
            if(code == 3){
                int code1 = state_to_code(4,token);
                outfile<<"<"<<token<<","<<code1<<","<<line_before<<">\n";
            }
            if(code == 2){
                int code1 = state_to_code(4,token);
                outfile<<"<"<<token<<","<<code1<<","<<line_before<<">\n";
            }
        }
        
        if(ch == EOF){
            break;
        }
    }
}

bool is_letter(char a){
    return (a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z');
}

bool is_dig(char a){
    return (a >= '0' && a <= '9');
}

int state_change(int state,char ch){
    if(state == 1){
        if(ch == ' ' || ch == '\n' || ch == '\t'){
            return state;
        }
        if(is_letter(ch)){
            state = 2;
            return state;
        }
        if(ch == '0'){
            return 39;
        }
        if(is_dig(ch)){
            state = 3;
            return state;
        }
        if(ch == '{'){
            return 4;
        }
        if(ch == '}'){
            return 4;
        }
        if(ch == '('){
            return 4;
        }
        if(ch == ')'){
            return 4;
        }
        if(ch == '['){
            return 4;
        }
        if(ch == ']'){
            return 4;
        }
        if(ch == '/'){
            return 5;
        }
        if(ch == '\''){
            return 6;
        }
        if(ch == '\"'){
            return 7;
        }
        if(ch == '<'){
            return 8;
        }
        if(ch == '>'){
            return 9;
        }
        if(ch == '='){
            return 10;
        }
        if(ch == '!'){
            return 11;
        }
        if(ch == '+'){
            return 12;
        }
        if(ch == '-'){
            return 13;
        }
        if(ch == '*'){
            return 14;
        }
        if(ch == '%'){
            return 15;
        }
        if(ch == '&'){
            return 16;
        }
        if(ch == '|'){
            return 17;
        }
        if(ch == '^'){
            return 18;
        }
        if(ch == ','){
            return 4;
        }
        if(ch == '?'){
            return 4;
        }
        if(ch == ':'){
            return 4;
        }
        if(ch == '~'){
            return 4;
        }
        if(ch == ';'){
            return 4;
        }
        if(ch == '.'){
            return 4;
        }
        else return 1;
    }
    else if(state == 2){//字母或者标识符
        if(is_letter(ch) || is_dig(ch)){
            state = 2;
            return 2;
        }
        else{
            return 0;
        }
    }
    else if(state == 3){//整数变化
        if(is_dig(ch)){
            return 3;
        }
        else if(ch == '.'){
            return 24;
        }
        else if(ch == 'e' || ch == 'E'){
            return 25;
        }
        else{
            return 0;
        }
    }
    else if(state == 24){//小数。
        if(is_dig(ch)){
            return 24;
        }
        else if(ch == 'e' || ch == 'E'){
            return 25;
        }
        else return 0;
    }
    else if(state == 25){//科学计数法
        if(ch == '-' || ch == '+'){
            return 26;
        }
        else if(is_dig(ch)){
            return 27;
        }
        else return 0;
    }
    else if(state == 26){
        if(is_dig(ch)){
            return 27;
        }
        else return 0;
    }
    else if(state == 27){//科学计数法
        if(is_dig(ch)){
            return 27;
        }
        else return 0;
    }
    else if(state == 4){//界符
        return 0;
    }
    else if(state == 5){//除号或者注释
        if(ch == '/'){
            return 28;
        }
        if(ch == '*'){
            return 29;
        }
        else 
            return 0;
    }
    else if(state == 28){//行注释
        if(ch == '\n' || ch == EOF){
            return 0;
        }
        else return 28;
    }
    else if(state == 29){//多行注释
        if(ch == '*'){
            return 30;
        }
        if(ch == EOF){
            return 0;
        }
        else return 29;
    }
    else if(state == 30){
        if(ch == '/' || ch == EOF){
            return 31;
        }
        else return 29;
    }
    else if(state == 31){
        return 0;
    }
    else if(state == 6){//字符
        if(ch == '\\'){
            return 32;
        }
        if(ch == '\''){
            return 34;
        }
        else{
            return 33;
        }
    }
    else if(state == 32){
        return 33;
    }
    else if(state == 33){
        if(ch == '\''){
            return 34;
        }
        else return -1;
    }
    else if(state == 34){
        return 0;
    }
    else if(state == 7){//字符串
        if(ch == '\"'){
            return 36;
        }
        else if(ch == '\n'){
            return -1;
        }
        else if(ch == '\\'){
            return 35;
        }
        else{
            return 7;
        }
    }
    else if(state == 35){
        return 7;
    }
    else if (state == 36){
        return 0;
    }
    else if(state == 8){//小于号
        if(ch == '='){
            return 4;
        }
        if(ch == '<'){
            return 37;
        }
        else return 0;
    }
    else if(state == 37){
        if(ch == '='){
            return 4;
        }
        else return 0;
    }
    else if(state == 9){//大于号
        if(ch == '='){
            return 4;
        }
        else if(ch == '>'){
            return 38;
        }
        else return 0;
    }
    else if(state == 38){
        if(ch == '='){
            return 4;
        }
        else return 0;
    }
    else if(state == 10){
        if(ch == '='){
            return 4;
        }
        else return 0;
    }
    else if(state == 11){
        if (ch == '=') return 4;
        else return 0; 
    }
    else if(state == 12){
        if(ch == '+') return 4;
        if(ch == '=') return 4;
        else return 0;
    }
    else if(state == 13){
        if(ch == '-') return 4;
        if(ch == '=') return 4;
        else return 0;
    }
    else if(state == 14){
        if(ch == '=') return 4;
        else return 0;
    }
    else if(state == 15){
        if(ch == '='){
            return 4;
        }
        else return 0;
    }
    else if(state == 16){
        if(ch == '&' || ch == '='){
            return 4;
        }
        else return 0;
    }
    else if(state == 17){
        if(ch == '|' || ch == '='){
            return 4;
        }
        else return 0;
    }
    else if(state == 18){
        if(ch == '='){
            return 4;
        }
        return 0;
    }
    else if(state == 39){
        if(ch == 'b'){
            return 40;
        }
        else if(ch == 'x'){
            return 41;
        }
        else if(ch > '0' && ch <='7'){
            return 42;
        }
        else if(ch == '0'){
            return 43;
        }
        else if(ch == '.'){
            return 24;
        }
        else return 0;
    }
    else if(state == 40){
        if(ch == '0' || ch == '1'){
            return 40;
        }
        if(ch == '.'){
            return -1;
        }
        else return 0;
    }
    else if(state == 41){
        if((ch >= '0' && ch <= '9') || (ch >='a' && ch<='f') || (ch >='A' && ch<='F')){
            return 41;
        }
        if(ch == '.'){
            return -1;
        }
        else return 0;
    }
    else if(state == 42){
        if(ch >= '0' && ch <='7'){
            return 42;
        }
        if(ch == '.'){
            return -1;
        }
        else return 0;
    }
    else if(state == 43){
        if(ch == '0'){
            return 43;
        }
        else if(ch >= '1' && ch <= '7'){
            return 42;
        }
        else if(ch == '.'){
            return 24;
        }
        else return 0;
    }
}

int state_to_code(int state_before,char*token){
    if(state_before == 2){
        string str = token;
        if(KT.count(str)){
            return KT[str];
        }
        else{
            IT.push_back(str);
            return 4;
        }
    }
    if(state_before == 3 || state_before == 24 || state_before == 25 || state_before == 26 || state_before == 27 || state_before == 39 || state_before == 40 || state_before == 41 || state_before == 42 ||state_before == 43){
        string str = token;
        CT.push_back(str);
        return 1;
    }
    if(state_before == 4){
        string str = token;
        return PT[str];
    }
    if(state_before == 5){
        string str = token;
        return PT[str];
    }
    if(state_before == 28 || state_before == 29 || state_before == 30 ||state_before == 31){
        return 0;
    }
    if(state_before == 34){
        string str = token;
        str.substr(1,1);
        cT.push_back(str);
        return 2;
    }
    if(state_before == 36){
        string str = token;
        str = str.substr(1,str.size()-2);
        sT.push_back(str);
        return 3;
    }
    if(state_before == 8 || state_before == 37){
        string str = token;
        return PT[str];
    }
    if(state_before == 9 || state_before == 38){
        string str = token;
        return PT[str];
    }
    if(state_before == 10){
        string str = token;
        return PT[str];
    }
    if(state_before == 11){
        string str = token;
        return PT[str];
    }
    if(state_before == 12){
        string str = token;
        return PT[str];
    }
    if(state_before == 13){
        string str = token;
        return PT[str];
    }
    if(state_before == 14){
        string str = token;
        return PT[str];
    }
    if(state_before == 15){
        string str = token;
        return PT[str];
    }
    if(state_before == 16){
        string str = token;
        return PT[str];
    }
    if(state_before == 17){
        string str = token;
        return PT[str];
    }
    if(state_before == 18){
        string str = token;
        return PT[str];
    }
}
void reset(FILE* &fp, char* &token, int &state,int &i){
    i = 0;
    state = 1;
    fseek(fp,-1L,1);
}