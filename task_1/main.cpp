#include <algorithm>

class foo {
public:
	explicit foo(const std::size_t j) : f_{new int[j]}, size_{j} {} // explicit запретит неявные преобразования типов

	foo(const foo& right) : f_{new int[right.size_]}, size_{right.size_} { // копирующий конструктор
		std::copy_n(right.f_, size_, f_);
	}

	foo(foo&& right) noexcept : f_{right.f_}, size_{right.size_} { // noexcept для перемещения, а не копирования контейнеров
		right.f_ = nullptr;
		right.size_ = 0;
	}

	foo& operator=(const foo& right) { // оператор присваивания
		if (this == &right) {
			return *this;
		}

		const auto tmp{new int[right.size_]};
		std::copy_n(right.f_, right.size_, tmp);
		delete[] f_;

		f_ = tmp;
		size_ = right.size_;

		return *this;
	}

	foo& operator=(foo&& right) noexcept { // оператор перемещения
		if (this == &right) {
			return *this;
		}

		delete[] f_;

		f_ = right.f_;
		size_ = right.size_;
		right.f_ = nullptr;
		right.size_ = 0;

		return *this;
	}

	virtual ~foo() { // виртуальный деструктор
		delete[] f_;
	}

	[[nodiscard]] virtual foo* copy() const { // const здесь и далее если данные не должны маняться
		return new foo(*this);
	}

protected: // protected чтобы bar получил доступ к методу и знал размер массива для объекта foo
	std::size_t size() const noexcept {
		return size_;
	}

private:
	int* f_{nullptr};
	std::size_t size_{0};
};

class bar : public foo {
public:
	explicit bar(const std::size_t j) : foo{j}, b_{new char[j]} {}

	bar(const bar& r) : foo(r), b_{new char[size()]} {
		std::copy_n(r.b_, size(), b_);
	}

	bar(bar&& right) noexcept : foo(std::move(right)), b_{right.b_} { // std::move для выбора конструктора перемещения
		right.b_ = nullptr;
	}

	bar& operator=(const bar& right) {
		if (this == &right) {
			return *this;
		}

		foo::operator=(right); // вызваем у базовой части текущего объекта перемещающий оператор присваивания foo
		const auto tmp = new char[size()];
		std::copy_n(right.b_, size(), tmp);
		delete[] b_;

		b_ = tmp;
		return *this;
	}

	bar& operator=(bar&& right) noexcept {
		if (this == &right) {
			return *this;
		}

		foo::operator=(std::move(right));
		delete[] b_;

		b_ = right.b_;
		right.b_ = nullptr;

		return *this;
	}

	~bar() override { // override для проверки что действительно переопределили
		delete[] b_;
	}

	foo* copy() const override {
		return new bar(*this);
	}

private:
	char* b_{nullptr};
};

int main() {
	const auto f = new foo(100);
	const auto b = new bar(200);

	*f = *b;

	delete f;
	delete b;

	const auto b1 = new bar(200);
	const auto f1 = b1->copy();

	delete f1;
	delete b1;
}
