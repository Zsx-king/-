#include<iostream>
#include<string>
#include<fstream>
using namespace std;
#define ERROR 0

typedef struct PNode {
	float coef; //系数
	int expn; //指数
	struct PNode* next;
} PNode, * Polynomial;
string head_1, head_2;//C++中有字符串类型
int temp;
char st = 'A';

void CreatPolyn(Polynomial& P, int m) //算法2.18 多项式的创建
{
	//输入m项的系数和指数，建立表示一个多项式的有序链表P
	Polynomial q, pre, s;
	int i;
	P = new PNode;
	P->next = NULL; //先建立一个带头结点的单链表
	char filename[20] = { 0 };
	cout << "请输入有关多项式p" << char(st + 32) << "系数和指数的数据文件名称（文件名+“.txt”，如Poly" << st << ".txt）：" << endl;
	++st;
	gets_s(filename);//从键盘上接收一个字符串（文件名字）
	fstream file;
	file.open(filename);
	if (!file) {
		cout << "未找到相关文件，无法打开！" << endl;
		exit(ERROR);
	}
	file >> head_1 >> head_2;//先把文件中第一行中的两个字符串吸收
	while (!file.eof())//判断是否读取到文件最后
	{
		s = new PNode; //生成新结点
		file >> s->coef >> s->expn; //输入元素值
		pre = P; //pre用于保存q的前驱，初值为头结点，每次外层循环一次就需要将q置于首节点，这时候pre也就是头结点（无值）
		q = P->next;//而后遍历寻找属于该输入节点的合适位置
		while (q && q->expn < s->expn) //通过比较指数找到第一个大于输入项指数的项q
		{
			pre = q;
			q = q->next;
		} //while
		s->next = q; //将输入项s插入到q和其前驱结点pre之间
		pre->next = s;
	}//for
	file.close();//关闭文件
} //CreatPolyn

void AddPolyn(Polynomial& Pa, Polynomial& Pb) //算法2.19 多项式的相加
{
	//多项式加法：Pa=Pa+Pb，利用两个多项式的结点构成“和多项式”
	Polynomial r, p1, p2, p3;
	float sum;
	p1 = Pa->next;
	p2 = Pb->next; //p1和p2初值分别指向Pa和Pb的第一个结点
	p3 = Pa; //p3指向和多项式的当前结点，初值为Pa
	while (p1 && p2) //p1和p2均非空
	{
		if (p1->expn == p2->expn) //指数相等
		{
			sum = p1->coef + p2->coef; //sum保存两项的系数和//sum=p1->coef*p2->coef;
			if (sum != 0) //系数和不为0
			{
				p1->coef = sum; //修改Pa当前结点p1的系数值为两项系数的和
				p3->next = p1;
				p3 = p3->next;
				p1 = p1->next;
				r = p2;
				p2 = p2->next;
				delete r; //删除Pb当前结点r
			}
			else //系数和为0
			{
				r = p1;
				p1 = p1->next;
				delete r; //删除Pa当前结点p1
				r = p2;
				p2 = p2->next;
				delete r; //删除Pb当前结点p2
			}
		}
		else if (p1->expn < p2->expn) //Pa当前结点p1的指数值小
		{
			p3->next = p1; //将p1链在p3之后
			p3 = p1; //p3指向p1
			p1 = p1->next; //p1指向后一项
		}
		else //Pa当前结点p2的指数值小
		{
			p3->next = p2; //将p2链在p3之后
			p3 = p2; //p3指向p2
			p2 = p2->next; //p2指向后一项
		}
	} //while
	p3->next = p1 ? p1 : p2; //插入非空多项式的剩余段
	//delete Pb; //释放Pb的头结点
} //AddPolyn
void Change(PNode* p1, PNode* p2) {//交换节点中的指数值和系数值，但是不改变链表的原有情况（即链不断，只是对数据进行交换而已）
	float f = p1->coef;
	int t = p1->expn;
	p1->coef = p2->coef;
	p1->expn = p2->expn;
	p2->coef = f;
	p2->expn = t;
}
void Sort(Polynomial Pa) {
	Polynomial p1,p2;
	p1 = p2 = Pa->next;
	while (p1&&p1->next) {//冒泡原理（内层循环每次将最大/最小的值放到最上面）
		p2 = p1->next;
		while (p2) {
			if (p1->expn > p2->expn)
				Change(p1, p2);
			p2 = p2->next;
		}
		p1 = p1->next;
	}
}
void disrepet(Polynomial& Pa) {//去出重复指数的项（由于乘运算之后得到的结果是笛卡尔积（即两个链表个数直接相乘，这样会导致存在重复指数的项））
	Polynomial p1,p2,p3,p4;
	p1=p2 = Pa->next;
	while (p1) {
		p3 = p1;//存放p2的前缀，由于找到重复的项后需要对该项进行删除，该链表又是单向链表，所以需要保存该链表的前驱从而保证链表的连续性。
		p2 = p1->next;
		while (p2) {
			if (p2->expn == p1->expn) {//此时，p2就是找到的重复指数的项，接下来需要将该项的系数与所比较的项相加后赋值给所比较的项，使用前驱确保链表连续，删除p2（使用其他变量赋值过了）
				p1->coef = p1->coef + p2->coef;
				//p2->coef = 0;
				p3->next = p2->next;//这里只是确保链的连续，所以不需要将前缀后移，即前缀没有变化。
				p4 = p2;//注：这里很容易与下面这句写反，顺序变化就会导致程序失败
				p2 = p2->next;
				delete p4;
			}
			else {
				p3 = p2;
				p2 = p2->next;
			}
		}
		p1 = p1->next;
	}//该步骤结束之后，会出现一种情况，存在系数为零的项，这是由于，原先乘后的多项式中存在一些系数和为零的项，这时候，就需要在对链表进行处理了
	p1 =  Pa->next;
	p2 = Pa;
	while (p1) {//此处的核心内容与上面几乎完全一样
		if (p1->coef == 0) {
			p2->next = p1->next;
			p3 = p1;
			p1 = p1->next;
			delete p3;
		}
		else {
			p2 = p1;
			p1 = p1->next;
		}
	}
}
void MultiplyPolyn(Polynomial& Pa, Polynomial& Pb) 
{
	Polynomial p1, p2, myhead=new PNode ;
	Polynomial Mp = myhead;//思路很简单，创建一个新的链表，用于接收连个链表的笛卡尔积，而后在调用去重，排序函数即可。
	p1 = Pa->next;
	p2 = Pb->next;
	while (p1) 
	{
		while (p2) {
			PNode* p = new PNode;
			p->expn = p1->expn + p2->expn;
			p->coef = p1->coef * p2->coef;
			Mp->next = p;
			p->next = NULL;//不添加该语句会导致程序异常退出
			Mp = p;
			p2 = p2->next;
		}
		p2 = Pb->next;
		p1 = p1->next;
	}
	Pa = myhead;
	disrepet(Pa);
	Sort(Pa);
	delete Pb;
} 

int main() {
	Polynomial Pa, Pb;
	Polynomial p;
	int i;

	//创建多项式Pa
	CreatPolyn(Pa, temp); //调用函数，输入Pa每一项的系数和指数

	//创建多项式Pb
	CreatPolyn(Pb, temp); //调用函数，输入Pa每一项的系数和指数

	//AddPolyn(Pa, Pb);
	MultiplyPolyn(Pa, Pb);
	cout << "多项式Pa和Pb相乘后的结果是：\n";
	p = Pa->next;
	i = 0;
	while (p) //输出相加后的结果，每一项以x^n表示
	{
		if (i)
			cout << " + ";
		cout << "(" << p->coef << ") * x^" << p->expn;
		p = p->next;
		i = 1;
	}
	cout << endl;
	return 0;
}