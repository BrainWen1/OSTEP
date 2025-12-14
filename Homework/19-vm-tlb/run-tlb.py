#!/usr/bin/env python3

import os

# if not os.path.exists('./a.out'):
#     os.system('gcc ./tlb.c')
n = 1000
p = 1

with open('./tlb.output', 'w') as f:
    while p < 3000:
        output = os.popen(f'./bin/tlb -p {p} -l {n}')
        out = output.read()
        print(out)
        f.write(out)
        p *= 2
