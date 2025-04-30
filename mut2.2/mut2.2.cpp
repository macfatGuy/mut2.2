#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <vector>
#include <iomanip>
#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

mutex cout_mutex;

void calculateThread(int threadId, int calculationLength) {
    thread::id tid = this_thread::get_id();
    auto startTime = chrono::high_resolution_clock::now();
    int barWidth = 50;
    const string block = "\u2588"; 

    for (int i = 0; i < calculationLength; ++i) {
        this_thread::sleep_for(chrono::milliseconds(50));

        lock_guard<mutex> lock(cout_mutex);
        cout << "Поток " << threadId << " (" << tid << "): [";

        double progress = static_cast<double>(i + 1) / calculationLength;  
        int pos = static_cast<int>(barWidth * progress);                

        for (int j = 0; j < barWidth; ++j) {
            if (j < pos) cout << block; 
            else if (j == pos && pos < barWidth) cout << ">";  
            else cout << " ";
        }

        cout << "] " << fixed << setprecision(2) << progress * 100.0 << "%\r";
        cout.flush(); 
    }

    auto endTime = chrono::high_resolution_clock::now();
    auto duration = chrono::duration<double>(endTime - startTime).count();

    lock_guard<mutex> lock(cout_mutex);

    cout << "Поток " << threadId << " (" << tid << "): [";
    for (int j = 0; j < barWidth; ++j)
        cout << block;

    cout << "] 100.00%\r";
    cout.flush();


    cout << "Поток " << threadId << " (" << tid << "): Завершено за " << fixed << duration << " сек" << endl;
}


int main() {
    setlocale(LC_ALL, "Russian");
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    int numThreads;
    int calculationLength;

    cout << "Введите количество потоков: ";
    cin >> numThreads;
    cout << "Введите длину расчета: ";
    cin >> calculationLength;

    vector<thread> threads;
    for (int i = 0; i < numThreads; ++i) {
        threads.push_back(thread(calculateThread, i + 1, calculationLength));
    }

    for (auto& thread : threads) {
        thread.join();
    }

    return 0;
}

