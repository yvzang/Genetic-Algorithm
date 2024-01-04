/*
 * Individual.h
 *
 *  Created on: 2022年10月20日
 *      Author: 10270
 */

#ifndef INDIVIDUAL_H_
#define INDIVIDUAL_H_
#include"Matrix.h"
#include<cmath>
#include<random>

typedef Matrix<long double> Matrixd;
using element_type = long double;

template<typename T>
T get_random(T low, T height) {
	using namespace std;
	static default_random_engine e(time(0));
	uniform_real_distribution<T> u(low, height);
	return u(e);
}

template<> inline int get_random(int low, int height) {
	using namespace std;
	static default_random_engine ei(time(0));
	uniform_int_distribution<int> u(low, height);
	return u(ei);
}


class Individual: public Matrix<element_type> {
private:
	double fitness;
public:
	Individual(int i=6, int j=6);
	element_type get_fitness(){return fitness;};
	static element_type F(element_type x, element_type gama);
	element_type calculate_fitness(Matrixd & c, int n, int m);
	element_type calculate_fitness_by_matrix(Matrixd& c, int node_size, int examp_size);
	element_type calculate_cba(Matrixd & c, int i, int t);
	element_type calculate_err(Matrixd & c, int n, int m);
	//operator
	bool operator<(const Individual&) const;
	//friend function
	friend std::ostream & operator<<(std::ostream &, const Individual &);
	virtual ~Individual();
};

#endif /* INDIVIDUAL_H_ */
