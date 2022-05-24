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
	//getchar();
}