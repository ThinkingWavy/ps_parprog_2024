## Exercise 1 

This exercise consists in familiarizing yourself with the hardware of LCC3, that you're running your applications on.

Understanding the architecture of the utilized hardware is paramount for optimizing (parallel) programs. It enables precise resource allocation, memory optimization, and strategic parallelization decisions. In this exercise, we are interested in gathering information about the hardware of the LCC3 compute nodes utilizing the [Portable Hardware Locality (hwloc)](https://www.open-mpi.org/projects/hwloc/) software package.

### Tasks

- Connect to the LCC3 compute cluster using your login credentials. Once connected, load the `hwloc` module via `module load hwloc/2.8.0-intel-2021.7.1-co55pj6`.
- Find out more about the hardware that you're running on by executing `lstopo --of txt`.
- Interpret the output and describe what information you can gather. Please also elaborate on these questions:
  - Can you retrieve the number of CPUs and cores from the output?
  - Investigate the memory hierarchy information provided by lstopo.
    - How much memory (RAM) does the compute node offer?
    - What does the term `NUMANode` tell you about the memory? Why are there two `NUMANodes`?
    - What interesting information can you retrieve about the caches of the system?
  - Is there anything else you can find out from the output?
- Based on your observations, how many threads could you utilize at maximum when parallelizing a program with OpenMP on this system?
- Compare your observations to the [LCC3 documentation]


### Output 
- __machine (Node)__: You can see the hardware information for the node on which the script or command is running.
- __Package (CPU)__:  The CPUs per Node. In our case those are two each. 
- __Cores__: The cores are clearly visible. In the case of the lcc3 those are six each. 
- __Hardware_Threads__: Each core has two hardware supported threads

#### NUMAode, RAM and Caches
- Definition: Is a Non-uniform memory access (NUMA) node. It is a server platform that can utilize multiple processors where all processors can access all memory on the board. If some memory is accessed by a processor that lies outside of its node it has to be accessed over the slower NUMA connection. 
- Two NUMA: Tells us that it is a distributed shared memory. Two NUMAs are needed, because each processor needs also be able to access each others memory/caches on the same node? 
- RAM: 47GB total
- Caches: L3 Cache is shared in each processor. L2 and L1 is for each core seperatly available. 

#### Threads
So the maximum threads that can be utilized on the lcc3 are 2CPUS*6Cores*2hardwareThreads = 24Threads parallelized

# Exercise 2

### Description

Amdahl's law is one of the most important properties of parallel programs.

### Tasks

- How is the speedup of a parallel program defined?
- What is the formal definition of Amdahl's law and what relationship does it describe for parallel programs (explain in your own words)? Why/How is this significant?
- Compute the theoretical speedup of a program that spends 10% of its time in unparallelizable, sequential regions for 6 cores and for a hypothetically unlimited number of cores.
- Compute the theoretical speedup of a program that spends 20% of its time in unparallelizable, sequential regions for 6 cores and for a hypothetically unlimited number of cores.
- Given an algorithm of time complexity O(n^3). How large (in %) can the unparallelizable, sequential region be at most, such that a speedup of 10 can be achieved using 64 cores?

### Output

- Speedup: processor/(1+(processor - 1)*unparalizablePortion)

- __Amdahls law__: Amdahls law is the maximum possible speedup achivable depending on the processors and the amount of the programm that cannot be parallelized. It is the time divided thew addition of the time of parallelized section + the time of the parallelized section divided threw the processors. That amounts to the `proportion` of the normal time of the programm to the time of the parallelized version of the program. It is significant because it tells us that even if we parallelize perfectly but have a rather big portion of the program in serial, `we cannot expect` a huge accerleration of the program. 

#### 10% unparalellized : 
Speedup_6 = $\frac{1}{(1 - 0.9) + \frac{0.9}{6}}$

Speedup_6 = $\frac{1}{0.1 + 0.15}$

Speedup_6 = 4

Speedup_unl = $\frac{1}{0.1+0}$ = 10
#### 20% unparalellized:
Speedup_6 = $\frac{1}{(1 - 0.8) + \frac{0.8}{6}}$

Speedup_6 = $\frac{1}{0.2 + 0.13333}$ = $\frac{1}{\frac{1}{3}}$ 

Speedup_6 = `3`

Speedup_unl = $\frac{1}{0.2+0}$ = 5 


#### Speedup_Alg:`

10 =  $\frac{64}{(1+63*f)}$

64 = 10 +630f

630f = 54

f =$\frac{54}{630}$ = 0,08571429 ~ `8.5 % unparalallizable`
​



