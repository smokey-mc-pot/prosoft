#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

class codegen_table {
public:
	bool add(const std::string& lang, std::function<void()> action) {
		if (!action) {
			throw std::invalid_argument("Empty action for language: " + lang);
		}

		auto [it, inserted] = table_.try_emplace(lang, std::move(action));
		return inserted;
	}

	void call(const std::string& lang) const {
		const auto it = table_.find(lang);

		if (it == table_.end()) {
			throw std::out_of_range("Language not found: " + lang);
		}

		if (!it->second) {
			throw std::bad_function_call();
		}

		std::invoke(it->second);
	}

private:
	std::unordered_map<std::string, std::function<void()>> table_;
};

std::string gen_cpp(const std::string& left, const int param) {
	return left + std::to_string(param) + ";";
}

auto make_avx512_gen(int& a, int& b) {
	return [&a, &b] {
		constexpr int c = 2;

		std::cout << "vpbroadcastd zmm0, " << a << "\n"
							<< "vpbroadcastd zmm1, " << b << "\n"
							<< "vpaddd zmm2, zmm1, zmm0\n"
							<< "vpbroadcastd zmm3, " << c << "\n"
							<< "vpmulld zmm4, zmm3, zmm2\n";
	};
}

int main() {
	codegen_table reg;

	// можно без функции
	reg.add("java", [] { std::cout << "Java code\n"; });

	// c функцией без захвата но с параметром
	reg.add("cpp", [] { std::cout << gen_cpp("int a = b + ", 4) << "\n"; });

	int a, b;
	std::cout << "Enter two integers a and b: ";
	std::cin >> a >> b;

	// функция с захватом по ссылке
	reg.add("masm", make_avx512_gen(a, b));

	std::cout << "Java generated code: \n";
	reg.call("java");
	std::cout << "\n";

	std::cout << "C++ generated code: \n";
	reg.call("cpp");
	std::cout << "\n";

	std::cout << "Assembly generated code: \n";
	reg.call("masm");
	std::cout << "\n";
}
