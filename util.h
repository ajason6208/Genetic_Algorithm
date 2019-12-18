#pragma once

#ifndef UTIL_H_
#define UTIL_H_

inline double distanceBetweenTwoPoints(double x, double y, double a, double b) {
	return sqrt(pow(x - y, 2.0) + pow(a - b, 2.0));
}

template <class T1, class T2, class Pred = std::less<T2> >
struct sort_pair_second_less {
	bool operator()(const std::pair<T1, T2>&left, const std::pair<T1, T2>&right) {
		Pred p;
		return p(left.second, right.second);
	}
};

template <class T1, class T2, class Pred = std::greater<T2> >
struct sort_pair_second_greater {
	bool operator()(const std::pair<T1, T2>&left, const std::pair<T1, T2>&right) {
		Pred p;
		return p(left.second, right.second);
	}
};

/*
std::map<char,int> x = { { 'a',1 },{ 'b',2 },{'c',0}};
auto max=get_max(x);
std::cout << max.first << "=>" << max.second << std::endl;
Outputs: b=>2
*/
template<typename KeyType, typename ValueType>
std::pair<KeyType, ValueType> get_max(const std::map<KeyType, ValueType>& x) {
	using pairtype = std::pair<KeyType, ValueType>;
	return *std::max_element(x.begin(), x.end(), [](const pairtype & p1, const pairtype & p2) {
		return p1.second < p2.second;
	});
}

#endif