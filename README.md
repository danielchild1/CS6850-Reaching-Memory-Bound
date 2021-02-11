# CS6850-Reaching-Memory-Bound
compile using: `g++ -O3 main.cpp -o main -pthread` 

`main` was compiled with no optimization <br />
`main02` was compiled with `-02` <br />
`main03` was compiled with `-03` <br />

makes an array of uint8_t of size 1024 * 1024 * 1024 * 100ULL which totals to 100GB. Then uses various
methods to read it. The program then displays the times in milli-seconds.
