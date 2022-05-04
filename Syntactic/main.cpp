#include"Lexical_analyzer.h"
#include"Syntactic.h"
int main() {
	char* fname;
	fname = (char*)(malloc(sizeof(char) * 256));
	cin >> fname;
	Lexical_analyzer Lex;
	Lex.Solution(fname);
	Syntactic Syn;
	// Syn.print_LL1Grammar();
	Syn.LL1solution("tokens.txt");
	Syn.SLR1solution("tokens.txt");
	int a, b, c, d,e;
	a = b = c = d = e = 1;
	a = ++a * b - --c >> 2 == 2 & 1 ^ e | 10 && b || c;
}