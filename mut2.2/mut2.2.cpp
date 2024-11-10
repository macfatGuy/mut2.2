#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <vector>
#include <iomanip> 

using namespace std;


void calculateThread(int threadId, int calculationLength) {
 
    thread::id tid = this_thread::get_id();

   
    auto startTime = chrono::high_resolution_clock::now();

    
    for (int i = 0; i < calculationLength; ++i) {
      
        this_thread::sleep_for(chrono::milliseconds(50));

     
        cout << "Поток " << threadId << " (" << tid << "): ";
        for (int j = 0; j <= i; ++j) { 
            cout << "=";
        }
        for (int j = i + 1; j < calculationLength; ++j) { 
            cout << "-";
        }
        cout << " " << fixed << setprecision(2) << (i + 1) * 100.0 / calculationLength << "%"; 
        cout.flush(); 

       
        cout << "\r";
    }

  
    auto endTime = chrono::high_resolution_clock::now();
    auto duration = chrono::duration<double>(endTime - startTime).count();

   
    cout << "Поток " << threadId << " (" << tid << "): " << endl;
    for (int j = 0; j < calculationLength; ++j) { 
        cout << "=";
    }
    cout << " " << fixed << duration << " сек" << endl;
}

int main() {

    setlocale(LC_ALL, "Russian");
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

