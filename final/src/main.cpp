#include"Lexical_analyzer.h"
#include"Syntactic.h"
#include"ASS.h"
#include"optimize.h"
int main() {
	char* fname;
	fname = (char*)(malloc(sizeof(char) * 256));
	cin >> fname;
	Symbol_table* sym;
	sym = new Symbol_table;
	Lexical_analyzer Lex;
	Lex.Solution(fname, sym);
	Syntactic Syn;
	Syn.LL1solution("../result/tokens.txt", sym);
	Optimize optim;
	optim.optimization("../result/quadruple-output.txt", sym);
	sym->print_table();
	ASS Asm;
	Asm.solution("../result/optimize.txt",sym);
	system("PAUSE");
}