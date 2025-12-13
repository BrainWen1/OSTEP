1. `./paging-linear-translate.py -P 1k -a 1m -p 512m -v -n 0`
ARG seed 0
ARG address space size 1m
ARG phys mem size 512m
ARG page size 1k
ARG verbose True
ARG addresses -1


The format of the page table is simple:
The high-order (left-most) bit is the VALID bit.
  If the bit is 1, the rest of the entry is the PFN.
  If the bit is 0, the page is not valid.
Use verbose mode (-v) if you want to print the VPN # by
each entry of the page table.

Page Table (from entry 0 down to the max size)
  [       0]  0x8006104a
  [       1]  0x00000000
  [       2]  0x00000000
  [       3]  0x80033d4e
  [       4]  0x80026d2f
  [       5]  0x00000000
......太多了省略了
  [    1019]  0x8002e9c9
  [    1020]  0x00000000
  [    1021]  0x00000000
  [    1022]  0x00000000
  [    1023]  0x00000000

Virtual Address Trace

For each virtual address, write down the physical address it translates to
OR write down that it is an out-of-bounds address (e.g., segfault).

---

`./paging-linear-translate.py -P 1k -a 2m -p 512m -v -n 0`

  [       0]  0x8006104a
  [       1]  0x00000000
  [       2]  0x00000000
  [       3]  0x80033d4e
  [       4]  0x80026d2f
  [       5]  0x00000000
......
  [    2043]  0x00000000
  [    2044]  0x00000000
  [    2045]  0x00000000
  [    2046]  0x8000eedd
  [    2047]  0x00000000

`./paging-linear-translate.py -P 2k -a 1m -p 512m -v -n 0`

  [       0]  0x80030825
  [       1]  0x00000000
  [       2]  0x00000000
  [       3]  0x80019ea7
  [       4]  0x80013697
  [       5]  0x00000000
......
  [     507]  0x00000000
  [     508]  0x8001a7f2
  [     509]  0x8001c337
  [     510]  0x00000000
  [     511]  0x00000000

2. 随着每个地址空间中的页的百分比增大，即 -u，生成的虚拟地址的有效率随之上升。

`paging-linear-translate.py -P 1k -a 16k -p 32k -v -u 0`
`paging-linear-translate.py -P 1k -a 16k -p 32k -v -u 25`
`paging-linear-translate.py -P 1k -a 16k -p 32k -v -u 50`
`paging-linear-translate.py -P 1k -a 16k -p 32k -v -u 75`
`paging-linear-translate.py -P 1k -a 16k -p 32k -v -u 100`

3. 这些参数组合都是现实的，所有页都能正确存放在内存中。

`./paging-linear-translate.py -P 8  -a 32   -p 1024 -v -s 1`
`./paging-linear-translate.py -P 8k -a 32k  -p 1m   -v -s 2`
`./paging-linear-translate.py -P 1m -a 256m -p 512m -v -s 3`

做一个示例，第三行命令：
页大小为 1 Mb，对应 1024 * 1024 个字节，也即需要 20 位来表示一个页中的偏移量；
其次地址空间总大小为 256 Mb，所以一共可以分为 256 个页，所以需要 8 位来表示所有的页号。

对于这样一个虚拟地址：VA 0x0308b24d (decimal: 50901581) --> 1f68b24d (decimal 526955085) [VPN 48]
十六进制 0x0308b24d -> 二进制 0b11000010001011001001001101
低 20 位：0b10001011001001001101 -> 569933 -> 偏移量
高 8 位（省略前导零）：0b110000 -> 48 -> 页号

4. 地址空间≥物理内存时，程序直接报错退出。

其他限制：
    - 地址空间、页大小必须是 2 的幂
    - 地址空间、物理内存必须是页大小的整数倍
    - 物理内存 / 地址空间不能超过 1GB
    - 物理内存 / 地址空间不能小于等于 1

