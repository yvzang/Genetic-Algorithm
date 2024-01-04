/*
 * Individual.cpp
 *
 *  Created on: 2022年10月20日
 *      Author: 10270
 */

#include "Individual.h"
#include<fstream>
#include<random>
#include<ctime>
#include<functional>

typedef Matrix<long double> Matrixd;
using element_type = long double;



Individual::Individual(int r, int l):Matrixd(r, l){
	for(int i = 0; i < r; i++){
		for(int j = 0; j < l; j++){
			at(i, j) = (i == j) ? get_random<element_type>(0, 1): get_random<element_type>(-1, 1);
		}
	}
	fitness = 0;
}


element_type Individual::calculate_fitness(Matrixd & c, int n, int m){
	/*
	c: 样本数据
	n: 节点数目
	m: 样本数目
	*/
	fitness = 1 / (calculate_err(c, n, m) + 1);
	return fitness;
}

element_type Individual::calculate_fitness_by_matrix(Matrixd& c, int node_size, int examp_size) {
	/*
	* @description: 用矩阵的方法计算适应度
	* @return: fitness
	*/
	//先得到左乘的矩阵
	Matrixd c1 = c.get_sub_matrix(0, 0, examp_size - 1, node_size);
	//得到cba矩阵
	Matrixd cba = c1 * (Matrixd)(*this);
	//激活函数
	std::function<element_type(element_type, element_type)> fc = &Individual::F;
	cba.transform(std::bind1st(fc, 1.5));
	//得到实际样本的矩阵
	Matrixd exp_matrix = c.get_sub_matrix(1, 0, examp_size, node_size);
	//计算误差矩阵
	Matrixd err_matrix = cba - exp_matrix;
	//计算err
	element_type sum = 0;
	for (auto row = err_matrix.begin(); row != err_matrix.end(); row++) {
		for (auto data_prt = (*row).begin(); data_prt != (*row).end(); data_prt++) {
			sum += pow(*data_prt, 2);
		}
	}
	element_type err = sum / (node_size * (examp_size - 1));
	fitness = 1 / (err + 1);
	return fitness;
}

element_type Individual::calculate_cba(Matrixd & c, int i, int t){
	//预测t+1时刻第i个节点的值
	//c: 样本数据
	//i: 染色体列号
	//t: 上一个时间序列号
	element_type sum = 0;
	for(int j = 0; j < get_line(); j++){
		sum += at(j, i) * c.at(t, j);
	}
	return sum;
}

element_type Individual::calculate_err(Matrixd & c, int n, int m){
	//c: 样本数据
	//n: 节点数目
	//m: 样本数目
	element_type sum_m = 0, sum_n = 0;
	for(int t = 1; t < m; t++){
		sum_n = 0;
		for(int i = 0; i < n; i++){
			sum_n += pow(calculate_cba(c, i, t-1) - c.at(t, i), 2);
		}
		sum_m += sum_n;
	}
	return sum_m / (n * m);
}

//operator
bool Individual::operator<(const Individual& indi) const {
	if (fitness < indi.fitness)return true;
	else return false;
}

//friend function
std::ostream & operator<<(std::ostream & os, const Individual & indi){
	std::cout << (Matrixd)indi;
	os << "fitness: " << indi.fitness << std::endl;
	return os;
}


Individual::~Individual() {
	// TODO Auto-generated destructor stub
}

element_type Individual::F(element_type x, element_type gama){
	return (std::exp(2 * x * gama) - 1) / (std::exp(2 * x * gama) + 1);
}
