#pragma once
#include<vector>
#include<iostream>
#include<map>
#include<unordered_map>
#include<stack>
#include<fstream>
#include<set>
#include"symbol_table.h"
using namespace std;
class Syntactic
{
private:
	bool is_equal(string, string);//�ж������ַ��Ƿ����
	bool is_op(string);//�ж��Ƿ�Ϊ������
	bool is_va(string);//�ж��Ƿ�Ϊ����
	bool is_type(string);//�ж��Ƿ�Ϊ����
	bool is_legal(string, Symbol_table*);//�жϸñ����Ƿ�Ϸ��ı�ʹ��(��������ʹ�ã�
	void LL1print_stack();//��ӡLL1��ջ
	void SLR1print_stack();//SLR1��ջ
	void Init();//LL1��ʼ��
	void SLR1_Init();//SLR1��ʼ��
	void SLR1_Reduction(int num);//SLR1��Լ����
	int temp_count;//��ʹ�õ���ʱ�����ļ���
	string is_same_type(string a, string b, Symbol_table* sym);//�ж����������Ƿ�������ͬ����
	map<string, map<string, int> > LL1_analize_list;//LL1������
	unordered_map<int, string> token2terminal;//��tokenת�����ս��
	vector<vector<string> > LL1Grammar;//LL1���ķ�
	set<string> LL1_VT;//�ս������
	set<string> LL1_VN;//���ս������
	stack<string> S;//�﷨ջ
	vector<vector<string> > SLR1_Grammar;//SLR1�ķ�
	map<string, map<string, string> > SLR1_analize_list;//SLR1������
	stack<string> SEM;//����ջ
	stack<string> Operator_stack;//������ջ
	vector<string> temp_type;//��ʱ����������
	vector<string> to_special(string terminal);//ʹ���ս���ܹ��������ķ���ʶ��
	vector<string> SLR1_to_special(string terminal);//ʹ���ս���ܹ��������ķ���ʶ��
	ofstream stack_output;//LL1ջ���λ��
	ofstream quadruple_output;//��Ԫʽ���λ��
	ofstream action_output;//�����������
	ofstream OPS_output;//����ջ���
	ofstream SEM_output;//����ջ���
	vector<string> QT;//��¼��Ԫʽ����
public:
	Syntactic(/* args */);
	~Syntactic();
	void SLR1solution(string file_name);
	void LL1solution(string file_name, Symbol_table* Symbol);
	void print_LL1Grammar();
	
};