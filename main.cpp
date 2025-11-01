#include <algorithm>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

using std::vector;

const int N = 9000;
const int M = 20;
vector<vector<int>> comb(M, vector<int>(M, 2));

void multiply_block(vector<vector<int>> &matrix, int start_row, int end_row) {
    for (int i = start_row; i < end_row; ++i) {
        for (int j = 0; j < N; ++j) {
            int s = 0;
            for (int k = 0; k < M; ++k) {
                for (int l = 0; l < M; ++l) {
                    s += comb[k][l];
                }
            }
            matrix[i][j] = s;
        }
    }
}

int main() {
    unsigned int n_in;
    std::cout << "Input the number of threads to be used: ";
    std::cin >> n_in;

    vector<vector<int>> v(N, vector<int>(N));

    // Start timing
    auto start = std::chrono::high_resolution_clock::now();

    unsigned int nthread = std::min(std::thread::hardware_concurrency(), n_in);
    std::cout << "Using " << nthread << " threads" << std::endl;

    int rows_per_thread = (N + nthread - 1) / nthread;
    vector<std::thread> threads;
    int start_row = 0;

    for (unsigned int t = 0; t < nthread && start_row < N; ++t) {
        int end_row = std::min(start_row + rows_per_thread, N);
        threads.emplace_back(multiply_block, std::ref(v), start_row, end_row);
        start_row = end_row;
    }

    for (auto& th : threads)
        th.join();

    // Stop timing
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << duration.count() << " ms" << std::endl;

    return 0;
}
