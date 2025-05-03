#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <vector>
#include <iomanip>
#include <random>
#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

mutex cout_mutex;

#ifdef _WIN32
void setCursorPosition(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
#else
void setCursorPosition(int x, int y) {
    cout << "\033[" << y << ";" << x << "H";
}
#endif

void draw(int threadId, int calculationLength, int totalThreads) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(10, 100);

    auto startTime = chrono::high_resolution_clock::now();

    for (int i = 0; i < calculationLength; ++i) {
        this_thread::sleep_for(chrono::milliseconds(distrib(gen)));

        {
            lock_guard<mutex> lock(cout_mutex);
            setCursorPosition(i, threadId);
            cout << "\u2588";
            cout.flush(); 
        }

        this_thread::yield();
    }

    auto endTime = chrono::high_resolution_clock::now();
    auto duration = chrono::duration<double>(endTime - startTime).count();

    {
        lock_guard<mutex> lock(cout_mutex);
        setCursorPosition(0, threadId); 
        cout << "Поток " << threadId << ": Завершено за " << fixed << setprecision(2) << duration << " сек" << endl;
        cout.flush();
    }
}

int main() {
    setlocale(LC_ALL, "Russian");

#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    for (int i = 0; i < consoleHeight; ++i) {
        setCursorPosition(0, i);
        for (int j = 0; j < consoleWidth; ++j) {
            cout << " ";
        }
    }
    setCursorPosition(0, 0);
#else
    cout << "\033[2J\033[H"; 
#endif

    int numThreads;
    int calculationLength;

    cout << "Введите количество потоков: ";
    cin >> numThreads;
    cout << "Введите длину расчета: ";
    cin >> calculationLength;

    vector<thread> threads;
    for (int i = 0; i < numThreads; ++i) {
        threads.push_back(thread(draw, i + 1, calculationLength, numThreads));
    }

    for (auto& thread : threads) {
        thread.join();
    }

    cout << endl;
    return 0;
}

