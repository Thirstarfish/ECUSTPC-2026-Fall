"""Generate a UOJ-style problem config into 1.txt.

Usage:
    python gen_config.py          # prompts for the case count
    python gen_config.py 16       # generate directly with 16 cases
"""

import sys


TEMPLATE = """\
type: default
time: 2000ms
memory: 1024MB
checker_type: testlib
checker:
  file: check.cpp
  lang: auto
subtasks:
- score: 100
  if: []
  id: 1
  type: min
  cases:
{cases}
"""


def make_cases(n: int) -> str:
    lines = []
    for i in range(1, n + 1):
        name = "%02d" % i
        lines.append("  - input: '%s'" % name)
        lines.append("    output: %s.a" % name)
    return "\n".join(lines)


def main() -> None:
    if len(sys.argv) > 1:
        n = int(sys.argv[1])
    else:
        n = int(input("请输入用例数量: ").strip())

    with open("1.txt", "w", encoding="utf-8") as f:
        f.write(TEMPLATE.format(cases=make_cases(n)))

    print("已生成 1.txt，共 %d 个用例" % n)


if __name__ == "__main__":
    main()
