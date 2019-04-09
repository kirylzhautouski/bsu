#include <iostream>
#include <array>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>

using namespace std;

const unsigned M = 2147483648; // 2 ^ 31
const unsigned a0 = 24389; // 24389
const unsigned b = 24389; // 24389

const unsigned K = 32; // 32

const unsigned n = 1000; // 1000

template<typename T>
void PrintArray(const array<T, n>& a) {
	for (auto i : a)
		cout << i << " ";
}

array<double, n> GenerateRandomCStyle() {
	array<double, n> result;

	srand(static_cast<unsigned>(time(NULL)));
	for (int i = 0; i < n; i++)
		result[i] = static_cast<double>(rand()) / (RAND_MAX + 1);

	return result;
}

array<double, n> GenerateRandom() {
	array<double, n> result;

	random_device rd;
	mt19937 gen(rd());
	for (unsigned i = 0; i < n; i++)
		result[i] = generate_canonical<double, 10>(gen);

	return result;
}

array<double, n> MCG() {
	array<double, n> seq;

	unsigned tmp = a0;

	for (unsigned i = 0; i < n; i++) {
		tmp = (b * tmp) % M;
		seq[i] = static_cast<double>(tmp) / M;
	}

	return seq;
}

array<double, n> GMM() {
	array<double, n> seq;

	array<double, n> mcg_seq = MCG();
	array<double, n> std_seq = GenerateRandomCStyle();

	array<double, K> v;
	for (unsigned i = 0; i < K; i++)
		v[i] = std_seq[i];

	for (unsigned i = 0; i < n; i++) {
		auto s = static_cast<int>(mcg_seq[i] * K);
		seq[i] = v[s];
		v[s] = std_seq[(K + i) % n];
	}

	return seq;
}

double KolmogorovD(array<double, n> seq) {
	sort(seq.begin(), seq.end());

	double D = 0;

	for (unsigned i = 0; i < n; i++) {
		auto tmp = fabs((static_cast<double>(i + 1) / n - seq[i]));

		if (D < tmp)
			D = tmp;
	}

	return D;
}

double PirsonXi2(const array<double, n>& seq, unsigned k) {
	double xi2 = 0;
	vector<unsigned> count(k, 0);

	for (unsigned i = 0; i < n; i++)
		count[static_cast<int>(seq[i] * k)]++;

	for (unsigned i = 0; i < k; i++)
		xi2 += pow(count[i] - static_cast<double>(n) / k, 2) / (static_cast<double>(n) / k);

	return xi2;
}

int main() {
	array<double, n> mcg_sequence = MCG();
	//PrintArray(mcg_sequence);

	array<double, n> gmm_sequence = GMM();
	//PrintArray(gmm_sequence);

	cout << "mcg: sqrt(n) * D = " << sqrt(n) * KolmogorovD(mcg_sequence) << endl;
	cout << "gmm: sqrt(n) * D = " << sqrt(n) * KolmogorovD(gmm_sequence) << endl;

	cout << "mcg: xi2 = " << PirsonXi2(mcg_sequence, 10) << endl;
	cout << "gmm: xi2 = " << PirsonXi2(gmm_sequence, 10) << endl;

	system("pause");
	return 0;
}