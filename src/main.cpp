#include<iostream>
#include"Matrix.h"
#include"Individual.h"
#include<string>
#include<fstream>
#include<cmath>
#include<vector>

using element_type = long double;

constexpr int N_GENERATIONS = 1000;
constexpr int POP_SIZE = 50;
constexpr int DNA_SIZE = 6;
constexpr int EXAMP_NUM = 11;
constexpr int NODE_NUM = DNA_SIZE;
constexpr double PC = 0.8;
constexpr double MC = 0.05;
constexpr double MAX_P = 0.5;
typedef Matrix<element_type> Matrixd;
typedef std::vector<Individual> Pop;

Matrixd read_data(std::string filename, int, int);
Matrixd encode_data(Matrixd &, Matrixd &);
Matrixd decode_data(Matrixd&, Matrixd &);
Pop get_heightest_fitness(Pop &, int n = 1);
Pop select(Pop &, int n = 1);
Individual crossover(Individual, Individual, double);
Individual crossover_by_avar(Individual, Individual, double);
Individual mutation(Individual, double, double);



int main() {
	using namespace std;
	//读取样本�?
	Matrixd origing_data = read_data("resource/data.txt", EXAMP_NUM, NODE_NUM);
	Matrixd max_min_data;
	Matrixd c = encode_data(origing_data, max_min_data);
	//c = encode_data(c);
	std::cout << c << endl;
	//初�?�化种群
	Pop parents_pop;
	Pop sub_pop;
	for (int i = 0; i < POP_SIZE; i++) {
		auto indi = Individual{ DNA_SIZE, DNA_SIZE };
		indi.calculate_fitness_by_matrix(c, DNA_SIZE, EXAMP_NUM);
		parents_pop.push_back(indi);
	}
	for(int t = 0; t < N_GENERATIONS; t++){
		std::cout << "�?" << t+1 << "�?" << endl;
		//最高两�?�?体插入下一代�?�群
		Pop heightest = get_heightest_fitness(parents_pop, 2);
		std::cout << "最高适应度：" << heightest[0].get_fitness() << endl;
		for(int i = 0; i < 2; i++){
			sub_pop.push_back(heightest[i]);
		}
		while(sub_pop.size() < POP_SIZE){
			//选择两个染色�?
			Pop parents = select(parents_pop, 2);
			//交叉
			Individual son = crossover_by_avar(parents[0], parents[1], PC);
			//变异
			son = mutation(son, MC, MAX_P);
			son.calculate_fitness_by_matrix(c, DNA_SIZE, EXAMP_NUM);//计算子代的适应�?
			//子代加入到新种群
			sub_pop.push_back(son);
		}
		//删除上一�?种群
		parents_pop.clear();
		parents_pop = sub_pop;
		sub_pop.clear();
	}
	/*
	* ************************************----test------****************************
	*/
	auto res = get_heightest_fitness(parents_pop)[0];
	cout << "W权值矩阵：" << endl << res << endl;;
	//测试结果
	//输入样本矩阵
	Matrixd exp_test = c.get_sub_matrix(0, 0, EXAMP_NUM - 1, NODE_NUM);
	Matrixd exp = origing_data.get_sub_matrix(1, 0, EXAMP_NUM, NODE_NUM);
	// 获得预测值矩�?
	Matrixd forecast = exp_test * res;
	forecast = decode_data(forecast, max_min_data);
	cout << "预测值矩阵：" << endl << forecast << endl;
	//获得绝�?��??�?和相对�??�?矩阵
	Matrixd abs_err = forecast - exp;
	Matrixd rela_err( 1, forecast.get_row() );
	//计算绝�?��??�?和相对�??�?
	element_type sum;
	for (int row = 0; row < forecast.get_row(); row++) {
		sum = 0;
		for (int line = 0; line < forecast.get_line(); line++) {
			abs_err[row][line] = abs(abs_err[row][line]);
			sum += abs_err[row][line] / exp[row][line] * 100;
		}
		rela_err[0][row] = sum / NODE_NUM;
	}
	cout << "绝�?��??�?�?" << endl << abs_err << endl;
	cout << "相�?��??�?�?" << endl << rela_err << endl;
}



Matrixd read_data(std::string filename, int row, int line){
	//读取样本文件
	using namespace std;
	ifstream ifs;
	ifs.open(filename);
	Matrixd m(row, line);
	if(!ifs.is_open()){
		std::cout << "文件不存�?!";
		exit(0);
	}
	for(int i = 0; i < row; i++){
		for(int j = 0;j < line; j++){
			ifs >> m.at(i, j);
		}
	}
	return m;
}

Matrixd encode_data(Matrixd & matrix, Matrixd & max_min_data){
	//对样�?数据进�?�编�?
	//编码的数�?�?0-1
	const int row = matrix.get_row();
	const int line = matrix.get_line();
	Matrixd c(row, line);
	max_min_data = Matrixd(2, line);
	double max, min, cur;
	//计算一行的最大最小�?
	for(int i = 0; i < line; i++){
		min = max = matrix.at(0, i);
		for(int j = 0; j < row; j++){
			cur = matrix[j][i];
			if(cur > max) max = cur;
			else if(cur < min) min = cur;
		}
		max_min_data[0][i] = max; max_min_data[1][i] = min;
	}
	//编码
	for(int i = 0; i < row; i++){
		for(int j = 0; j < line; j++){
			c.at(i, j) = (matrix.at(i, j) - max_min_data[1][j])/(max_min_data[0][j] - max_min_data[1][j]);
		}
	}
	return c;
}

Matrixd decode_data(Matrixd& matrix, Matrixd& max_min_data) {
	Matrixd c = matrix;
	for (int row = 0; row < matrix.get_row(); row++) {
		for (int line = 0; line < matrix.get_line(); line++) {
			c[row][line] = matrix[row][line] * (max_min_data[0][line] - max_min_data[1][line]) + max_min_data[1][line];
		}
	}
	return c;
}


Pop get_heightest_fitness(Pop & p, int n){
	/*
	* @description: 从p种群�?选出n�?适应度最高的�?�?
	*/
	//先进行排�?
	std::sort(p.begin(), p.end(), [](Individual& indi1, Individual& indi2) {return indi1.get_fitness() > indi2.get_fitness(); });
	//将结果放�?Pop容器�?
	Pop res_pop{};
	for(int i = 0; i < n; i++){
		res_pop.push_back(p[i]);
	}
	return res_pop;
}

Pop select(Pop & p, int n){
	/*
	* @description:�?盘赌选择法，从�?�群p�?选择n�?�?�?
	* @return: Pop
	*/
	using namespace std;
	element_type fitness_sum = 0;
	//计算适应度总和
	for(Individual indi : p)
		fitness_sum += indi.get_fitness();
	//计算每个�?体适应度比�?
	element_type* fitness_rate = new element_type[p.size()] {};
	element_type* rates = new element_type[p.size()] {};
	element_type rate_sum = 0;
	for(int i = 0; i < p.size(); i++) {
		fitness_rate[i] = p[i].get_fitness() / fitness_sum;
		rate_sum += fitness_rate[i];
		rates[i] = rate_sum;
	}
	//开始选择
	Pop res{};
	for(int num = 0; num < n; num++){
		double rate = get_random<double>(0, 1);
		for(int i = 0; i < p.size() - 1; i++){
			if(rate <= rates[0]){
				res.push_back(p[0]);
				break;
			}
			else if(rate >= rates[p.size() - 1]){
				res.push_back(p.size() - 1);
				break;
			}
			if(rate > rates[i] && rate < rates[i + 1]){
				res.push_back(p[i]);
				break;
			}
		}
	}
	return res;
}

Individual crossover(Individual para1, Individual para2, double rate){
	/*
	* @description: 交叉算法，随机选择一�?交叉点，将交叉点后面的DNA相互交换
	* @param: rate: 交叉概率
	*/
	using namespace std;
	if(get_random<double>(0, 1) > rate)return para1;
	int pos_lst[2] = {get_random<int>(0, DNA_SIZE-1), get_random<int>(0, DNA_SIZE-1)};
	for(int i = pos_lst[0]; i < para2.get_row(); i++){
		for(int j = pos_lst[1]; j < para2.get_line(); j++){
			element_type temp = para1.at(i, j);
			para1.at(i, j) = para2.at(i, j);
			para2.at(i, j) = temp;
		}
	}
	return para1;
}

Individual crossover_by_avar(Individual para1, Individual para2, double rate) {
	/*
	* @description: 均匀交叉，遍历para1，每�?节点随机交叉
	* @param: rate: 交叉概率
	*/
	using namespace std;
	if (get_random<double>(0, 1) > rate)return para1;
	for (int i = 0; i < para1.get_row(); i++) {
		for (int j = 0; j < para1.get_line(); j++) {
			if (get_random<int>(0, 1) == 0)continue;
			para1.at(i, j) = para2.at(i, j);
		}
	}
	return para1;
}

Individual mutation(Individual para, double rate, double max_len){
	/*
	* @description: 变异，随机变异某个DNA
	* @param: rate: 变异概率
	*		max_len: 最大变异�?�长
	*/
	using namespace std;
	if(get_random<double>(0, 1) > rate)return para;
	double r = get_random<double>(0, 1);
	int R = get_random<int>(0, 100);
	int pos_lst[2] = {get_random<int>(0, DNA_SIZE-1), get_random<int>(0, DNA_SIZE-1)};
	auto& data = para.at(pos_lst[0], pos_lst[1]);
	element_type add;
	if (R % 2 == 0) {
		add = data + (r - 0.5) * max_len;
		data = (pos_lst[0] == pos_lst[1] && add < 0) ? data : add;
	}
	else {
		add = data - (r - 0.5) * max_len;
		data = (pos_lst[0] == pos_lst[1] && add < 0) ? data : add;
	}
	return para;
}
