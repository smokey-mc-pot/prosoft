#include <memory>
#include <vector>

// правило нуля
class foo {
public:
	explicit foo(const int j) : f_{j} {}

	virtual ~foo() = default;
	foo(const foo&) = default;
	foo(foo&&) = default;
	foo& operator=(const foo&) = default;
	foo& operator=(foo&&) = default;

	virtual std::unique_ptr<foo> clone() const {
		return std::make_unique<foo>(*this);
	}

protected:
	std::size_t size() const noexcept {
		return f_.size();
	}

private:
	std::vector<int> f_;
};

class bar : public foo {
public:
	explicit bar(const int j) : foo(j), b_(j) {}

	~bar() override = default;
	bar(const bar&) = default;
	bar(bar&&) = default;
	bar& operator=(const bar&) = default;
	bar& operator=(bar&&) = default;

	std::unique_ptr<foo> clone() const override {
		return std::make_unique<bar>(*this);
	}

private:
	std::vector<char> b_;
};

int main() {
	const auto f = std::make_unique<foo>(200);
	const auto b = std::make_unique<bar>(400);

	*f = *b;

	const auto b1 = std::make_unique<bar>(200);
	auto f1 = b1->clone();
}
