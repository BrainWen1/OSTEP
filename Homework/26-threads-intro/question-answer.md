1. `./x86.py -p loop.s -t 1 -i 100 -R dx`
2. 
ARG seed 0
ARG numthreads 1
ARG program loop.s
ARG interrupt frequency 100
ARG interrupt randomness False
ARG argv 
ARG load address 1000
ARG memsize 128
ARG memtrace 
ARG regtrace dx
ARG cctrace False
ARG printstats False
ARG verbose False

   dx          Thread 0         
    0   
   -1   1000 sub  $1,%dx
   -1   1001 test $0,%dx
   -1   1002 jgte .top
   -1   1003 halt

2. `./x86.py -p loop.s -t 2 -i 100 -a dx=3,dx=3 -R dx`

ARG seed 0
ARG numthreads 2
ARG program loop.s
ARG interrupt frequency 100
ARG interrupt randomness False
ARG argv dx=3,dx=3
ARG load address 1000
ARG memsize 128
ARG memtrace 
ARG regtrace dx
ARG cctrace False
ARG printstats False
ARG verbose False

   dx          Thread 0                Thread 1         
    3   
    2   1000 sub  $1,%dx
    2   1001 test $0,%dx
    2   1002 jgte .top
    1   1000 sub  $1,%dx
    1   1001 test $0,%dx
    1   1002 jgte .top
    0   1000 sub  $1,%dx
    0   1001 test $0,%dx
    0   1002 jgte .top
   -1   1000 sub  $1,%dx
   -1   1001 test $0,%dx
   -1   1002 jgte .top
   -1   1003 halt
    3   ----- Halt;Switch -----  ----- Halt;Switch -----  
    2                            1000 sub  $1,%dx
    2                            1001 test $0,%dx
    2                            1002 jgte .top
    1                            1000 sub  $1,%dx
    1                            1001 test $0,%dx
    1                            1002 jgte .top
    0                            1000 sub  $1,%dx
    0                            1001 test $0,%dx
    0                            1002 jgte .top
   -1                            1000 sub  $1,%dx
   -1                            1001 test $0,%dx
   -1                            1002 jgte .top
   -1                            1003 halt

%dx 是线程私有寄存器，并非共享资源，因此每个线程都有自己的 dx 寄存器副本，互不干扰,
所以多线程不会影响结果，也没有竞态条件。

3. `./x86.py -p loop.s -t 2 -i 3 -r -a dx=3,dx=3 -R dx`

ARG seed 0
ARG numthreads 2
ARG program loop.s
ARG interrupt frequency 3
ARG interrupt randomness True
ARG argv dx=3,dx=3
ARG load address 1000
ARG memsize 128
ARG memtrace 
ARG regtrace dx
ARG cctrace False
ARG printstats False
ARG verbose False

   dx          Thread 0                Thread 1         
    3   
    2   1000 sub  $1,%dx
    2   1001 test $0,%dx
    2   1002 jgte .top
    3   ------ Interrupt ------  ------ Interrupt ------  
    2                            1000 sub  $1,%dx
    2                            1001 test $0,%dx
    2                            1002 jgte .top
    2   ------ Interrupt ------  ------ Interrupt ------  
    1   1000 sub  $1,%dx
    1   1001 test $0,%dx
    2   ------ Interrupt ------  ------ Interrupt ------  
    1                            1000 sub  $1,%dx
    1   ------ Interrupt ------  ------ Interrupt ------  
    1   1002 jgte .top
    0   1000 sub  $1,%dx
    1   ------ Interrupt ------  ------ Interrupt ------  
    1                            1001 test $0,%dx
    1                            1002 jgte .top
    0   ------ Interrupt ------  ------ Interrupt ------  
    0   1001 test $0,%dx
    0   1002 jgte .top
   -1   1000 sub  $1,%dx
    1   ------ Interrupt ------  ------ Interrupt ------  
    0                            1000 sub  $1,%dx
   -1   ------ Interrupt ------  ------ Interrupt ------  
   -1   1001 test $0,%dx
   -1   1002 jgte .top
    0   ------ Interrupt ------  ------ Interrupt ------  
    0                            1001 test $0,%dx
    0                            1002 jgte .top
   -1   ------ Interrupt ------  ------ Interrupt ------  
   -1   1003 halt
    0   ----- Halt;Switch -----  ----- Halt;Switch -----  
   -1                            1000 sub  $1,%dx
   -1                            1001 test $0,%dx
   -1   ------ Interrupt ------  ------ Interrupt ------  
   -1                            1002 jgte .top
   -1                            1003 halt

无论中断多频繁、是否随机，每个线程的 %dx 都是独立的私有资源，
线程切换只会暂停当前线程、执行另一个线程的私有逻辑，不会出现资源共享导致的行为变化，
所以结果依然相同，不会影响结果，也没有竞态条件。

4. `./x86.py -p looping-race-nolock.s -t 1 -M 2000 -R bx`

ARG seed 0
ARG numthreads 1
ARG program looping-race-nolock.s
ARG interrupt frequency 50
ARG interrupt randomness False
ARG argv 
ARG load address 1000
ARG memsize 128
ARG memtrace 2000
ARG regtrace bx
ARG cctrace False
ARG printstats False
ARG verbose False

 2000      bx          Thread 0         
    0       0   
    0       0   1000 mov 2000, %ax
    0       0   1001 add $1, %ax
    1       0   1002 mov %ax, 2000
    1      -1   1003 sub  $1, %bx
    1      -1   1004 test $0, %bx
    1      -1   1005 jgt .top
    1      -1   1006 halt

5. `./x86.py -p looping-race-nolock.s -t 2 -a bx=3 -M 2000 -R bx`

ARG seed 0
ARG numthreads 2
ARG program looping-race-nolock.s
ARG interrupt frequency 50
ARG interrupt randomness False
ARG argv bx=3
ARG load address 1000
ARG memsize 128
ARG memtrace 2000
ARG regtrace bx
ARG cctrace False
ARG printstats False
ARG verbose False

 2000      bx          Thread 0                Thread 1         
    0       3   
    0       3   1000 mov 2000, %ax
    0       3   1001 add $1, %ax
    1       3   1002 mov %ax, 2000
    1       2   1003 sub  $1, %bx
    1       2   1004 test $0, %bx
    1       2   1005 jgt .top
    1       2   1000 mov 2000, %ax
    1       2   1001 add $1, %ax
    2       2   1002 mov %ax, 2000
    2       1   1003 sub  $1, %bx
    2       1   1004 test $0, %bx
    2       1   1005 jgt .top
    2       1   1000 mov 2000, %ax
    2       1   1001 add $1, %ax
    3       1   1002 mov %ax, 2000
    3       0   1003 sub  $1, %bx
    3       0   1004 test $0, %bx
    3       0   1005 jgt .top
    3       0   1006 halt
    3       3   ----- Halt;Switch -----  ----- Halt;Switch -----  
    3       3                            1000 mov 2000, %ax
    3       3                            1001 add $1, %ax
    4       3                            1002 mov %ax, 2000
    4       2                            1003 sub  $1, %bx
    4       2                            1004 test $0, %bx
    4       2                            1005 jgt .top
    4       2                            1000 mov 2000, %ax
    4       2                            1001 add $1, %ax
    5       2                            1002 mov %ax, 2000
    5       1                            1003 sub  $1, %bx
    5       1                            1004 test $0, %bx
    5       1                            1005 jgt .top
    5       1                            1000 mov 2000, %ax
    5       1                            1001 add $1, %ax
    6       1                            1002 mov %ax, 2000
    6       0                            1003 sub  $1, %bx
    6       0                            1004 test $0, %bx
    6       0                            1005 jgt .top
    6       0                            1006 halt
