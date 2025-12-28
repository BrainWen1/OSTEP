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

每个线程都循环3次是因为每个线程的 bx 寄存器初始值为3，每次循环减1，直到 bx 变为0 时跳出循环。
地址2000的内存位置被两个线程共享访问，导致竞态条件，最终结果不可预测，可能是4、5、6中的任意一个，具体取决于线程调度的时序，
这里的默认中断频率为 50，所以可以保证共享数据操作的原子性，避免竞态条件的发生，因此最终结果是6。

1. `./x86.py -p looping-race-nolock.s -t 2 -M 2000 -i 4 -r -s 0 -R bx`

ARG seed 0
ARG numthreads 2
ARG program looping-race-nolock.s
ARG interrupt frequency 4
ARG interrupt randomness True
ARG argv 
ARG load address 1000
ARG memsize 128
ARG memtrace 2000
ARG regtrace bx
ARG cctrace False
ARG printstats False
ARG verbose False

 2000      bx          Thread 0                Thread 1         
    0       0   
    0       0   1000 mov 2000, %ax
    0       0   1001 add $1, %ax
    1       0   1002 mov %ax, 2000
    1      -1   1003 sub  $1, %bx
    1       0   ------ Interrupt ------  ------ Interrupt ------  
    1       0                            1000 mov 2000, %ax
    1       0                            1001 add $1, %ax
    2       0                            1002 mov %ax, 2000
    2      -1                            1003 sub  $1, %bx
    2      -1   ------ Interrupt ------  ------ Interrupt ------  
    2      -1   1004 test $0, %bx
    2      -1   1005 jgt .top
    2      -1   ------ Interrupt ------  ------ Interrupt ------  
    2      -1                            1004 test $0, %bx
    2      -1                            1005 jgt .top
    2      -1   ------ Interrupt ------  ------ Interrupt ------  
    2      -1   1006 halt
    2      -1   ----- Halt;Switch -----  ----- Halt;Switch -----  
    2      -1                            1006 halt

---

ARG seed 1
ARG numthreads 2
ARG program looping-race-nolock.s
ARG interrupt frequency 4
ARG interrupt randomness True
ARG argv 
ARG load address 1000
ARG memsize 128
ARG memtrace 2000
ARG regtrace bx
ARG cctrace False
ARG printstats False
ARG verbose False

 2000      bx          Thread 0                Thread 1         
    0       0   
    0       0   1000 mov 2000, %ax
    0       0   ------ Interrupt ------  ------ Interrupt ------  
    0       0                            1000 mov 2000, %ax
    0       0                            1001 add $1, %ax
    1       0                            1002 mov %ax, 2000
    1      -1                            1003 sub  $1, %bx
    1       0   ------ Interrupt ------  ------ Interrupt ------  
    1       0   1001 add $1, %ax
    1       0   1002 mov %ax, 2000
    1      -1   1003 sub  $1, %bx
    1      -1   1004 test $0, %bx
    1      -1   ------ Interrupt ------  ------ Interrupt ------  
    1      -1                            1004 test $0, %bx
    1      -1                            1005 jgt .top
    1      -1   ------ Interrupt ------  ------ Interrupt ------  
    1      -1   1005 jgt .top
    1      -1   1006 halt
    1      -1   ----- Halt;Switch -----  ----- Halt;Switch -----  
    1      -1   ------ Interrupt ------  ------ Interrupt ------  
    1      -1                            1006 halt

只用以上两个例子可以看出，由于两个线程都在访问和修改同一个内存地址2000的值，所以`mov 2000 %ax`到`mov %ax, 2000`这之间的指令，
需要保证捆绑在一起来保证原子性，否则就会出现竞态条件，导致最终结果不可预测。
在这两个例子中，最终结果分别是1和2，说明竞态条件确实存在，并且线程调度的不同会影响最终结果。
换句话说，临界区就在线程之间共享的内存访问和修改操作中，如果不加锁保护，就会出现竞态条件。

7. `./x86.py -p looping-race-nolock.s -a bx=1 -t 2 -M 2000 -i 1 -R bx`

ARG seed 0
ARG numthreads 2
ARG program looping-race-nolock.s
ARG interrupt frequency 1
ARG interrupt randomness False
ARG argv bx=1
ARG load address 1000
ARG memsize 128
ARG memtrace 2000
ARG regtrace bx
ARG cctrace False
ARG printstats False
ARG verbose False

 2000      bx          Thread 0                Thread 1         
    0       1   
    0       1   1000 mov 2000, %ax
    0       1   ------ Interrupt ------  ------ Interrupt ------  
    0       1                            1000 mov 2000, %ax
    0       1   ------ Interrupt ------  ------ Interrupt ------  
    0       1   1001 add $1, %ax
    0       1   ------ Interrupt ------  ------ Interrupt ------  
    0       1                            1001 add $1, %ax
    0       1   ------ Interrupt ------  ------ Interrupt ------  
    1       1   1002 mov %ax, 2000
    1       1   ------ Interrupt ------  ------ Interrupt ------  
    1       1                            1002 mov %ax, 2000
    1       1   ------ Interrupt ------  ------ Interrupt ------  
    1       0   1003 sub  $1, %bx
    1       1   ------ Interrupt ------  ------ Interrupt ------  
    1       0                            1003 sub  $1, %bx
    1       0   ------ Interrupt ------  ------ Interrupt ------  
    1       0   1004 test $0, %bx
    1       0   ------ Interrupt ------  ------ Interrupt ------  
    1       0                            1004 test $0, %bx
    1       0   ------ Interrupt ------  ------ Interrupt ------  
    1       0   1005 jgt .top
    1       0   ------ Interrupt ------  ------ Interrupt ------  
    1       0                            1005 jgt .top
    1       0   ------ Interrupt ------  ------ Interrupt ------  
    1       0   1006 halt
    1       0   ----- Halt;Switch -----  ----- Halt;Switch -----  
    1       0   ------ Interrupt ------  ------ Interrupt ------  
    1       0                            1006 halt

共享变量的最终值为1，因为两个线程都只执行了一次循环，线程之间还是存在竞态条件。
只有在中断频率足够高，能够保证共享数据的操作的原子性的情况下，才能避免竞态条件的发生。

8. `./x86.py -p looping-race-nolock.s -t 2 -M 2000 -a bx=10 -i 4 -R bx`

ARG seed 0
ARG numthreads 2
ARG program looping-race-nolock.s
ARG interrupt frequency 4
ARG interrupt randomness False
ARG argv bx=10
ARG load address 1000
ARG memsize 128
ARG memtrace 2000
ARG regtrace bx
ARG cctrace False
ARG printstats False
ARG verbose False

 2000      bx          Thread 0                Thread 1         
    0      10   
    0      10   1000 mov 2000, %ax
    0      10   1001 add $1, %ax
    1      10   1002 mov %ax, 2000
    1       9   1003 sub  $1, %bx
    1      10   ------ Interrupt ------  ------ Interrupt ------  
    1      10                            1000 mov 2000, %ax
    1      10                            1001 add $1, %ax
    2      10                            1002 mov %ax, 2000
    2       9                            1003 sub  $1, %bx
    2       9   ------ Interrupt ------  ------ Interrupt ------  
    2       9   1004 test $0, %bx
    2       9   1005 jgt .top
    2       9   1000 mov 2000, %ax
    2       9   1001 add $1, %ax
    2       9   ------ Interrupt ------  ------ Interrupt ------  
    2       9                            1004 test $0, %bx
    2       9                            1005 jgt .top
    2       9                            1000 mov 2000, %ax
    2       9                            1001 add $1, %ax
    2       9   ------ Interrupt ------  ------ Interrupt ------  
    3       9   1002 mov %ax, 2000
    3       8   1003 sub  $1, %bx
    3       8   1004 test $0, %bx
    3       8   1005 jgt .top
    3       9   ------ Interrupt ------  ------ Interrupt ------  
    3       9                            1002 mov %ax, 2000
    3       8                            1003 sub  $1, %bx
    3       8                            1004 test $0, %bx
    3       8                            1005 jgt .top

- 这中间太多了，省略部分输出

   15       0   ------ Interrupt ------  ------ Interrupt ------  
   15       0   1006 halt
   15       0   ----- Halt;Switch -----  ----- Halt;Switch -----  
   15       0                            1006 halt

由此例子可见，依旧有竞态条件存在，最终结果并非20，而是一个小于20的值，这是因为中断频率较小，不足以保证共享数据操作的原子性，
导致两个线程在访问和修改共享变量时相互干扰，出现竞态条件。
在这个例子中，需要将中断频率设置为 3 的倍数，才能保证每个线程在访问和修改共享变量时不会被中断，从而避免竞态条件的发生。

9. `./x86.py -p wait-for-me.s -a ax=1,ax=0 -R ax -M 2000`

ARG seed 0
ARG numthreads 2
ARG program wait-for-me.s
ARG interrupt frequency 50
ARG interrupt randomness False
ARG argv ax=1,ax=0
ARG load address 1000
ARG memsize 128
ARG memtrace 2000
ARG regtrace ax
ARG cctrace False
ARG printstats False
ARG verbose False

 2000      ax          Thread 0                Thread 1         
    0       1   
    0       1   1000 test $1, %ax
    0       1   1001 je .signaller
    1       1   1006 mov  $1, 2000
    1       1   1007 halt
    1       0   ----- Halt;Switch -----  ----- Halt;Switch -----  
    1       0                            1000 test $1, %ax
    1       0                            1001 je .signaller
    1       0                            1002 mov  2000, %cx
    1       0                            1003 test $1, %cx
    1       0                            1004 jne .waiter
    1       0                            1005 halt

线程例程 wait-for-me.s 实现了一个简单的线程同步机制，其中一个线程（signaller）设置一个共享变量（地址2000）来通知另一个线程（waiter）继续执行。

- 线程0（signaller）开始时 ax=1，表示它需要执行信号操作。它执行 test 指令检查 ax 的值，如果为1，则跳转到 .signaller 标签，执行 mov 指令将值1写入地址2000，表示信号已发送，然后线程0执行 halt 指令结束。
- 线程1（waiter）开始时 ax=0，表示它需要等待信号。它执行 test 指令检查 ax 的值，如果为0，则继续执行 mov 指令从地址2000读取值到寄存器 cx，然后执行 test 指令检查 cx 的值。如果 cx 不为1，则跳转到 .waiter 标签，继续等待信号；如果 cx 为1，则表示信号已收到，线程1执行 halt 指令结束。
- 最终，地址2000 的值为1，表示信号已发送，线程0 成功通知了线程1，线程1 在收到信号后也成功结束。

由此模拟了多线程通过共享内存实现 “等待 - 通知” 的同步协作机制。

10. `./x86.py -p wait-for-me.s -a ax=0,ax=1 -R ax -M 2000`

ARG seed 0
ARG numthreads 2
ARG program wait-for-me.s
ARG interrupt frequency 50
ARG interrupt randomness False
ARG argv ax=0,ax=1
ARG load address 1000
ARG memsize 128
ARG memtrace 2000
ARG regtrace ax
ARG cctrace False
ARG printstats False
ARG verbose False

 2000      ax          Thread 0                Thread 1         
    0       0   
    0       0   1000 test $1, %ax
    0       0   1001 je .signaller
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       1   ------ Interrupt ------  ------ Interrupt ------  
    0       1                            1000 test $1, %ax
    0       1                            1001 je .signaller
    1       1                            1006 mov  $1, 2000
    1       1                            1007 halt
    1       0   ----- Halt;Switch -----  ----- Halt;Switch -----  
    1       0   1002 mov  2000, %cx
    1       0   1003 test $1, %cx
    1       0   1004 jne .waiter
    1       0   1005 halt

这个例子改为线程 0 等待，线程 1 发送信号。线程 0 在循环中不断检查共享变量（地址2000）的值，直到线程 1 设置该值为1。
因为中断频率默认为 50，所以线程 0 先检查了很多次，始终没有收到信号，直到触发线程中断，线程 1 执行 mov 指令将值1写入地址2000，线程 0 才在下一次检查时发现信号已到达，最终线程 0 执行 halt 指令结束。

如果设置 -i 1000 的话，线程 0 会一直检查共享变量的值，直到中断发生，线程 1 才有机会执行信号操作，从而避免死等现象。
由此可见，一味的增大中断频率以保证共享数据操作的原子性，也会带来其它问题，导致CPU资源浪费，影响系统性能，所以需要权衡取舍。
