#include <iostream>
#include <random>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int seed;
    std::cin >> seed;
    std::mt19937 rnd(seed);

    for (int _ = 0; _ < 500'000; _ += 2) {
        std::cout << 1 << ' ' << (int) rnd() << '\n';
        std::cout << 2 << ' ' << (int) rnd() << '\n';
    }
    for (int _ = 0; _ < 500'000; ++_) {
        std::cout << 5 << ' ' << std::uniform_int_distribution(1, 500'000)(rnd) << '\n';
    }
}