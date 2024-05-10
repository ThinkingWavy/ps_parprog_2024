# Exercise Sheet 07
Solution by Johanna Backer, Camillo Zanolin and Marco Fröhlich
## Exercise 1
### Snippet 1
```C
a[0] = 0;
#pragma omp parallel for
for (i=1; i<N; i++) {
    a[i] = 2.0*i*(i-1); //S_1
    b[i] = a[i] - a[i-1]; //S_2
}
```
##### Dependency analysis 

| i   | S_1:a[i] | S_2:a[i] | S_2:a[i-1] |
| --- | -------- | -------- | ---------- |
| 1   | a[1] =   | = a[1]   | = a[0]     |
| 2   | a[2] =   | = a[2]   | = a[1]     |

Dependencies:
- $D1 = S_1 \delta S_2 $
- $D2 = S_2 \delta S_1 $

| Dep | Source    | Type     | Sink        | Dist Vect | Dir Vec | Loop carried |
| --- | --------- | -------- | ----------- | --------- | ------- | ------------ |
| D1  | $S1:a[i]$ | $\delta$ | $S2:a[i]$   | 0         | $=$     | No           |
| D2  | $S1:a[i]$ | $\delta$ | $S2:a[i-1]$ | 1         | $<$     | Yes          |

Existing race condition. If the for loop is divided into chunks it can lead to issues, because of the loop-carried dependency D2. 

```C
a[0] = 0;
#pragma omp parallel for
for (i=1; i<N; i++) {
    a[i] = 2.0*i*(i-1);
}
#pragma omp parallel for 
for(i=1; i<N, i++){
    b[i] = a[i] - a[i-1];
}
```

### Snippet 2
```c
a[0] = 0;
#pragma omp parallel
{
    #pragma omp for nowait
    for (i=1; i<N; i++) {
        a[i] = 3.0*i*(i+1); //S_1
    }
    #pragma omp for
    for (i=1; i<N; i++) {
        b[i] = a[i] - a[i-1]; //S_2
    }
}
```

Dep      | Source       | Type     | Sink      | Dist Vect | Dir Vec 
-------- | --------     | -------- | --------- | ------- | -------- 
D1       | $S1:a[i]$    | $\delta$ | $S2:a[i-1]$   | 1     |  $<$   

The race condition can occur because of the `nowait` in the first for loop. All `a[i]` must be computed before the second loop can be started, because of the loop-carried dependence D1 again. 

### Snippet 3
```C
#pragma omp parallel for
for (i=1; i<N; i++) {
   x = sqrt(b[i]) - 1; //S1
   a[i] = x*x + 2*x + 1; //S2
}
```

| Source | Type     | Sink | Dist Vec | Dir Vec | Loop        |
| ------ | -------- | ---- | -------- | ------- | ----------- |
| S1:x   | $\delta$ | S2:x | 0        | =       | independent |

The Code snippet is not correctly parallized. The variable should be private x. Because otherwise, it is a global variable that has concurrent accesses. 

Solution:
```C
#pragma omp parallel for private(x)
for (i=1; i<N; i++) {
   x = sqrt(b[i]) - 1; //S1
   a[i] = x*x + 2*x + 1; //S2
}
```


### Snippet 4
```c
f = 2;
#pragma omp parallel for private(f,x)
for (i=1; i<N; i++) {
    x = f * b[i];
    a[i] = x - 7;
}
a[0] = x; 
```

A race condition occurs because the write execution of `a[0]` depends on which order the loops are carried out. To solve this issue, a `lastprivate[x]` can be used.
The `firstprivate(f)`is needed because otherwise the local variable f is not initialized. Alternatively, the variable f could be left global in this case.

```c
f = 2;
#pragma omp parallel for firstprivate(f) lastprivate(x)
for (i=1; i<N; i++) {
    x = f * b[i];
    a[i] = x - 7;
}
a[0] = x; 
```

### Snippet 5
```C
sum = 0; 
#pragma omp parallel for
for (i=1; i<N; i++) {
S1    sum = sum + b[i];
}
```

| Source | Type     | Sink   | Dist Vec | Dir Vec | Loop        |
| ------ | -------- | ------ | -------- | ------- | ----------- |
| $S1:sum$ | $\delta$ | $S1:sum$ | 0        | =       | independent |

Race Condition between threads when parallized, because `sum` is a shared variable. For correct implementation, synchronization is needed. For example:

```C
sum = 0; 
#pragma omp parallel for reduction(+: sum)
for (i=1; i<N; i++) {
   sum += b[i];
}
```

## Exercise 2

#### Loop Line 27

- could not be vectorized, because of `printf` statement
  > analysis.c:28:17: missed: statement clobbers memory: printf ("%d ", \_6);\
  > analysis.c:27:26: missed: not vectorized: loop contains function calls or data references that cannot be analyzed

#### Loop Line 22

- could not be vectorized because of bad data dependency
  > analysis.c:23:25: missed: possible alias involving gather/scatter between a[_4] and a[i_47]\
  > analysis.c:22:26: missed: bad data dependence.

#### Loop Line 10

- Loop was successfully vectorized

#### Loop Line 14

- not vectorized
  > analysis.c:15:22: missed: not vectorized: more than one data ref in stmt: MEM <unsigned char[4096]> [(char * {ref-all})&b] = MEM <unsigned char[4096]> [(char * {ref-all})&a];

#### Loop Line 18

- not vectorized
  > analysis.c:19:24: missed: statement clobbers memory: \_\_builtin_memmove (&a, \_12, 4080);

Each loop is analyzed for its structure, including loop nest analysis, data references, scalar cycles, and loop induction variables. The compiler tries different vectorization modes to determine if vectorization is possible, considering factors like loop structure, data references, and alignments Afterwards the cost of vectorization is analyzed, considering factors such as vector inside/outside loop costs, scalar iteration costs, and prologue/epilogue costs.

## Exercise 3:


### Snippet 1: Manual/Compiler
```C
    void copy(double* x, double* y) {   //4
        for(int i = 0; i < 1024; i++) { //5
            x[i] = y[i];                //6
        }                               //7
    }                                   //8
```
It is possible to manually parallelize the code and to have it done by the compiler. Manually it could just be done with a plain 

```c
#pragma omp parallel for
```

To see if the compiler is capable of parallelizing it, it can simply be shown by creating a c file and setting the same flags as in exercise 2. 

The output of compiling an executable of a c file containing the snippet above with the equal flags as part 2, contains the following:
```loop_vect.c:5:19: note:  LOOP VECTORIZED```
So it can be deduced that it is possible. 


### Snippet 2: Normalize
```C
    for (int i=4; i<=N; i+=9) {
        for (int j=0; j<=N; j+=5) {
            A[i] = 0;
        }
    }
```

- Step 1 replace loop header
    for(In = 1; In <= (U-L/S); In++) 

- Step 2: replace i 
    replace i with In*S-S+L

- Step 3: final assignment 
    i = In*S-S+L


```c
int L = 4 //lower
int U = N //upper
int S = 9 //step
for (In = 1; In <= (N+5)/9; In++){//Step 1 
     A[9*In+5] = 0; //Step 2
}
i = 9*In+5 //Step 3
```  

### Snippet 3: Dependency Analysis
```c
for(int i = 1; i < N; i++) {
    for(int j = 1; j < M; j++) {
        for(int k = 1; k < L; k++) {
            a[i+1][j][k-1] = a[i][j][k] + 5;
        }
    }
}
```

| i   | j   | k   | S_1:a[i+1][j][k-1]=  | =S_1:a[i][j][k]      |
| --- | --- | --- | -------------------- | -------------------- |
| 1   | 1   | 1   | a[2][1][0]=          | =a[1][1][1]          |
| 1   | 1   | 2   | **a[2][1][1]= *D1*** |                      |
| 2   | 1   | 1   |                      | **=a[2][1][1] *D1*** |
|     |     |     |                      |                      |

| Dep | Source               | Type     | Sink             | Dist Vect | Dir Vec   |
| --- | -------------------- | -------- | ---------------- | --------- | --------- |
| D1  | $S1:*a[i+1][j][k-1]$ | $\delta$ | $S2:a[i][j][k] $ | (1,0,-1)  | $(<,=,>)$ |

