#! /usr/bin/env python3
"""
Generate a random address trace and run it through paging-policy.py with
multiple replacement policies. Useful for homework Q3 to see how FIFO, LRU,
OPT (and a few others) behave on the same random sequence.
"""

import argparse
import random
import re
import subprocess
import sys
from pathlib import Path


def generate_addresses(seed: int, num: int, maxpage: int) -> list[int]:
	"""Return a reproducible list of random page numbers."""
	rng = random.Random(seed)
	return [rng.randrange(maxpage) for _ in range(num)]


def run_policy(paging_py: Path, policy: str, addresses: list[int], cachesize: int, clockbits: int) -> tuple[int, int, float, str]:
	"""
	Run paging-policy.py for one policy and return (hits, misses, hitrate, raw_output).
	We feed the same pre-generated addresses so every policy sees identical input.
	"""
	addr_arg = ",".join(str(a) for a in addresses)
	cmd = [
		sys.executable,
		str(paging_py),
		"-a",
		addr_arg,
		"-n",
		str(len(addresses)),
		"-p",
		policy,
		"-C",
		str(cachesize),
		"-b",
		str(clockbits),
		"-m",
		str(max(addresses) + 1 if addresses else 1),
		"-c",
		"-N",
	]

	result = subprocess.run(cmd, capture_output=True, text=True, check=False)
	output = result.stdout + result.stderr

	match = re.search(r"FINALSTATS hits (\d+)\s+misses (\d+)\s+hitrate ([0-9.]+)", output)
	if not match:
		raise RuntimeError(f"Failed to parse paging-policy output for {policy}\n{output}")

	hits, misses, hitrate = match.groups()
	return int(hits), int(misses), float(hitrate), output


def parse_args() -> argparse.Namespace:
	parser = argparse.ArgumentParser(description="Compare paging policies on a random trace")
	parser.add_argument("--seed", type=int, default=0, help="Random seed for address generation")
	parser.add_argument("--num", type=int, default=20, help="Number of addresses to generate")
	parser.add_argument("--maxpage", type=int, default=10, help="Page numbers are in [0, maxpage)")
	parser.add_argument(
		"--policies",
		default="FIFO,LRU,OPT,RAND,CLOCK",
		help="Comma-separated policies to test (any supported by paging-policy.py)",
	)
	parser.add_argument("--cachesize", type=int, default=5, help="Cache size in pages")
	parser.add_argument("--clockbits", type=int, default=2, help="Clock bits for CLOCK policy")
	parser.add_argument("--show-run", action="store_true", help="Print raw simulator output for each policy")
	return parser.parse_args()


def main() -> None:
	args = parse_args()
	paging_py = Path(__file__).resolve().parent / "paging-policy.py"
	if not paging_py.exists():
		raise SystemExit(f"paging-policy.py not found at {paging_py}")

	addresses = generate_addresses(args.seed, args.num, args.maxpage)
	print(f"Random trace (seed={args.seed}, num={args.num}, maxpage={args.maxpage}):")
	print(" ".join(str(a) for a in addresses))
	print()

	policies = [p.strip() for p in args.policies.split(",") if p.strip()]
	print(f"Policies: {', '.join(policies)}")
	print(f"Cache size: {args.cachesize} pages")
	print()

	header = f"{'Policy':<8} {'Hits':>5} {'Misses':>7} {'Hit%':>7}"
	print(header)
	print("-" * len(header))

	for policy in policies:
		hits, misses, hitrate, output = run_policy(
			paging_py,
			policy,
			addresses,
			args.cachesize,
			args.clockbits,
		)
		print(f"{policy:<8} {hits:>5} {misses:>7} {hitrate:>6.2f}")
		if args.show_run:
			print("\n" + output + "\n")


if __name__ == "__main__":
	main()

