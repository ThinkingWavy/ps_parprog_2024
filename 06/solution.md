# PS Parallel Programming Sheet 06
Solution by Johanna Backer, Camillo Zanolin and Marco Fröhlich

## Exercise 1:
Regarding each snippet

1) What are the data dependencies?
2) Parallelize and optimize the code


### a)
```c
for (int i=0; i < n-1; i++) {
    x[i] = (y[i] + x[i+1]) / 7; //S_1
}
```

##### Aus dem Proseminar 
Antidependency *loop carried*
$$D1 = S_1, \delta^{-1}, S_1 $$

Dep      | Source   | Type     | Sink      | Dist Vect | Dir Vec | Loop carried | Dependence carried by loop |
-------- | -------- | -------- | --------- | ------- | -------- | ------------- | ------
Inhalt   | Inhalt   | Inhalt   | Inhalt   | Inhalt   | Inhalt   |


1) Each value $x[i]$ is dependent on stored in $x[i-1]$. Also, it is dependent on the value $y[i]$ which is computed outside of the given loop.
2) Can be parallized by splitting $c$ equal chunks of size $n/c$. To achieve a correct result, the previous value of the array x must be stored as a variable before the loops start.

```c
temp = x //some memory copying required
#pragma omp parallel for
for (int i=0; i < n-1; i++) {
    x[i]for (int i=0; i < n-1; i++) {
    x[i] = (y[i] + x[i+1]) / 7;
} = (y[i] + temp[i+1]) / 7;
}
```

### b)
```c
for (int i=0; i < n; i++) {
    a = (x[i] + y[i]) / (i+1);
    z[i] = a;
}

f = sqrt(a + k);
```
1) The value of $f$ is dependent on the value of $a$ because it is determined in the last iteration of the loop. Also, the value is dependent on $k$, but this is set outside of the snippet.\
The computation of $z[i]$ inside of the loop can be done in parallel because the value is not dependent on previous iterations of the loop.
2) For loop can simply be parallelized with whatever scheduling method desired, with as a thread-local variable. To guarantee that $a$ has the correct value after the for loop it is required to recalculate its value before calculating f. Otherwise, it is not guaranteed that $a$ has the value computed in the last iteration of the loop.

```c
#pragma omp parallel for lastprivate(a)
for (int i=0; i < n; i++) {
    a = (x[i] + y[i]) / (i+1);
    z[i] = a;
}
a = (x[n]+y[n])/(i+1)
f = sqrt(a + k);
```

### c)
```c
for (int i=0; i < n; i++) {
   x[i] = y[i] * 2 + b * i; //S1
}


for (int i=0; i < n; i++) {
   y[i] = x[i]  + a / (i+1); //S2
}
```

##### From the PS 
$$D1 = S_1 \delta^{1} S_2 $$
$$D2 = S_2 \delta^{-1} S_1 $$

Dep      | Source   | Type     | Sink      | Dist Vect | Dir Vec | Loop carried | Dependence carried by loop |
-------- | -------- | -------- | --------- | ------- | -------- | ------------- | ------
D1 | Inhalt   | Inhalt   | Inhalt   | Inhalt   | Inhalt   |
D2 | Inhalt   | Inhalt   | Inhalt   | Inhalt   | Inhalt   |

 1) The values of $x[i]$ and $y[i]$ are both dependent on each other. If the first loop is executed before the second the result will differ from the other way around.
 2)  To Parallelize this snippet by compressing it into one loop it must be guaranteed that the compiler does not switch the order of computation of $x[i]$ and $y[i]$. But this might not be possible therefore just insert a barrier between the loop for $x$ and $y$.

 ```c
 #pragma omp parallel{
    #pragma omp for
    for (int i=0; i < n; i++) {
    x[i] = y[i] * 2 + b * i;
    }

    //Verbesserung in PS: Not needed because implicit barrier omp for
    //#pragma omp barrier
    
    #pragma omp for
    for (int i=0; i < n; i++) {
    y[i] = x[i]  + a / (i+1);
    }
}
```

## Exercise 2:

*not done*
#### Tasks:
- Where are the data dependencies?
- How can you parallelize the loops?

2a)
```c
double factor = 1;

for (int i=0; i < n; i++) {
    x[i] = factor * y[i]; //SOLUTION: Bitshift. 2^i 
    factor = factor / 2;
}
```

SOLUTION: split into two loops (loop distribution)
2b)
```c
for (int i = 1; i<n; i++) {
    x[i] = (x[i] + y[i-1]) / 2;
    y[i] = y[i] + z[i] * 3;
}
```

2c)
```c
x[0] = x[0] + 5 * y[0];
for (int i = 1; i<n; i++) {
    x[i] = x[i] + 5 * y[i];
    if ( twice ) {
        x[i-1] = 2 * x[i-1]
    }
}
```
SOLUTION: 
```c
for (int i = 0; i<n; i++) { 
    #pragma omp parallel for 
    x[i] = x[i] + 5 * y[i];
}

if ( twice ) {
    #pragma omp parallel for 
    for (int i = 1; i<n; i++) {
        x[i-1] = 2 * x[i-1]
    }
}
```


## Exercise 3:

```c
for (int i = 0; i < 4; ++i) {
    for (int j = 1; j < 4; ++j) {
        a[i + 2][j - 1] = b * a[i][j] + 4;
    }
}
```

### Tasks

#### 1) For each iteration, give the corresponding distance and direction vectors

Write: a[i+2][j-1] \
Read: a[i][j]

**Distance vector**: Difference in the first index and distance in the second index between read and write. \

###### Verbesserung in PS
Not correct. Better make a table 

i | j| Read | Write
-------- | -------- | -------- | ------
0   | 1   | $= a [0][1]$ | $a [2][0] =$
0   | 2   | $= a [0][2]$|  $a [2][1] =$
...

Dep      | Array Element   |  Dist Vect | Dir Vec 
-------- | -------- | -------- | --------- 
le[0,2] $\delta$ l3[2,1] | a[2][1]   | (2,-1) | (<,>) 
...

<!-- $d_1 = i -(i+2) = -2$ \ 
$d_2 = j-(j-1) = 1$ \
$D(d_1, d_2) = D(-2,1)$   -->


**Direction vector**: Indicates the direction of dependencies between iterations. \
![alt text](image.png) \
Example: \
$d_1 < 0$ implies a backward dependency in the "i" direction. \
$d_2 > 0$ implies a forward dependency in the "j" direction. \
For our example: $D(i,j) = D(>, <)$

#### 2) What type of dependence is represented in this code snippet?
**Data Dependence**: The order of execution affecte the correctness of the program output. \
- **Write-After-Read** (WAR) or Anti-dependence, a future write depends on the value read in a previous iteration. a[i+2][j-1] is written based on the value read from a[i][j]. \
- Read-After-Write (RAW) or True dependence, a read happends after a value has been written in an earlier iteration, and the data written incluences the data read. It could be that a[i+2][j-1] in a later iteration depends on values written in an earlier iteration of i.

**Loop Dependence**: The a[i+2][j-1] which means that the output of current iteration depends on the specific previous iteration (i-2). This is a loop-carried dependence.

#### 3) How would you parallelize this code?

##### Verbesserung: Innere schleife Parallelisieren 

<!-- ```c
#pragma omp parallel for schedule(static, 1)
    for (int i = 0; i < N - 2; i += 3) { // i+=3 to avoid dependency conflicts
        for (int j = 1; j < M - 1; ++j) {
            a[i + 2][j - 1] = b * a[i][j] + 4;
            if (i + 3 < N)
                a[i + 3][j - 1] = b * a[i + 1][j] + 4;
            if (i + 4 < N) 
                a[i + 4][j - 1] = b * a[i + 2][j] + 4;
        }
    }
```
**schedule()**: distributes the iterations in chunks where each thread gets consecutive blocks of one iteration at a time. This ensures that no two threads will be writing to and reading from the same elements in a conflicting manner.

But because we need at least [i+2], we give each thread the needed values in each for iteration. So we **add the calculation of [i+3] and [i+4]** and change the i+=3. With this we can ensure that dependences are resolved in the thread's chunk. -->
