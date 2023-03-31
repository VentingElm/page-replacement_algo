The memory traces used will be bzip.trace and sixpack.trace.

The file neccessary to compile the program:
-memsim.cpp : The simulator program which takes user input in two forms and implements the page algorithms given as input.
The page replacement algortithms are FIFO(First-in First-Out Replacement Policy), LRU(Least Recently Used Policy) and VMS(segmented-fifo policy).

To compile the program:
1. You can use command "make" and it will compile the file and will then create an executable "memsim"
2. Alternatively, you can also try "g++ memsim.cpp -o memsim" to compile the program

To run the program:

The program can be run with two types of input format
1. For fifo and lru use the following format:
./memsim tracefile nFrames policy quiet/debug

2. For segmented fifo use the following format:
./memsim tracefile nframes policy percentage quiet/debug

where:
-tracefile: name of the tracefile
-nFrames: number of frames to use
-policy: fifo, lru or vms
-percentage: 1-100
- quiet/debug: quiet will just print the stats while debug will print every event that happens

Running Examples:
./memsim bzip.trace 64 fifo quiet
total memory frames: 64
events in trace: 1000000
total disk reads: 1467
total disk writes: 514

./memsim bzip.trace 64 vms 0 quiet
total memory frames: 64
events in trace: 1000000
total disk reads: 1467
total disk writes: 514

./memsim bzip.trace 64 vms 25 quiet
total memory frames: 64
events in trace: 1000000
total disk reads: 1367
total disk writes: 471

./memsim bzip.trace 64 vms 50 quiet
total memory frames: 64
events in trace: 1000000
total disk reads: 1290
total disk writes: 427

./memsim bzip.trace 64 vms 75 quiet
total memory frames: 64
events in trace: 1000000
total disk reads: 1267
total disk writes: 416

./memsim bzip.trace 64 vms 100 quiet
total memory frames: 64
events in trace: 1000000
total disk reads: 1264
total disk writes: 420

./memsim bzip.trace 64 lru quiet
total memory frames: 64
events in trace: 1000000
total disk reads: 1264
total disk writes: 420

