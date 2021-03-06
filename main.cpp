#include <cstdio> // For printf, no cout in multithreaded
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <inttypes.h>

using std::mutex;
using std::thread;
using std::vector;

const uint64_t SIZE = 1024 * 1024 * 1024 * 100ULL; // 100 GB!
unsigned int threadsSupported = std::thread::hardware_concurrency();
const uint64_t NUM_ROWS = (1024* 1024); // * 1024
const uint64_t NUM_COLS = (1024 * 100);// * 100

uint8_t *arr{nullptr};
uint8_t *result{0};
mutex myMutex;
vector<double> times;
unsigned int workerNumber = 0;

void rowMajorWork(uint8_t threadID)
{
    uint64_t localResult = 0;

    uint64_t startIndex = threadID * NUM_ROWS / threadsSupported;
    uint64_t endIndex = (threadID + 1) * NUM_ROWS / threadsSupported;
    for (uint64_t r = startIndex; r < endIndex; r++)
    {
        for (uint64_t c = 0; c < NUM_COLS; c++)
            localResult += arr[r * NUM_COLS + c];
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
        unsigned int localWorkerNumber;

        myMutex.lock();
        localWorkerNumber = workerNumber++;
        myMutex.unlock();

        startIndex = localWorkerNumber * NUM_ROWS / (threadsSupported * 10);
        endIndex = (localWorkerNumber + 1) * NUM_ROWS / (threadsSupported * 10);

        if (localWorkerNumber < threadsSupported * 10)
        {
            for (uint64_t r = startIndex; r < endIndex; r++)
            {
                for (uint64_t c = 0; c < NUM_COLS; c++)
                    localResult += arr[r * NUM_COLS + c];
            }
        }
        else
        {
            // MUTEX
    myMutex.lock();
    // Run critical region of code
    result += localResult;
    myMutex.unlock();
    break;
        }
    }
}

void workForCrewCasting(uint8_t threadID)
{
    while (workerNumber < threadsSupported * 10)
    {
        uint64_t startIndex, endIndex, localResult = 0;
        unsigned int localWorkerNumber;

        myMutex.lock();
        localWorkerNumber = workerNumber++;
        myMutex.unlock();

        startIndex = localWorkerNumber * SIZE / (threadsSupported * 10);
        endIndex = (localWorkerNumber + 1) * SIZE / (threadsSupported * 10);

        if (localWorkerNumber < threadsSupported * 10)
        {
            for (uint64_t r = startIndex; r < endIndex; r ++)
            {
                for (uint64_t c = 0; c < NUM_COLS; c += 8)
                {
                    // Make a copy of 8 bytes.
                    uint64_t buffer = *(reinterpret_cast<uint64_t *>(&arr[r * NUM_COLS + c]));

                    uint8_t *bufferArray = reinterpret_cast<uint8_t *>(&buffer);
                    localResult += bufferArray[0];
                    localResult += bufferArray[1];
                    localResult += bufferArray[2];
                    localResult += bufferArray[3];
                    localResult += bufferArray[4];
                    localResult += bufferArray[5];
                    localResult += bufferArray[6];
                    localResult += bufferArray[7];

                }
            }
        }
        else
        {   // MUTEX
    myMutex.lock();
    // Run critical region of code
    result += localResult;
    myMutex.unlock();
            break;
        }
    }
}

void workForCrewLoopUnrolling(uint8_t threadID)
{
    while (workerNumber < threadsSupported * 10)
    {
        uint64_t startIndex, endIndex, localResult = 0;
        unsigned int localWorkerNumber;

        myMutex.lock();
        localWorkerNumber = workerNumber++;
        myMutex.unlock();

        startIndex = workerNumber * SIZE / threadsSupported * 10;
        endIndex = (workerNumber + 1) * SIZE / threadsSupported * 10;

        if (localWorkerNumber < threadsSupported * 10)
        {
            for (uint64_t r = startIndex; r < endIndex; r ++)
            {
                for (uint64_t c = 0; c < NUM_COLS; c += 4)
                {
                //localResult += arr[r * NUM_COLS + c];
                localResult += (uint64_t)arr[r * NUM_COLS + c];
                localResult += (uint64_t)arr[r * NUM_COLS + c+ 1];
                localResult += (uint64_t)arr[r * NUM_COLS + c + 2];
                localResult += (uint64_t)arr[r * NUM_COLS + c + 3];
                }
            }
        }
        else
        {
            // MUTEX
    myMutex.lock();
    // Run critical region of code
    result += localResult;
    myMutex.unlock();
            break;
        }
    }
}

void workForCrewLoopUnrolling20(uint8_t threadID)
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
            for (uint64_t r = startIndex; r < endIndex; r ++)
            {
                for (uint64_t c = 0; c < NUM_COLS; c += 20)
                {
                localResult += (uint64_t)arr[r * NUM_COLS + c];
                localResult += (uint64_t)arr[r * NUM_COLS + c + 1];
                localResult += (uint64_t)arr[r * NUM_COLS + c + 2];
                localResult += (uint64_t)arr[r * NUM_COLS + c + 3];
                localResult += (uint64_t)arr[r * NUM_COLS + c + 4];
                localResult += (uint64_t)arr[r * NUM_COLS + c + 5];
                localResult += (uint64_t)arr[r * NUM_COLS + c + 6];
                localResult += (uint64_t)arr[r * NUM_COLS + c + 7];
                localResult += (uint64_t)arr[r * NUM_COLS + c + 8];
                localResult += (uint64_t)arr[r * NUM_COLS + c + 9];
                localResult += (uint64_t)arr[r * NUM_COLS + c + 10];
                localResult += (uint64_t)arr[r * NUM_COLS + c + 11];
                localResult += (uint64_t)arr[r * NUM_COLS + c + 12];
                localResult += (uint64_t)arr[r * NUM_COLS + c + 13];
                localResult += (uint64_t)arr[r * NUM_COLS + c + 14];
                localResult += (uint64_t)arr[r * NUM_COLS + c + 15];
                localResult += (uint64_t)arr[r * NUM_COLS + c + 16];
                localResult += (uint64_t)arr[r * NUM_COLS + c + 17];
                localResult += (uint64_t)arr[r * NUM_COLS + c + 18];
                localResult += (uint64_t)arr[r * NUM_COLS + c + 19];
                }
            }
        }
        else
        {
            // MUTEX
    myMutex.lock();
    // Run critical region of code
    result += localResult;
    myMutex.unlock();
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
    std::chrono::duration<double, std::milli> running_time = end - start;
    times.push_back(running_time.count());
    workerNumber = 0;
    printf("Worker Crew:                  %f\n", times[4]);

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
    workerNumber = 0;
    printf("worker Crew casting:          %f\n", times[5]);
    

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
    workerNumber = 0;
    printf("Loop unrolling 4:             %f\n", times[6]);
   

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
     printf("Loop unrolling 20:            %f\n", times[7]);
}

void columnMajorWork(uint8_t threadID)
{
    uint64_t localResult = 0;

    // Assume that every thread starts its second dimension index at 0 to its max
    // Each thread starts its first dimension based on math
    uint16_t shrinkSize = 128;
    uint64_t startCol = (threadID * NUM_COLS / shrinkSize) / threadsSupported;
    uint64_t endCol = ((threadID + 1) * NUM_COLS / shrinkSize) / threadsSupported;
    for (uint64_t j = startCol; j < endCol; j++)
    { // horizontal row loop
        for (uint64_t i = 0; i < NUM_ROWS; i++)
        { // vertical column loop
            //printf("ThreadID: %u, I am now computing arr[%lu][%lu] at address %p\n", threadID, i, j, &(arr[i * NUM_COLS + j]));
            localResult += arr[i * NUM_COLS + j];
        }
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
    std::chrono::duration<double, std::milli> running_time = end - start;
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
    std::chrono::duration<double, std::milli> running_time = end - start;
    times.push_back(running_time.count() * 128);
}

void singleThreadRow()
{
    auto start = std::chrono::high_resolution_clock::now();

    uint64_t localResult = 0;

    for (uint64_t r = 0; r < NUM_ROWS; r++)
    {
        for (uint64_t c = 0; c < NUM_COLS; c++)
        {
            localResult += arr[r * NUM_COLS + c];
        }
    }

    // MUTEX
    myMutex.lock();
    // Run critical region of code
    result += localResult;
    myMutex.unlock();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> running_time = end - start;
    times.push_back(running_time.count());
}

void singleThreadColumn()
{
    auto start = std::chrono::high_resolution_clock::now();
    uint64_t localResult = 0;

    for (uint64_t c = 0; c < NUM_COLS; c++)
    {

        for (uint64_t r = 0; r < NUM_ROWS; r++)

        {
            localResult += arr[r * NUM_COLS + c];
        }
    }

    // MUTEX
    myMutex.lock();
    // Run critical region of code
    result += localResult;
    myMutex.unlock();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> running_time = end - start;
    times.push_back(running_time.count());
}

void startArray()
{
    while (workerNumber < threadsSupported * 10)
    {
        uint64_t startIndex, endIndex;
        unsigned localWorkerNumber;

        myMutex.lock();
        localWorkerNumber = workerNumber++;
        myMutex.unlock();

        startIndex = localWorkerNumber * SIZE / (threadsSupported * 10);
        endIndex = (localWorkerNumber + 1) * SIZE / (threadsSupported * 10);

        if (localWorkerNumber < threadsSupported * 10)
        {
            for (uint64_t i = startIndex; i < endIndex; i++)
            {
                arr[i] = 2;
            }
        }
        else
        {
            break;
        }
    }
}

int main()
{

    printf("This machine has %d cores.\n", threadsSupported);

    arr = new uint8_t[SIZE];
    printf("The item at index 42 is: %d!!!\n\n", arr[42]);

    // Create thread tracking objets, these are NOT threads themselves
    thread *threads = new thread[threadsSupported];

    // initializing the array
    for (int i = 0; i < threadsSupported; i++)
    {
        threads[i] = thread(startArray);
    }
    for (int i = 0; i < threadsSupported; i++)
    {
        threads[i].join();
    }
    workerNumber = 0;

    printf("\n\nTask                        Time\n");
    singleThreadRow();
    printf("Single thread row major:      %f\n", times[0]);

    singleThreadColumn();
    printf("Single thread column major:   %f\n", times[1]);

    multiRowMajor(threads);
    printf("Multi-threaded row major:     %f\n", times[2]);

    multiColumnMajor(threads);
    printf("Multi-threaded column major:  %f\n", times[3]);

    workerCrewSetup(threads);
    
    printf("%hhn\n", result);
    

    delete[] threads;
    delete[] arr;
    return 0;
}