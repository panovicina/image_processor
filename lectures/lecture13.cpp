#include <exception>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

using Operation = int(int, int);

template <typename T>
void Print(const std::vector<T>& vector) {
    for (auto v: vector) {
        std::cout << v << " ";
    }
    std::cout << std::endl;
}

int Sum(int a, int b) {
    return a + b;
}

int Sub(int a, int b) {
    return a - b;
}

struct Foo {
    int Div(int a, int b) const {
        return a / b;
    }

    int Do(int a, int b) const {
        return [&](int a, int b) { return this->Div(a, b); }(a, b);
    }

    int x = 100;
};

template <typename Op, typename T>
auto ZipMap(const std::vector<T>& a, const std::vector<T>& b) {
    if (a.size() != b.size()) {
        throw std::invalid_argument("a.size != b.size");
    }

    Op op;
    std::vector<decltype(op(a[0], b[0]))> result;

    for (size_t i = 0; i < a.size(); ++i) {
        result.push_back(op(a[i], b[i]));
    }

    return result;
}

class Division {
public:
    template<typename T1, typename T2>
    auto operator()(T1 a, T2 b) const {
        return a / b;
    }
};

using namespace std::placeholders;

int main() {
    Foo foo;
    auto method = std::mem_fn(&Foo::Do);
    std::cout << method(foo, 10, 20) << std::endl;

    auto method2 = std::bind(&Foo::Do, &foo, _1, _2);
    std::cout << method2(10000, 10) << std::endl;

    int x = 100;
    auto div = [=, &foo](auto a, auto b) { return a / b + x; };
    std::cout << div(10, 3) << std::endl;
    std::cout << div(10.0, 3.0) << std::endl;
    std::cout << div(10.0, 3) << std::endl;

    std::vector<double> a = {1, 2, 3};
    std::vector<double> b = {10, 20, 30};

    Print(ZipMap<Division>(a, b));

    // Print(ZipMap(a, b, Sub));
    // Print(ZipMap(a, b, div));
    // Print(ZipMap(a, b, Foo::Do));

    return 0;
}
