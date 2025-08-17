#include "legacy.c"
#include <iostream>
#include <map>

class my_blah {
public:
	explicit my_blah(const int v) : v_{v} {}

private:
	int v_{0};
};

class adapter {
public:
	adapter() {
		for (const auto& v : values){  // const range-based for loop для безопасности
			map_.emplace(v, my_blah(v)); // конструируется на месте
		}
	}

private:
	std::map<int, my_blah> map_;
};

int main() {
	adapter adapter;

	for (const auto& v : values){
		std::cout << v << " ";
	}

	std::cout << "\nAdapter works\n";
}