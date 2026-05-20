#include <raytracer.h>

#include <algorithm>
#include <vector>
#include <iostream>
#include <iomanip>
#include <mathutils.h>

struct Sample {
    double x;
    double p_x;
};

bool compare_by_x(const Sample& a, const Sample& b) {
    return a.x < b.x;
}

void run_halfway_estimate() {
    const unsigned int N = 10000;
    Sample samples[N];
    double sum = 0.0;

    // Iterate through all samples
    for (unsigned int i = 0; i < N; i++) {
        // Area under curve
        auto x = random_double(0, 2 * pi);
        auto sin_x = std::sin(x);
        auto p_x = exp(-x / (2 * pi)) * sin_x * sin_x;
        sum += p_x;

        // Store
        Sample this_sample = {x, p_x};
        samples[i] = this_sample;
    }

    std::sort(std::begin(samples), std::end(samples), compare_by_x);

    // Find out sample at which we have half our area
    double half_sum = sum / 2.0;
    double halfway_point = 0.0;
    double accum = 0.0;
    for (unsigned int i = 0; i < N; i++) {
        accum += samples[i].p_x;
        if (accum >= half_sum) {
            halfway_point = samples[i].x;
            break;
        }
    }

    std::cout << std::fixed << std::setprecision(12);
    std::cout << "Average = " << sum / N << '\n';
    std::cout << "Area under curve = " << 2 * pi * sum / N << '\n';
    std::cout << "Halfway = " << halfway_point << '\n';
}