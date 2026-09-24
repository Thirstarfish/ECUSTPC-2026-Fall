# Checker 回归测试

在本题目录运行（Python 3.9+、支持 `__int128` 的 g++）：

```text
python tests/checker_tests.py
```

脚本自动编译当前 checker，逐例检查**准确的退出码**：0=OK、1=WA、2=PE、3=FAIL。任一不符则测试脚本非零退出；每例超时 15 秒也会失败。默认只在系统临时目录生成文件。

使用已有可执行文件，或导出可手工运行的测试数据：

```text
python tests/checker_tests.py --checker path/to/checker.exe
python tests/checker_tests.py --export path/to/cases
```

导出后，每个用例目录包含 `input.in`、`participant.out`、`jury.ans`、`expected.txt`。手工调用参数顺序为：

```text
checker.exe input.in participant.out jury.ans
```

覆盖样例、坐标极值、距离端点及相邻整数、斜向距离、恰好达到/差一对、重复点、格式错误、截断和多余输出、标准答案错误、多测、T=100、n=5000，以及固定随机种子的边界测试。

小规模几何用例使用独立的 Python 任意精度整数计算核验预期结果；最大规模构造通过独立检查足够多的单位边，并利用纵坐标间隔证明最小距离合法。
