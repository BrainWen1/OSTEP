- # 帮助信息
    `./process-run.py -h`

- # 含义：2个进程，每个10条指令，100%是CPU计算（无IO），仅进程结束时切换
	`./process-run.py -l 10:100,10:100 -S SWITCH_ON_END -c`

- # 含义：进程1的指令是“计算7次→IO→计算1次→IO”，IO耗时5个时钟周期，IO时切换
	`./process-run.py -P c7,i,c1,i -L 5 -S SWITCH_ON_IO -c`

1. `./process-run.py -l 5:100,5:100`
    两个进程：每个都是五个CPU指令，并且都是100%CPU计算

2. `./process-run.py -l 4:100,1:0`
    两个进程：第一个有四个CPU指令，并且是100%CPU计算；第二个有一个CPU指令，0%的CPU指令，即发起一个IO请求

3. `./process-run.py -l 1:0,4:100`
    和上一题一样的进程，但是发起IO请求的进程先执行，此时发起IO请求后在等待IO结束的五个时间片里CPU会切换到进程2执行计算，从而缩减总时间片，提高CPU利用效率

4. `./process-run.py -l 1:0,4:100 -c -S SWITCH_ON_END`
    比较上一题添加了只在进程结束时进行进程切换，此时先执行的发起IO请求的进程会一直占用CPU知道IO完成进程结束，然后才会执行进程2进行计算

5. `./process-run.py -l 1:0,4:100 -c -S SWITCH_ON_IO`
    修改为等待IO时切换进程，于是可以在进程1阻塞时切换到进程2进行计算

6. `./process-run.py -l 3:0,5:100,5:100,5:100 -S SWITCH_ON_IO -I IO_RUN_LATER -cp`
    在进程1等待IO时，会切换到进程2进行计算，在第6个时间片时进程1的IO完成、进程2的计算也完成，但是不会直接切换回进程1继续执行指令，而是顺延到进程3和进程4依次执行计算，最后再回到进程1再次发起IO请求，此时其他进程都已经执行完毕，所以不会把CPU切换到其他进程计算，而是一直等待进程1的IO完成，CPU利用率低

7. `./process-run.py -l 3:0,5:100,5:100,5:100 -S SWITCH_ON_IO -I IO_RUN_IMMEDIATE -cp`
    修改为立即切换回发起IO的进程继续执行指令，此时进程1发起第一次IO并等待时，会切换到进程2进行计算，第一次IO结束时，立即切换回进程1并且发起第二次IO，CPU又切换到进程3进行计算，进程4同理。此种情况下，CPU利用率为100%，资源利用效率非常高。至于 “为什么运行一个刚刚完成 I/O 的进程会是一个好主意？” 的问题，是因为有较大可能这个进程会再次发起IO请求，花费一个CPU时间片发起IO请求后切换到其他进程计算，会使资源利用效率显著提高

8. `./process-run.py -s 666 -l 3:50,3:50 -cp -I IO_RUN_LATER`
    -s number: 用种子number伪随机生成一组进程信息并且执行，通过 -I IO_RUN_LATER 或 IO_RUN_IMMEDIATE，以及 -S SWITCH_ON_IO 或 -S SWITCH_ON_END 来控制执行规则，多次运行确定清楚了执行规则
