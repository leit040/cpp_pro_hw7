#include <iostream>
#include <vector>
#include <future>
#include <random>

std::vector<int> GenerateRandomVector(size_t size, int minValue, int maxValue) {
    std::vector<int> result(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(minValue, maxValue);

    for (size_t i = 0; i < size; ++i) {
        result[i] = dist(gen);
        //result[i] = 1;
    }
    return result;
}

int calculation(int from, int to, const std::vector<int>& v) {
    int result = 0;
    for (int i = from; i < to; ++i) {
        result += v[i];
    }
    return result;
}

int main() {
    std::vector<int> v = GenerateRandomVector(10000, 0, 10000);
    size_t threadCount = 7;
    size_t chunkSize = v.size() / threadCount;
    std::vector<std::future<int>> futures;

    for (size_t x = 0; x < threadCount; ++x) {
        size_t from = x * chunkSize;
        size_t to = (x == threadCount - 1) ? v.size() : from + chunkSize;

        std::cout << "Thread " << x << ": from = " << from << ", to = " << to << "\n";
        futures.push_back(std::async(std::launch::async, [from, to, &v](){
            int result = 0;
            for (int i = from; i < to; ++i) {
                result += v[i];
            }
            return result;
        }));
    }

    int totalResult = 0;
    for (auto& f : futures) {
        totalResult += f.get();
    }
    int res = calculation(0,10000,v);
    std::cout << "Total async result: " << totalResult << "\n";
    std::cout << "Total result: " << res << "\n";

    return 0;
}