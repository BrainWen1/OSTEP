- 模拟器 paging-policy.py 允许你使用不同的页替换策略。

1. 使用以下参数生成随机地址：-s 0 -n 10，-s 1 -n 10 和-s 2 -n 10。将策略从 FIFO 更改为 LRU，并将其更改为 OPT。
    计算所述地址追踪中的每个访问是否命中或未命中。

答：`./paging-policy.py -n 10 -s 0 -c`ARG addresses -1
ARG addressfile 
ARG numaddrs 10
ARG policy FIFO
ARG clockbits 2
ARG cachesize 3
ARG maxpage 10
ARG seed 0
ARG notrace False

Assuming a replacement policy of FIFO, and a cache of size 3 pages,
figure out whether each of the following page references hit or miss
in the page cache.

Access: 8  Hit/Miss?  State of Memory?
Access: 7  Hit/Miss?  State of Memory?
Access: 4  Hit/Miss?  State of Memory?
Access: 2  Hit/Miss?  State of Memory?
Access: 5  Hit/Miss?  State of Memory?
Access: 4  Hit/Miss?  State of Memory?
Access: 7  Hit/Miss?  State of Memory?
Access: 3  Hit/Miss?  State of Memory?
Access: 4  Hit/Miss?  State of Memory?
Access: 5  Hit/Miss?  State of Memory?

- FIFO 策略结果：m -> miss / h -> hit
    m m m m m h m m m m -> 1/10 = 10% hit rate
- LRU 策略结果：
    m m m m m h m m h m -> 2/10 = 20% hit rate
- OPT 策略结果：
    m m m m m h h m h h -> 4/10 = 40% hit rate

---

`./paging-policy.py -n 10 -s 1 -c`
ARG addresses -1
ARG addressfile 
ARG numaddrs 10
ARG policy FIFO
ARG clockbits 2
ARG cachesize 3
ARG maxpage 10
ARG seed 1
ARG notrace False

Assuming a replacement policy of FIFO, and a cache of size 3 pages,
figure out whether each of the following page references hit or miss
in the page cache.

Access: 1  Hit/Miss?  State of Memory?
Access: 8  Hit/Miss?  State of Memory?
Access: 7  Hit/Miss?  State of Memory?
Access: 2  Hit/Miss?  State of Memory?
Access: 4  Hit/Miss?  State of Memory?
Access: 4  Hit/Miss?  State of Memory?
Access: 6  Hit/Miss?  State of Memory?
Access: 7  Hit/Miss?  State of Memory?
Access: 0  Hit/Miss?  State of Memory?
Access: 0  Hit/Miss?  State of Memory?

- FIFO 策略结果：m -> miss / h -> hit
    m m m m m h m m m h -> 1/10 = 10% hit rate
- LRU 策略结果：
    m m m m m h m m m h -> 2/10 = 20% hit rate
- OPT 策略结果：
    m m m m m h m h m h -> 3/10 = 30% hit rate

---

`./paging-policy.py -n 10 -s 2 -c`
ARG addresses -1
ARG addressfile 
ARG numaddrs 10
ARG policy FIFO
ARG clockbits 2
ARG cachesize 3
ARG maxpage 10
ARG seed 2
ARG notrace False

Assuming a replacement policy of FIFO, and a cache of size 3 pages,
figure out whether each of the following page references hit or miss
in the page cache.

Access: 9  Hit/Miss?  State of Memory?
Access: 9  Hit/Miss?  State of Memory?
Access: 0  Hit/Miss?  State of Memory?
Access: 0  Hit/Miss?  State of Memory?
Access: 8  Hit/Miss?  State of Memory?
Access: 7  Hit/Miss?  State of Memory?
Access: 6  Hit/Miss?  State of Memory?
Access: 3  Hit/Miss?  State of Memory?
Access: 6  Hit/Miss?  State of Memory?
Access: 6  Hit/Miss?  State of Memory?

- FIFO 策略结果：m -> miss / h -> hit
    m h m h m m m m h h -> 4/10 = 40% hit rate
- LRU 策略结果：
    m h m h m m m m h h -> 4/10 = 40% hit rate
- OPT 策略结果：
    m h m h m m m m h h -> 4/10 = 40% hit rate

---

2. 对于大小为 5 的高速缓存，为以下每个策略生成最差情况的地址引用序列：FIFO、 LRU 和 MRU（最差情况下的引用序列导致尽可能多的未命中）。
    对于最差情况下的引用序列，需要的缓存增大多少，才能大幅提高性能，并接近 OPT？

答：
- FIFO 最差情况引用序列：循环
    1, 2, 3, 4, 5, 6, 1, 2, 3, 4, 5, 6, ...
    需要将缓存增大到 6 才能接近 OPT 性能。
- LRU 最差情况引用序列：循环
    1, 2, 3, 4, 5, 6, 1, 2, 3, 4, 5, 6, ...
    需要将缓存增大到 6 才能接近 OPT 性能。
- MRU 最差情况引用序列：交替
    1, 2, 3, 4, 5, 6, 5, 6, ...
    需要将缓存增大到 6 才能接近 OPT 性能。

---

3. 生成一个随机追踪序列（使用 Python 或 Perl）。你预计不同的策略在这样的追踪序列上的表现如何？

答：
- 示例一
`./random-sequence.py --seed 0 --num 200`
Random trace (seed=0, num=200, maxpage=10):
6 6 0 4 8 7 6 4 7 5 9 3 8 2 4 2 1 9 4 8 9 2 4 1 1 5 7 8 1 5 6 5 9 3 8 7 7 8 4 0 8 0 1 6 0 9 7 5 3 5 1 3 9 3 3 2 8 7 1 1 5 8 7 1 4 8 4 1 8 5 8 3 9 8 9 4 7 1 9 6 5 9 3 4 2 3 2 0 9 4 7 1 1 2 2 0 1 8 6 8 4 8 3 3 9 6 9 4 7 7 5 1 5 9 1 7 9 5 3 3 0 4 1 3 5 2 5 6 0 1 2 3 0 9 8 9 1 0 1 3 9 9 1 6 1 5 1 0 9 0 3 2 1 7 3 0 0 8 6 9 1 4 1 3 1 4 5 6 2 0 8 7 0 9 1 6 3 4 5 7 9 2 3 0 2 2 5 8 4 1 9 7 2 0 7 6 9 8 4 5

Policies: FIFO, LRU, OPT, RAND, CLOCK
Cache size: 5 pages

Policy    Hits  Misses    Hit%
------------------------------
FIFO        92     108   46.00
LRU         88     112   44.00
OPT        138      62   69.00
RAND        95     105   47.50
CLOCK       89     111   44.50

在这个随机追踪序列上，OPT 策略表现最好，因为它能够预见未来的页面请求，从而最大限度地减少未命中次数。FIFO 和 LRU 的表现相对较差，因为它们无法利用未来信息。
RAND 和 CLOCK 的表现介于两者之间，但仍然不如 OPT。总体而言，随着缓存大小的增加，所有策略的性能都会有所提升，但 OPT 的优势最为显著。

- 示例二
'`./random-sequence.py --seed 1 --num 200`'
Random trace (seed=1, num=200, maxpage=10):
2 9 1 4 1 7 7 7 6 3 1 7 0 6 6 9 0 7 4 3 9 1 5 0 0 0 8 0 6 3 6 0 8 3 7 7 8 3 5 3 3 7 4 0 6 8 1 2 4 1 5 8 6 8 3 4 4 9 7 8 6 9 0 7 3 6 6 2 5 8 5 1 7 8 1 2 8 6 5 7 0 7 0 4 9 9 9 6 2 2 8 3 0 3 8 8 3 6 8 5 9 5 7 4 8 9 0 6 8 2 8 8 3 6 0 7 5 9 8 3 8 6 7 5 6 5 0 8 8 9 9 5 7 9 0 3 2 8 9 2 1 8 4 0 1 1 0 7 0 4 3 4 1 9 2 5 4 1 2 2 4 8 2 4 4 7 5 7 7 1 0 4 6 5 6 3 4 1 4 8 3 9 6 0 3 0 6 2 0 2 7 8 6 8 3 8 7 3 8 0

Policies: FIFO, LRU, OPT, RAND, CLOCK
Cache size: 5 pages

Policy    Hits  Misses    Hit%
------------------------------
FIFO       105      95   52.50
LRU        101      99   50.50
OPT        145      55   72.50
RAND        96     104   48.00
CLOCK      103      97   51.50

从结果可以看出，OPT 策略再次表现最佳，显著优于其他策略。FIFO 和 LRU 的性能相似，但均低于 OPT。RAND 和 CLOCK 的表现也较为接近，但仍然不及 OPT。
总体而言，随机地址追踪序列显示出不同策略在处理不可预测的访问模式时的差异，OPT 由于其预见性，在各种情况下都能提供最佳性能。

- 示例三
`./random-sequence.py --seed 2 --num 200`
Random trace (seed=2, num=200, maxpage=10):
0 1 1 5 2 4 4 9 3 9 0 9 2 6 6 8 5 8 7 8 4 0 0 5 7 5 6 6 8 2 8 2 3 3 0 2 5 2 2 8 8 5 8 8 2 7 6 8 5 9 5 5 7 2 6 7 8 3 7 4 7 8 8 5 7 7 5 9 8 7 7 3 5 2 9 4 7 4 4 8 8 8 8 9 9 6 4 3 7 8 5 9 1 5 0 3 1 0 9 0 4 9 3 1 8 2 4 3 3 0 6 0 0 5 5 2 3 0 1 1 1 0 0 0 5 4 2 2 2 8 0 6 9 0 3 2 0 0 5 9 1 4 5 7 0 4 7 8 9 0 4 6 9 2 7 3 1 5 1 0 7 2 8 9 6 7 8 5 2 5 4 4 9 6 0 8 2 0 4 0 2 2 2 1 7 3 8 0 3 3 7 1 4 1 9 3 9 9 5 4

Policies: FIFO, LRU, OPT, RAND, CLOCK
Cache size: 5 pages

Policy    Hits  Misses    Hit%
------------------------------
FIFO       103      97   51.50
LRU        104      96   52.00
OPT        143      57   71.50
RAND       102      98   51.00
CLOCK      112      88   56.00

这次随机序列的结果，OPT 依旧是性能最佳，同时 CLOCK 方案是除了 OPT 外的最好性能，其他三种方案的表现比较接近。
由此可见 OPT 的性能一直是最优的存在，但是其他的策略的表现优劣都不是呈现一个固定的排名，而是随着序列的不同而有此消彼长的趋势。

---

4. 现在生成一些局部性追踪序列。如何能够产生这样的追踪序列？LRU 表现如何？RAND 比 LRU 好多少？CLOCK 表现如何？CLOCK 使用不同数量的时钟位，表现如何？

答：
- 局部性追踪序列的产生方式
需人为构造具备时间局部性或空间局部性的访问模式（而非纯随机序列）：
    时间局部性：核心重复访问小集合页（如1,2,3循环），偶尔引入新页，模拟程序循环逻辑；
    空间局部性：连续 / 反向访问相邻页（如0,1,2,3…遍历），模拟数组 / 内存块访问逻辑；
    混合局部性：结合 “小集合重复 + 相邻页访问”，贴近真实程序特征。

- LRU 策略的表现
LRU（最近最少使用）能精准利用局部性特征：
    时间局部性下，保留 “最近访问的页”，命中率显著高于无局部性感知的策略；
    空间局部性下，相邻页的近期访问特征被保留，仍能维持高命中率；
    是局部性序列下性能最优的实用策略（仅次于理论最优的 OPT）。

- RAND 与 LRU 的对比
RAND（随机替换）无局部性利用能力：
    命中率远低于 LRU（通常差距 20% 以上）；
    随机淘汰行为会浪费局部性带来的命中机会，无任何场景下优于 LRU。

- CLOCK 策略的表现
CLOCK（时钟算法）是 LRU 的高效近似实现：
    性能接近 LRU（命中率略低于 LRU），但实现开销（内存 / CPU）远低于 LRU；
    局部性序列下，命中率显著高于 FIFO、RAND，是 “性能 - 开销” 的最优折中。

- CLOCK 时钟位数量的影响
时钟位数量直接决定 CLOCK 贴近 LRU 的程度：
    时钟位越多（如 4 位＞2 位），能更精细记录页的 “近期访问程度”；
    更多时钟位会让 CLOCK 命中率更接近 LRU，性能提升明显。

- 核心性能梯度（理论）
    局部性序列下策略性能从高到低：OPT（理论最优） > LRU > CLOCK（多时钟位） > CLOCK（少时钟位） > RAND

---

5. 使用像 valgrind 这样的程序来测试真实应用程序并生成虚拟页面引用序列。例如，运行 valgrind --tool = lackey --trace-mem = yes ls 将为程序 ls 所做的每个指令和数据引用，输出近乎完整的引用追踪。为了使上述仿真器有用，你必须首先将每个虚拟内存引用转换为虚拟页码参考（通过屏蔽偏移量并向右移位来完成）。为了满足大部分请求，你的应用程序追踪需要多大的缓存？随着缓存大小的增加绘制其工作集的图形。

答：缓存大小需覆盖程序的工作集大小才能满足大部分请求；轻量级程序（如 ls）的工作集很小，缓存增大到一定程度后，工作集与命中率会趋于饱和。

`sort -n ./5/page_trace.txt | uniq | wc -l`
输出 128，表示 ls 的工作集大小为 128 个 4KB 页（约 512KB），只要缓存大小≥512KB，就能满足ls的大部分内存请求（命中率趋于饱和）。
通过增加缓存大小并记录命中率，可以绘制出工作集与命中率的关系图，通常会看到随着缓存大小的增加，命中率迅速上升，直到达到工作集大小后趋于平稳。
