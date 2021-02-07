#include <cstdio> // For printf, no cout in multithreaded
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>

using std::mutex;
using std::thread;
using std::vector;

const uint64_t SIZE = 1024 * 1024; // * 1024 * 100ULL; // 100 GB!
unsigned int threadsSupported = std::thread::hardware_concurrency();

uint8_t *arr{nullptr};
uint8_t *result{0};
mutex myMutex;
vector<double> times;
unsigned int workerNumber = 0;

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

void workForCrew(uint8_t threadID)
{
    while (workerNumber < threadsSupported * 10)
    {
        uint64_t startIndex, endIndex, localResult = 0;
        unsigned int localWorkerNumber = workerNumber;

        myMutex.lock();
        startIndex = workerNumber * SIZE / threadsSupported * 10;
        endIndex = (workerNumber + 1) * SIZE / threadsSupported * 10;
        workerNumber++;
        myMutex.unlock();

        if (localWorkerNumber < threadsSupported * 10)
        {
            for (uint64_t i = startIndex; i < endIndex; i++)
            {
                if (i % (1024 * 1024UL) == 0)
                {
                    printf("threadID %u:, i is %lu\n", threadID, i);
                }
                localResult += arr[i];
            }
        }
        else
        {
            break;
        }
    }
}

void workForCrewCasting(uint8_t threadID){
    while (workerNumber < threadsSupported * 10)
    {
        uint64_t startIndex, endIndex, localResult = 0;
        unsigned int localWorkerNumber = workerNumber;

        myMutex.lock();
        startIndex = workerNumber * SIZE / threadsSupported * 10;
        endIndex = (workerNumber + 1) * SIZE / threadsSupported * 10;
        workerNumber++;
        myMutex.unlock();

        if (localWorkerNumber < threadsSupported * 10)
        {
            for (uint64_t i = startIndex; i < endIndex; i++)
            {
                if (i % (1024 * 1024UL) == 0)
                {
                    printf("threadID %u:, i is %lu\n", threadID, i);
                }
                localResult += (uint64_t)arr[i];
            }
        }
        else
        {
            break;
        }
    }
}

void workForCrewLoopUnrolling(uint8_t threadID){
     while (workerNumber < threadsSupported * 10)
    {
        uint64_t startIndex, endIndex, localResult = 0;
        unsigned int localWorkerNumber = workerNumber;

        myMutex.lock();
        startIndex = workerNumber * SIZE / threadsSupported * 10;
        endIndex = (workerNumber + 1) * SIZE / threadsSupported * 10;
        workerNumber++;
        myMutex.unlock();

        if (localWorkerNumber < threadsSupported * 10)
        {
            for (uint64_t i = startIndex; i < endIndex; i+=4)
            {
                if (i % (1024 * 1024UL) == 0)
                {
                    printf("threadID %u:, i is %lu\n", threadID, i);
                }
                localResult += (uint64_t)arr[i];
                localResult += (uint64_t)arr[i+1];
                localResult += (uint64_t)arr[i+2];
                localResult += (uint64_t)arr[i+3];
            }
        }
        else
        {
            break;
        }
    }
}

void workForCrewLoopUnrolling20(uint8_t threadID){
     while (workerNumber < threadsSupported * 10)
    {
        uint64_t startIndex, endIndex, localResult = 0;
        unsigned int localWorkerNumber = workerNumber;

        myMutex.lock();
        startIndex = workerNumber * SIZE / threadsSupported * 10;
        endIndex = (workerNumber + 1) * SIZE / threadsSupported * 10;
        workerNumber++;
        myMutex.unlock();

        if (localWorkerNumber < threadsSupported * 10)
        {
            for (uint64_t i = startIndex; i < endIndex; i+=20)
            {
                if (i % (1024 * 1024UL) == 0)
                {
                    printf("threadID %u:, i is %lu\n", threadID, i);
                }
                localResult += (uint64_t)arr[i];
                localResult += (uint64_t)arr[i+1];
                localResult += (uint64_t)arr[i+2];
                localResult += (uint64_t)arr[i+3];
                localResult += (uint64_t)arr[i+4];
                localResult += (uint64_t)arr[i+5];
                localResult += (uint64_t)arr[i+6];
                localResult += (uint64_t)arr[i+7];
                localResult += (uint64_t)arr[i+8];
                localResult += (uint64_t)arr[i+9];
                localResult += (uint64_t)arr[i+10];
                localResult += (uint64_t)arr[i+11];
                localResult += (uint64_t)arr[i+12];
                localResult += (uint64_t)arr[i+13];
                localResult += (uint64_t)arr[i+14];
                localResult += (uint64_t)arr[i+15];
                localResult += (uint64_t)arr[i+16];
                localResult += (uint64_t)arr[i+17];
                localResult += (uint64_t)arr[i+18];
                localResult += (uint64_t)arr[i+19];
            }
        }
        else
        {
            break;
        }
    }
}

void workerCrewSetup(thread *threads)
{
    auto start = std::chrono::high_resolution_clock::now();

    // // FORK-JOIN MODEL
    for (int i = 0; i < threadsSupported; i++)
    {
        threads[i] = thread(workForCrew, i);
    }
    for (int i = 0; i < threadsSupported; i++)
    {
        threads[i].join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::nano> running_time = end - start;
    times.push_back(running_time.count());

    //-----------------------------------------------
    //Casting
    start = std::chrono::high_resolution_clock::now();

    // // FORK-JOIN MODEL
    for (int i = 0; i < threadsSupported; i++)
    {
        threads[i] = thread(workForCrewCasting, i);
    }
    for (int i = 0; i < threadsSupported; i++)
    {
        threads[i].join();
    }

    end = std::chrono::high_resolution_clock::now();
    running_time = end - start;
    times.push_back(running_time.count());

    //-----------------------------------------------
    //Loop Unrolling
    start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < threadsSupported; i++)
    {
        threads[i] = thread(workForCrewLoopUnrolling, i);
    }
    for (int i = 0; i < threadsSupported; i++)
    {
        threads[i].join();
    }

    end = std::chrono::high_resolution_clock::now();
    running_time = end - start;
    times.push_back(running_time.count());

    //-----------------------------------------------
    //Loop Unrolling 20
    start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < threadsSupported; i++)
    {
        threads[i] = thread(workForCrewLoopUnrolling20, i);
    }
    for (int i = 0; i < threadsSupported; i++)
    {
        threads[i].join();
    }

    end = std::chrono::high_resolution_clock::now();
    running_time = end - start;
    times.push_back(running_time.count());
}

void columnMajorWork(uint64_t threadID)
{
    uint64_t const numColumns = SIZE / threadsSupported;
    uint64_t localResult = 0;

    printf("I am thread %lu\n", threadID);
    for (uint64_t i = threadID; i < SIZE; i += numColumns)
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

void multiColumnMajor(thread *threads)
{
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
    workerCrewSetup(threads);

    printf("\n\nTask                      Time\n");
    printf("Single thread row major:      %f\n", times[0]);
    printf("Single thread column major:   %f\n", times[1]);
    printf("Multi-threaded row major:     %f\n", times[2]);
    printf("Multi-threaded column major:  %f\n", times[3]);
    printf("Worker Crew:                  %f\n", times[4]);
    printf("worker Crew casting:          %f\n", times[5]);
    printf("Loop unrolling 4:             %f\n", times[6]);
    printf("Loop unrolling 20:            %f\n", times[7]);


    // printf("Master thread, child threads are complete!\n");

    // //uint64_t result = 0;
    // //for (uint64_t i = 0; i < SIZE; i++) {
    // //}
    delete[] threads;
    delete[] arr;
    return 0;
}