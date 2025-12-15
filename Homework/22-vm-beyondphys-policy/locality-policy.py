#!/usr/bin/env python3

import argparse
import subprocess
import re
from pathlib import Path
from typing import Optional  # 导入Optional解决类型报错


# 定义3类局部性序列的配置（序列内容、长度、最大页号、序列名称）
SEQUENCES = {
    "时间局部性": {
        "addr": "1,2,3,1,2,3,1,2,3,4,1,2,3,4,5,1,2,3,5,6,1,2,3,6,7,1,2,3,7,8,1,2,3,8,9,1,2,3,9,10,1,2,3,10,11,1,2,3,11,12",
        "num": 50,
        "maxpage": 13
    },
    "空间局部性": {
        "addr": "0,1,2,3,4,5,4,3,2,1,0,1,2,3,4,5,6,7,8,7,6,5,4,3,2,1,0,1,2,3,4,5,6,7,8,9,8,7,6,5,4,3,2,1,0,1,2,3,4,5",
        "num": 50,
        "maxpage": 10
    },
    "混合局部性": {
        "addr": "5,6,7,5,6,7,8,9,8,9,5,6,7,8,9,10,11,10,11,5,6,7,8,9,12,13,12,13,5,6,7,8,9,14,15,14,15,5,6,7,8,9,16,17,16,17,5,6,7,8,9,18,19,18,19,5,6,7,8,9",
        "num": 60,
        "maxpage": 20
    }
}

# 固定配置（可根据需要调整）
CACHE_SIZE = 6  # 缓存大小
CLOCK_BITS_LIST = [2, 4]  # CLOCK策略测试的时钟位
PAGING_PY_PATH = Path("./paging-policy.py")  # paging-policy.py的路径


def run_paging_policy(policy: str, seq_config: dict, clock_bits: Optional[int] = None) -> dict:
    """
    运行paging-policy.py，返回解析后的统计结果
    :param policy: 替换策略（FIFO/LRU/OPT/RAND/CLOCK）
    :param seq_config: 序列配置（addr/num/maxpage）
    :param clock_bits: CLOCK策略的时钟位（仅CLOCK需要）
    :return: 包含hits/misses/hitrate的字典
    """
    # 构建命令
    cmd = [
        "python3",
        str(PAGING_PY_PATH),
        "-a", seq_config["addr"],
        "-n", str(seq_config["num"]),
        "-p", policy,
        "-C", str(CACHE_SIZE),
        "-m", str(seq_config["maxpage"]),
        "-c"
    ]
    # CLOCK策略添加时钟位参数
    if policy == "CLOCK" and clock_bits is not None:
        cmd.extend(["-b", str(clock_bits)])

    # 执行命令并捕获输出
    try:
        result = subprocess.run(
            cmd,
            capture_output=True,
            text=True,
            check=True
        )
        output = result.stdout + result.stderr
    except subprocess.CalledProcessError as e:
        raise RuntimeError(f"运行paging-policy.py失败：{e}\n输出：{e.stdout}\n错误：{e.stderr}")

    # 解析FINALSTATS（修复逻辑：不额外乘100，直接用paging-policy输出的数值）
    match = re.search(r"FINALSTATS hits (\d+)\s+misses (\d+)\s+hitrate ([0-9.]+)", output)
    if not match:
        raise RuntimeError(f"解析结果失败，输出：{output}")

    hits, misses, hitrate = match.groups()
    return {
        "hits": int(hits),
        "misses": int(misses),
        "hitrate": float(hitrate)  # 仅修复：去掉之前错误的×100，恢复你原本的逻辑
    }


def main():
    # 解析命令行参数
    parser = argparse.ArgumentParser(description="运行局部性序列的页面置换策略测试")
    parser.add_argument("policy", type=str, choices=["FIFO", "LRU", "OPT", "RAND", "CLOCK"],
                        help="要测试的置换策略（FIFO/LRU/OPT/RAND/CLOCK）")
    args = parser.parse_args()

    # 校验paging-policy.py是否存在
    if not PAGING_PY_PATH.exists():
        print(f"错误：未找到paging-policy.py，路径：{PAGING_PY_PATH.absolute()}")
        exit(1)

    # 打印基础信息（完全保留你的对齐代码）
    print("=" * 90)
    print(f"测试策略：{args.policy}")
    print(f"缓存大小：{CACHE_SIZE} 页")
    if args.policy == "CLOCK":
        print(f"CLOCK时钟位：{CLOCK_BITS_LIST}")
    print("=" * 90)
    # 调整列宽度，确保对齐（完全保留你的代码）
    header = f"{'序列类型':<15} {'命中数':>10} {'未命中数':>12} {'命中率(%)':>15} {'时钟位':>10}"
    print(header)
    print("-" * 90)

    # 遍历所有序列测试（完全保留你的对齐代码）
    for seq_name, seq_config in SEQUENCES.items():
        if args.policy == "CLOCK":
            # CLOCK策略测试多个时钟位
            for bits in CLOCK_BITS_LIST:
                stats = run_paging_policy(args.policy, seq_config, bits)
                print(f"{seq_name:<15} {stats['hits']:>10} {stats['misses']:>12} {stats['hitrate']:>20.2f}% {bits:>16}")
        else:
            # 其他策略直接测试
            stats = run_paging_policy(args.policy, seq_config)
            print(f"{seq_name:<15} {stats['hits']:>10} {stats['misses']:>12} {stats['hitrate']:>20.2f}% {'-':>16}")

    print("=" * 90)


if __name__ == "__main__":
    main()
