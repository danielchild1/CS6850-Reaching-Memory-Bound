#include <cstdio> // For printf, no cout in multithreaded
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>

using std::mutex;
using std::thread;
using std::vector;

const uint64_t SIZE = 1024 * 1024;// * 1024 * 100ULL; // 100 GB!
unsigned int threadsSupported = std::thread::hardware_concurrency();

uint8_t *arr{nullptr};
uint8_t *result{0};
mutex myMutex;
vector<double> times;


void rowMajorWork(uint8_t threadID)
{
    uint64_t localResult = 0;

    printf("I am thread %u\n", threadID);
    uint64_t startIndex = threadID * SIZE / threadsSupported;
    uint64_t endIndex = (threadID + 1) * SIZE / threadsSupported;
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

void columnMajorWork(uint64_t threadID){
    uint64_t const numColumns = SIZE/threadsSupported;
    uint64_t localResult = 0;

    printf("I am thread %lu\n", threadID);
    for (uint64_t i = threadID; i < SIZE; i+= numColumns)
    {
        if (i % (1024 * 1024 * 1024UL) == 0)
        {
            printf("threadID %lu:, i is %lu\n", threadID, i);
        }
        localResult += arr[i];
    }
    // MUTEX
    myMutex.lock();
    // Run critical region of code
    result += localResult;
    myMutex.unlock();

}

void multiRowMajor(thread *threads)
{
    auto start = std::chrono::high_resolution_clock::now();


    // // FORK-JOIN MODEL
    for (int i = 0; i < threadsSupported; i++)
    {
        threads[i] = thread(rowMajorWork, i);
    }
    for (int i = 0; i < threadsSupported; i++)
    {
        threads[i].join();
    }


    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::nano> running_time = end - start;
    times.push_back(running_time.count());
}

void multiColumnMajor(thread *threads){
     auto start = std::chrono::high_resolution_clock::now();


    // // FORK-JOIN MODEL
    for (int i = 0; i < threadsSupported; i++)
    {
        threads[i] = thread(columnMajorWork, i);
    }
    for (int i = 0; i < threadsSupported; i++)
    {
        threads[i].join();
    }


    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::nano> running_time = end - start;
    times.push_back(running_time.count());
}

void singleThreadRow()
{
    auto start = std::chrono::high_resolution_clock::now();

    uint64_t localResult = 0;

    for (uint64_t r = 0; r < SIZE; r++)
    {
        if (r % (1024 * 1024 * 1024UL) == 0)
        {
            printf("Row major: i is %lu\n", r);
        }
        localResult += arr[r];
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::nano> running_time = end - start;
    times.push_back(running_time.count());
}

void singleThreadColumn()
{
    auto start = std::chrono::high_resolution_clock::now();
    uint64_t localResult = 0;

    for (int c = 0; c < SIZE; c++)
    {

        for (uint64_t r = 0 + c; r < SIZE; r += 10)
        {
            if (r % (1024 * 1024 * 1024UL) == 0)
            {
                printf("Column major: i is %lu\n", r);
            }
            localResult += arr[r];
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::nano> running_time = end - start;
    times.push_back(running_time.count());
}

int main()
{
    printf("This machine has %d cores.\n", threadsSupported);

    arr = new uint8_t[SIZE];
    printf("The item at index 42 is: %d\n", arr[42]);


    // Create thread tracking objets, these are NOT threads themselves
    thread *threads = new thread[threadsSupported];

    singleThreadRow();
    singleThreadColumn();
    multiRowMajor(threads);
    multiColumnMajor(threads);







    printf("Task                                Time\n");
    printf("Single thread row major:    %f\n", times[0]);
    printf("Single thread column major: %f\n", times[1]);
    printf("Multi-threaded row major:   %f\n", times[2]);
    printf("Multi-threaded column major: %f\n", times[4]);


    

    // printf("Master thread, child threads are complete!\n");

    // //uint64_t result = 0;
    // //for (uint64_t i = 0; i < SIZE; i++) {
    // //}
    delete[] threads;
    delete[] arr;
    return 0;
}