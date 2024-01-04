/*
 * Matrix.h
 *
 *  Created on: 2022年10月20日
 *      Author: 10270
 */

#ifndef MATRIX_H_
#define MATRIX_H_
#include<iostream>
#include<stdexcept>
#include<string>
#include<initializer_list>
#include<vector>
#include<iomanip>
#include<algorithm>
using std::string;
using std::vector;


template <class Type>
class Matrix {
private:
	vector<vector<Type>> data;
	int row;
	int line;
public:
	Matrix(int, int);
	Matrix(const std::initializer_list<std::initializer_list<Type>>&);
	Matrix();
	Type& at(int, int);
	int get_row() noexcept { return row; };
	int get_line()noexcept { return line; };
	Matrix get_sub_matrix(int, int, int, int);
	typename vector<vector<Type>>::iterator begin() noexcept { return data.begin(); };
	typename vector<vector<Type>>::iterator end() noexcept { return data.end(); };
	void for_each(void(*func)(Type&));
	Type* get_data() { return data; };
	template<typename func> void transform(func);
	vector<Type>& operator[](int r);
	Matrix operator*(const Matrix&) const;
	Matrix operator+(const Matrix&) const;
	Matrix<Type> operator-(const Matrix<Type>& m) const;

	//friend function
	template<class T>
	friend std::ostream & operator<<(std::ostream &, const Matrix<T> &);

	virtual ~Matrix();
};

template<class Type>
Matrix<Type>::Matrix(int r, int l){
	data = vector<vector<Type>>(r);
	for (vector<Type>& v : data)
		v = vector<Type>(l);
	row = r; line = l;
}

template<class Type>
Matrix<Type>::Matrix(const std::initializer_list<std::initializer_list<Type>> & ini){
	row = ini.size();
	line = (*ini.begin()).size();
	for (int i = 0; i < row; i++) {
		data.push_back(vector<Type> (*(ini.begin() + i)));
	}
}

template<class Type>
Matrix<Type> Matrix<Type>::get_sub_matrix(int row1, int line1, int row2, int line2) {
	/*
	* @description: 获得点[row1, line1]到点[row2, line2)的子矩阵，矩阵下标从0开始
	*				范围“[]”代表能取得，“()”代表不能取得
	* @return Matrix
	*/
	if (row1 >= row2 || line1 >= line2) {
		throw std::domain_error("子矩阵定义域错误");
	}
	Matrix<Type> m(row2 - row1, line2 - line1);
	for (int i = row1; i < row2; i++) {
		for (int j = line1; j < line2; j++)
			m[i - row1][j - line1] = at(i, j);
	}
	return m;
}

template<class Type>
vector<Type>& Matrix<Type>::operator[](int r) {
	if (r >= row || r < 0) {
		string msg{ "访问行" };
		msg = msg + std::to_string(r); msg += "地址越界";
		throw std::out_of_range(msg);
	}
	return data[r];
}

template<class Type>
template<class Func>
void Matrix<Type>::transform(Func func){
	for (auto row = begin(); row != end(); row++) {
		std::transform((*row).begin(), (*row).end(), (*row).begin(), func);
	}
}

template<class Type>
inline void Matrix<Type>::for_each(void(*func)(Type&)) {
	for (auto row = begin(); row != end(); row++) {
		for (auto data_prt = (*row).begin(); data_prt != (*row).end(); data_prt++)
			func(*data_prt);
	}
}


template<class Type>
Matrix<Type>::Matrix() {

}

template<class Type>
Type& Matrix<Type>::at(int r, int l) {
	if (r < 0 || r >= row || l < 0 || l >= line) {
		string msg{ "访问行" };
		msg = msg + std::to_string(r) + "，列" + std::to_string(l) + "地址越界";
		throw std::out_of_range(msg);
	}
	return data[r][l];
}

template<class Type>
Matrix<Type>::~Matrix() {
}


//friend function
template<class Type>
std::ostream& operator<<(std::ostream& os,const Matrix<Type>& m) {
	using namespace std;
	cout.setf(ios_base::left);
	for (auto row : m.data) {
		for (auto i : row)
			cout << setprecision(5) << setw(10) << i;
		cout << endl;
	}
	return os;
}


template<class Type>
Matrix<Type> Matrix<Type>::operator*(const Matrix<Type>& m) const {
	if (line != m.row) {
		throw std::domain_error("前一个矩阵列数应与后一个矩阵行数相同");
	}
	Type sum = 0;
	auto res = Matrix<Type>(row, m.line);
	for (int r = 0; r < res.row; r++) {
		for (int l = 0; l < res.line; l++) {
			//求前一个矩阵第r行与后一个矩阵第l列的总积
			sum = 0;
			for (int i = 0; i < res.line; i++) {
				sum += data[r][i] * m.data[i][l];
			}
			res.at(r, l) = sum;
		}
	}
	return res;
}

template<class Type>
Matrix<Type> Matrix<Type>::operator+(const Matrix<Type>& m) const {
	if (row != m.row || line != m.line) {
		throw std::domain_error("两个矩阵的行列数应该相同");
	}
	auto res = Matrix<Type>(row, line);
	for (int r = 0; r < row; r++) {
		for (int l = 0; l < line; l++) {
			res.at(r, l) = data[r][l] + m.data[r][l];
		}
	}
	return res;
}

template<class Type>
Matrix<Type> Matrix<Type>::operator-(const Matrix<Type>& m) const {
	if (row != m.row || line != m.line) {
		throw std::domain_error("两个矩阵的行列数应该相同");
	}
	auto res = Matrix<Type>(row, line);
	for (int r = 0; r < row; r++) {
		for (int l = 0; l < line; l++) {
			res.at(r, l) = data[r][l] - m.data[r][l];
		}
	}
	return res;
}


#endif /* MATRIX_H_ */