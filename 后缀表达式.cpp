/***链栈实现后缀表达式求值***/

#include<iostream>
using namespace std;

const char oper[7] = { '+', '-', '*', '/', '(', ')', '#' };
#define OK 1
#define ERROR 0
#define OVERFLOW -2
typedef int SElemType;
typedef int Status;
typedef struct SNode {
	int data;
	struct SNode* next;
} SNode, * LinkStack;

Status InitStack(LinkStack& S) {
	S = NULL;
	return OK;
}
bool StackEmpty(LinkStack S) {
	if (!S)
		return true;
	return false;
}
Status Push(LinkStack& S, SElemType e) {
	SNode* p = new SNode;
	if (!p) {
		return OVERFLOW;
	}
	p->data = e;
	p->next = S;
	S = p;
	return OK;
}
Status Pop(LinkStack& S, SElemType& e) {
	SNode* p;
	if (!S)
		return ERROR;
	e = S->data;
	p = S;
	S = S->next;
	delete p;
	return OK;
}
Status GetTop(LinkStack S) {
	if (!S)
		return ERROR;
	return S->data;
}
bool In(char ch) {//判断ch是否为运算符
	for (int i = 0; i < 7; i++) {
		if (ch == oper[i]) {
			return true;
		}
	}
	return false;
}
char Precede(char theta1, char theta2) {//判断运算符优先级
	if ((theta1 == '(' && theta2 == ')') || (theta1 == '#' && theta2 == '#')) {
		return '=';
	}
	else if (theta1 == '(' || theta1 == '#' || theta2 == '(' || (theta1
		== '+' || theta1 == '-') && (theta2 == '*' || theta2 == '/')) {
		return '<';
	}
	else
		return '>';
}
char Operate(char first, char theta, char second) {//计算两数运算结果
	switch (theta) {
	case '+':
		return (first - '0') + (second - '0') + 48;
	case '-':
		return (first - '0') - (second - '0') + 48;
	case '*':
		return (first - '0') * (second - '0') + 48;
	case '/':
		return (first - '0') / (second - '0') + 48;
	}
	return 0;
}
int shift(char ch) {
	int s = 0;
	while (ch == ch - 128)
		s++;
	return s * 128 + ch ;
}
char Operate_(char first, char theta, char second) {
	switch (theta) {
	case '+':
		return (first - '0') + (second - '0') + 48;
	case '-':
		return (first - '0') - (second - '0') + 48;
	case '*':
		return (first - '0') * (second - '0') + 48;
	case '/':
		return (first - '0') / (second - '0') + 48;
	}
	return 0;
}
//
//实验　后缀表达式求值
char EvaluateExpression() {//存在bug，一：输入计算的值只能是个位数，二：结果不能超过128。
	LinkStack OPND;
	LinkStack OPTR;
	SElemType e1, e2;
	char ch, theta, a, b, x, top;

	InitStack(OPTR); //初始化OPTR栈
	InitStack(OPND);
	Push(OPTR, '#'); //将表达式起始符“#”压入OPTR栈
	cin >> ch;
	while (ch!= '#'/*|| (GetTop(OPTR) != '#')*/)
	{
		if (In(ch)) {
			Pop(OPND, e2);
			Pop(OPND, e1);
			Push(OPND, Operate_(e1, ch, e2));
		}
		else {
			Push(OPND, ch);
		}
		cin >> ch;
	} //while
	return GetTop(OPND); //OPND栈顶元素即为表达式求值结果
}
///实验  中缀表达式求值
char EvaluateExpression_() {//算术表达式求值的算符优先算法，设OPTR和OPND分别为运算符栈和操作数栈
	LinkStack OPND;
	LinkStack OPTR;
	SElemType e,e1, e2;
	char ch, theta=' ', a, b, x, top;
	InitStack(OPTR); //初始化OPTR栈
	InitStack(OPND);
	Push(OPTR, '#'); //将表达式起始符“#”压入OPTR栈
	cin >> ch;
	while (ch!='#'|| (GetTop(OPTR) != '#')) 
	{
		if (In(ch)) {
			x = Precede(GetTop(OPTR), ch);
			switch (x) {
			case '<':
				Push(OPTR, ch);
				cin >> ch;
				break;
			case '=':
				Pop(OPTR,e);
				cin >> ch;
				break;
			case '>':
				Pop(OPND, e2);
				Pop(OPND, e1);
				//Pop(OPTR, theta);//为了便与多位数字的函数调试（改变了数据类型，不再是char，而是int，所以，这里会出现编译错误）
				Push(OPND, Operate_(e1, theta, e2));
				break;
			}
		}
		else {
			Push(OPND, ch);
			cin >> ch;
		}
	}
	return GetTop(OPND); //OPND栈顶元素即为表达式求值结果
}
int _Operate(int a, char ch, int b) {
	switch (ch) {
	case '+':
		return a + b;
	case '-':
		return a - b;
	case '*':
		return a * b;
	case '/':
		return a / b;
	}
}
int NumbersEvaluateExpression() {
	LinkStack OPND;
	LinkStack OPTR;
	SElemType e1,e2;
	char ch;
	InitStack(OPTR);
	InitStack(OPND);
	cin >> ch;
	int s = 0;
	while (ch != '#')
	{
		if (In(ch)) {
			Pop(OPND, e2);
			Pop(OPND, e1);
			Push(OPND, _Operate(e1, ch, e2));
		}
		else if (ch == '$') {//由于cin不会读取空格键，所以，输入数据的时候需要使用其他字符将各个数字隔开
			Push(OPND, s);
			s = 0;
		}
		else {
			s = 10 * s + ch - 48;
		}
		cin >> ch;
	} 
	return GetTop(OPND); 
}
int main() {
	cout << "请输入您想要求的后缀表达式（注：每个数字后都需要字符$）：" << endl;
	cout << "您输入的后缀表达式的结果是：" << NumbersEvaluateExpression() << endl;
	//cout << "您输入的后缀表达式的结果是：" << EvaluateExpression_()-48 << endl;
	return 0;
}

