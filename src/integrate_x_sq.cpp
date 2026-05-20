#include <raytracer.h>

#include <iostream>
#include <iomanip>

// Crude approximation to nonuniform probabilistic density function
double f(double d) {
    if (d <= 0.5) {
        return std::sqrt(2.0) * random_double();
    } else {
        return std::sqrt(2.0) + (2 - std::sqrt(2.0)) * random_double();
    }
}

double icd(double d) {
    return 8.0 * std::pow(d, 1.0 / 3.0);
}

double pdf(double x) {
    return (3.0 / 8.0) * x * x;
}

void run_integral() {
    int N = 1;
    auto sum = 0.0;

    for (int i = 0; i < N; i++) {
        auto z = random_double();
        if (z == 0.0) { // avoid NaNs
            continue;
        }
        auto x = icd(z);
        sum += x * x / pdf(x);
    }

    std::cout << std::fixed << std::setprecision(12);
    std::cout << "I = " << (sum / N) << '\n';

}
