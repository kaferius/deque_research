#include <iostream>
#include <random>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int seed;
    std::cin >> seed;
    std::mt19937 rnd(seed);

    int sz = 0;
    for (int _ = 0; _ < 1'000'000; ++_) {
        if (sz == 0) {
            std::cout << (rnd() & 1) + 1 << ' ' << (int) rnd() << '\n';
            sz++;
        } else {
            int t = (rnd() % 5) + 1;
            std::cout << t << ' ';
            if (t < 3) {
                std::cout << (int) rnd() << '\n';
                sz++;
            } else if (t == 5) {
                std::cout << std::uniform_int_distribution(1, sz)(rnd) << '\n';
            } else {
                sz--;
                std::cout << '\n';
            }
        }
    }
}