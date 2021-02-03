#include <cstdio> // For printf, no cout in multithreaded
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>

using std::mutex;
using std::thread;
using std::vector;

const uint64_t SIZE = 1024 * 1024 * 1024 * 100ULL; // 100 GB!
const uint64_t ROW = 1024 * 1024;
const uint64_t COLUMN = 1024 * 100ULL;

uint8_t *arr{nullptr};
uint8_t *result{0};
mutex myMutex;

vector<double> times;

void doWork(uint8_t threadID)
{
    uint64_t localResult = 0;

    printf("I am thread %u\n", threadID);
    uint64_t startIndex = threadID * SIZE / 2;
    uint64_t endIndex = (threadID + 1) * SIZE / 2;
    for (uint64_t i = startIndex; i < endIndex; i++)
    {
        if (i % (1024 * 1024 * 1024UL) == 0)
        {
            printf("threadID %u:, i is %lu\n", threadID, i);
        }
        localResult += arr[i];
    }
    // MUTEX
    myMutex.lock();
    // Run critical region of code
    result += localResult;
    myMutex.unlock();
}

void multiRowMajor(){
    auto start = std::chrono::high_resolution_clock::now();




    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::nano> running_time = end - start;
    times.push_back(running_time.count());

}

void singleThreadRow()
{
    auto start = std::chrono::high_resolution_clock::now();

    uint64_t massiveArray[ROW][COLUMN];
    for (int r = 0; r < ROW; r++)
    {
        for (int c = 0; c < COLUMN; c++)
        {
            massiveArray[r][c] = 74;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::nano> running_time = end - start;
    times.push_back(running_time.count());
}

void singleThreadColumn()
{
    auto start = std::chrono::high_resolution_clock::now();
    uint64_t massiveArray[ROW][COLUMN];
    for (int c = 0; c < COLUMN; c++)
    {

        for (int r = 0; r < ROW; r++)
        {

            massiveArray[r][c] = 74;
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::nano> running_time = end - start;
    times.push_back(running_time.count());
}

int main()
{
    singleThreadRow();
    singleThreadColumn();

    printf("Task                                Time\n");
    printf("Single thread row major:    %f\n", times[0]);
    printf("Single thread column major: %f\n", times[1]);

    // arr = new uint8_t[SIZE];
    // printf("The item at index 42 is: %d\n", arr[42]);

    // // Create thread tracking objets, these are NOT threads themselves
    // thread *threads = new thread[2];

    // // FORK-JOIN MODEL
    // for (int i = 0; i < 2; i++)
    // {
    //     threads[i] = thread(doWork, i);
    // }
    // for (int i = 0; i < 2; i++)
    // {
    //     threads[i].join();
    // }

    // printf("Master thread, child threads are complete!\n");

    // //uint64_t result = 0;
    // //for (uint64_t i = 0; i < SIZE; i++) {
    // //}
    // delete[] threads;
    // delete[] arr;
    return 0;
}