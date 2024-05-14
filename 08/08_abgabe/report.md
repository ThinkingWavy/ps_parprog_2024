# Exercise Sheet 08
Solution by Johanna Backer, Camillo Zanolin and Marco Fröhlich.

## Task 1
**Benchmarks:**\
Each variant of the program had 500000000 iterations and executed 10 times.

#### Master
![master](task1/images/master.png)

#### Spread
![spread](task1/images/spread.png)

#### Close
![close](task1/images/close.png)

### Findings
The variant with the master affinity setting was the most consistent one. Spread and Close one had both outliners up and downwards. On average the fastest variant used the master affinity setting with 17.233 seconds.

## Task 2

### Benchmark Serial vs. Parallel version
Each version was benchmarked on the LCC3 cluster using 10 iterations per number of threads $\{1,2,4,6,12\}$ and array matrix $\{10000^2, 20000^2, 30000^2\}$.

**Parallel version:**
![parallel](task2/images/parallel.png)

**Serial version:**\
The serial version had the following results:

| matrix size | time (s) |
| ----------- | -------- |
| 10000       | 0.41     |
| 20000       | 1.71     |
| 30000       | 3.78     |

### Static, Guided, Dynamic scheduling
Each version was benchmarked on the LCC3 cluster using 10 iterations per number of threads $\{1,2,4,6,12\}$ and array matrix $\{10000^2, 20000^2, 30000^2\}$.\
\
**static scheduling:**
![static](task2/images/static.png)\
**guided scheduling:**
![guided](task2/images/guided.png)\
**dynamic scheduling:**
![dynamic](task2/images/dynamic.png)\
The dynamic version was not finished with all 150 versions, due to the time limit for computation on the LCC3.\
\
The static and guided scheduling version took nearly the same amount of time. For the matrix size $3000^2$, these implementations took surprisingly the same amount of time.
The dynamic version took way more time.
