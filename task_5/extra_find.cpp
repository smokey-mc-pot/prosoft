#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <vector>

int main() {
	constexpr size_t n = 1'111'111;
	constexpr int32_t key = 1408396; // ключ для поиска

	alignas(64) std::vector v(n, 12345); // инициализируем вектор размером n, заполняя его какими то числами, напимер 12345
	v[n - 124] = key;										 // устанавливаем ключ почти в конец вектора чтобы сымитировать случай долгого поиска

	// зесекаем время в миллисекундах
	const auto t0 = std::chrono::high_resolution_clock::now();
	const auto std_idx = std::ranges::find(v.begin(), v.end(), key);
	const auto t1 = std::chrono::high_resolution_clock::now();

	const size_t std_idx_val = (std_idx == v.end()) ? SIZE_MAX : static_cast<size_t>(std::distance(v.begin(), std_idx));

	std::cout << "std::ranges::find: idx = " << std_idx_val << " time = " << std::chrono::duration<double, std::milli>(t1 - t0).count() << " ms\n";

	return 0;
}
