#include <iostream>
#include <vector>
#include <limits>
#include <omp.h>

struct ReductionResult {
    int min;
    int max;
    int sum;
    double average;
};

ReductionResult parallelReduction(const std::vector<int>& arr) {
    ReductionResult result;
    int n = arr.size();

    result.min = std::numeric_limits<int>::max();
    result.max = std::numeric_limits<int>::min();
    result.sum = 0;

#pragma omp parallel
    {
        int local_min = std::numeric_limits<int>::max();
        int local_max = std::numeric_limits<int>::min();
        int local_sum = 0;

#pragma omp for
        for (int i = 0; i < n; i++) {
            local_min = std::min(local_min, arr[i]);
            local_max = std::max(local_max, arr[i]);
            local_sum += arr[i];
        }

#pragma omp critical
        {
            result.min = std::min(result.min, local_min);
            result.max = std::max(result.max, local_max);
            result.sum += local_sum;
        }
    }

    result.average = static_cast<double>(result.sum) / n;

    return result;
}

int main() {
    std::vector<int> arr = { 9, 3, 5, 1, 7, 2, 8, 6, 4 };

    ReductionResult result = parallelReduction(arr);

    std::cout << "Min: " << result.min << std::endl;
    std::cout << "Max: " << result.max << std::endl;
    std::cout << "Sum: " << result.sum << std::endl;
    std::cout << "Average: " << result.average << std::endl;

    return 0;
}
