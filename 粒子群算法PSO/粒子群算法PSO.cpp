// 粒子群算法PSO.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<iostream>
#include<cmath>
#include<cstdlib>
#include<ctime>
#include<algorithm>
#include<cstdio>
using namespace std;
const int C1 = 2;    //学习因子
const int C2 = 2;
const int N = 20;    //种群规模
const int GEN = 10000;   //迭代次数
const double PI = 3.1415926;
class VarX {     //位移矢量
public:
	double x1, x2;
	double fitval;
	static double l1, r1;
	static double l2, r2;
	VarX() {
		fitval = 0.0;
		x1 = x2 = 0.0;
	}
};
double VarX::l1 = (-3.0);
double VarX::r1 = (12.1);
double VarX::l2 = (4.1);
double VarX::r2 = 5.8;
class VarV {    //速度矢量
public:
	double v1,v2;
	static double l1, r1;
	static double l2, r2;
	VarV() {
		v1 = v2 = 0.0;
	}
};
double VarV::l1 = (-3.0);
double VarV::r1 = (3.0);
double VarV::l2 = (-3.0);
double VarV::r2 = 3.0;
class psotype {   //种群个体
public:
	VarV V;
	VarX X;
	VarX PBest;   //个体局部最优
};
class PSO {
public:
	psotype population[N];
	double r1, r2;
	double w;   //惯性权重
	VarX GBest;  //全局最优
	PSO() {
		r1 = r2 = 0.0;
		w = 0.0;
	}
	void init();   //初始化
	double fx(double x1,double x2);   //所求函数
	double randval(double lval,double rval);   //随机数函数
	void findgbest();  //找全局最优
	void setpbest();   //找个体历史最优
	void Update();   //更新速度和位置
	void report();    //输出
	void calfit();    //计算适应值
};
void PSO::calfit() {
	for (int i = 0;i < N;i++) {
		population[i].X.fitval = fx(population[i].X.x1, population[i].X.x2);
	}
}
void PSO::init() {
	for (int i = 0;i < N;i++) {
		population[i].V.v1 = randval(VarV::l1, VarV::r1);
		population[i].V.v2 = randval(VarV::l2, VarV::r2);
		population[i].X.x1 = randval(VarX::l1, VarX::r1);
		population[i].X.x2 = randval(VarX::l2, VarX::r2);
		population[i].X.fitval = fx(population[i].X.x1, population[i].X.x2);
		population[i].PBest = population[i].X;
	}
	findgbest();
}
double PSO::fx(double x1,double x2) {
	double y = (21.5 + x1 * sin(4 * PI*x1) + x2*sin(20 * PI*x2));
	return y;
	//double y = x1*x1 + x2*x2;
	//return y;
}
void PSO::findgbest(){
	VarX M = population[0].PBest;
	for (int i = 1;i < N;i++) {
		if (population[i].PBest.fitval > M.fitval) {
			M = population[i].PBest;
		}
	}
	GBest = M;
}
void PSO::setpbest() {
	for (int i = 0;i < N;i++) {
		if (population[i].X.fitval > population[i].PBest.fitval) {
			population[i].PBest = population[i].X;
		}
	}
}
void PSO::Update() {
	for (int i = 0;i < N;i++) {
		r1 = randval(0.0, 1.0);
		r2 = randval(0.0, 1.0);
		w = randval(0.1, 0.9);
		population[i].V.v1=w*population[i].V.v1 +
			C1*r1*(population[i].PBest.x1 - population[i].X.x1) +
			C2*r2*(GBest.x1 - population[i].X.x1);
		if (population[i].V.v1 < (VarV::l1))
			population[i].V.v1 = (VarV::l1);
		if (population[i].V.v1 > (VarV::r1))
			population[i].V.v1 = (VarV::r1);
		population[i].V.v2 = w*population[i].V.v2 +
			C1*r1*(population[i].PBest.x2 - population[i].X.x2) +
			C2*r2*(GBest.x2 - population[i].X.x2);
		if (population[i].V.v2 < (VarV::l2))
			population[i].V.v2 = (VarV::l2);
		if (population[i].V.v2 >(VarV::r2))
			population[i].V.v2 = (VarV::r2);
		population[i].X.x1 = population[i].X.x1 + population[i].V.v1;
		if (population[i].X.x1 < (VarX::l1))
			population[i].X.x1 = (VarX::l1);
		if (population[i].X.x1 >(VarX::r1))
			population[i].X.x1 = (VarX::r1);
		population[i].X.x2 = population[i].X.x2 + population[i].V.v2;
		if (population[i].X.x2 < (VarX::l2))
			population[i].X.x2 = (VarX::l2);
		if (population[i].X.x2 >(VarX::r2))
			population[i].X.x2 = (VarX::r2);
	}
}
double PSO::randval(double lval,double rval){
	double val;
	val = lval + (rval - lval)*rand()*1.0 / RAND_MAX;
	return val;
}
void PSO::report() {
	double x1 = GBest.x1;
	double x2 = GBest.x2;
	double fit = GBest.fitval;
	cout << x1 << "  " << x2 << "  " << fit << endl;
}
int main()
{
	srand((unsigned int)(time(NULL)));
	PSO pso;
	pso.init();
	int gen = GEN;
	while (gen--) {
		pso.Update();
		pso.calfit();
		pso.setpbest();
		pso.findgbest();
		pso.report();
	}
    return 0;
}

