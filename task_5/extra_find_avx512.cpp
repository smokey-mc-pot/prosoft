#include <chrono>
#include <cstdint>
#include <cstdio>
#include <immintrin.h>
#include <iostream>
#include <vector>

int main() {
	constexpr size_t n = 11'111'111'111;
	constexpr int32_t key = 1408396; // ключ для поиска

	alignas(64) std::vector v(n, 12345); // инициализируем вектор размером n, заполняя его какими то числами, напимер 12345
	v[n - 124] = key;										 // устанавливаем ключ почти в конец вектора чтобы сымитировать случай долгого поиска

	constexpr size_t num_of_el = 16; // так как решил показать для 32 битных чисел, то будем грузить по 16 в 512 битные регистры
	const int32_t* src = v.data();
	const size_t length = v.size();
	const int32_t* b_ptr = src;										// указатель на начало исходных данных
	const int32_t* e_ptr = src + length;					// указатель на конец исходных данных
	const __m512i v_key = _mm512_set1_epi32(key); // устанавливаем key в каждые 32 бита регистра

	const auto t0 = std::chrono::high_resolution_clock::now();

	// первая условная итерация, будем грузить по 4 блока по 16 элементов в каждом, так выгоднее для кэша и механизма предикта ветвлений
	for (constexpr size_t unroll = 4; b_ptr + unroll * num_of_el <= e_ptr; b_ptr += unroll * num_of_el) {
		// заранее подгружаем в кэш L1 через префетчинг агрессивно с опцией _MM_HINT_T0
		// кэш линия на x86-64 как правило составляет 64 байта, но можно проверить через cpuid
		/*
		unsigned cash_line_check() {
			unsigned eax, ebx, ecx, edx;
			__cpuid_count(4, 0, eax, ebx, ecx, edx);
			unsigned line_size = (ebx & 0xFFF) + 1;
			return line_size;
		}
		*/

		_mm_prefetch(reinterpret_cast<const char*>(b_ptr + unroll * num_of_el), _MM_HINT_T0);

		const __m512i v0 = _mm512_loadu_si512(b_ptr);
		const __m512i v1 = _mm512_loadu_si512(b_ptr + num_of_el);
		const __m512i v2 = _mm512_loadu_si512(b_ptr + 2 * num_of_el);
		const __m512i v3 = _mm512_loadu_si512(b_ptr + 3 * num_of_el);

		// сравниваем каждый из 4 загруженных блоков с ключом
		const __mmask16 m0 = _mm512_cmpeq_epi32_mask(v0, v_key);
		const __mmask16 m1 = _mm512_cmpeq_epi32_mask(v1, v_key);
		const __mmask16 m2 = _mm512_cmpeq_epi32_mask(v2, v_key);
		const __mmask16 m3 = _mm512_cmpeq_epi32_mask(v3, v_key);

		// склеиваем получившиеся маски в 1 64 битный "скорее всего" регистр
		const uint64_t mask_for_4 = static_cast<uint64_t>(m0) |
																static_cast<uint64_t>(m1) << 16 |
																static_cast<uint64_t>(m2) << 32 |
																static_cast<uint64_t>(m3) << 48;

		// если мы нашли совпадение в одном из 4 блоков, то определяем в каком именно и на какой позиции
		if (mask_for_4) {																			// идем сюда если маска не нулевая
			const unsigned block = __builtin_ctzll(mask_for_4); // возвращает число нулей с младшего бита
			const unsigned block_number = block >> 4;						// даст номер блока
			const unsigned lane = block & 15;										// даст позицию в блоке
																													// сдвигаем до текущей партии блоков, прибавляем номар блока и умножаем на количество элементов нем и добавляем позиуию
			size_t avx512_idx = b_ptr - src + block_number * num_of_el + lane;
			const auto t1 = std::chrono::high_resolution_clock::now();
			std::cout << "avx512_find: idx = " << avx512_idx << " time = " << std::chrono::duration<double, std::milli>(t1 - t0).count() << " ms\n";
			return 0;
		}
	}

	// аналогично тому что выше, но уже без анроллинга, шаг покороче, 2я цсловная итерация циклов
	for (; b_ptr + num_of_el <= e_ptr; b_ptr += num_of_el) {
		const __m512i v5 = _mm512_loadu_si512(b_ptr);

		if (const __mmask16 mask_last = _mm512_cmpeq_epi32_mask(v5, v_key)) {
			size_t avx512_idx = b_ptr - src + __builtin_ctz(mask_last);
			const auto t1 = std::chrono::high_resolution_clock::now();
			std::cout << "avx512_find: idx = " << avx512_idx << " time = " << std::chrono::duration<double, std::milli>(t1 - t0).count() << " ms\n";
			return 0;
		}
	}

	// 3я условная итерация, когда остались менее 16 элементов, так называемый хвост
	if (const size_t rem = e_ptr - b_ptr) {
		// (1u << rem) просто установит бит на "нужную" позицию, вычитая 1 получим единицы в млажших бита, их кол-во = rem
		const __mmask16 tail = static_cast<__mmask16>((1u << rem) - 1u); // явно юеззнаковый int
		const __m512i v6 = _mm512_maskz_loadu_epi32(tail, b_ptr);

		if (const __mmask16 m = _mm512_mask_cmpeq_epi32_mask(tail, v6, v_key)) {
			size_t avx512_idx = b_ptr - src + __builtin_ctz(m);
			const auto t1 = std::chrono::high_resolution_clock::now();
			std::cout << "avx512_find: idx = " << avx512_idx << " time = " << std::chrono::duration<double, std::milli>(t1 - t0).count() << " ms\n";
			return 0;
		}
	}

	const auto t1 = std::chrono::high_resolution_clock::now();
	std::cout << "avx512_find: not found, time = " << std::chrono::duration<double, std::milli>(t1 - t0).count() << " ms\n";

	return 0;
}
