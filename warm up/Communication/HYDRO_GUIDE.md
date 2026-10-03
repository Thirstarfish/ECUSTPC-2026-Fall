# 在 Hydro 配置通信题

`manager_hydro.cpp` 是 Hydro 通信题的 manager；原有 `interactor.cpp`、`checker.cpp` 和选手代码保持不变。

## 配置

上传 `manager_hydro.cpp` 为 manager 源文件，在题目配置中填写：

```yaml
type: communication
num_processes: 2
manager: manager_hydro.cpp
time: 1000ms
memory: 1024MB
```

字段名以当前 Hydro 页面为准；关键是 `num_processes: 2` 和 `manager`。每个测试点的输入文件使用现有生成器格式：第一行是 `0`，第二行是整数 `x`（`0 <= x <= 10^18`），例如 `0\n42`。

## 评测协议

Hydro 将两个选手进程同时启动，并把四个参数传给 manager：`<进程0读> <进程0写> <进程1读> <进程1写>`，通常是 `/proc/self/fd/3`、`/proc/self/fd/4`、`/proc/self/fd/5`、`/proc/self/fd/6`。当前 manager 同时兼容这种路径和本地测试时的数字描述符。manager 会：

1. 向进程 0 发送 `send` 和 `x`，读取 13 个小写字母；
2. 向进程 1 发送 `receive` 和该字符串，读取整数；
3. 标准输出打印 `1`（正确）或 `0`（错误），标准错误输出说明。

Hydro 按 `score * manager_stdout` 计算得分，因此满分测试点必须输出 `1`。选手每次输出后都要换行并刷新。不要把 `interactor.cpp` 配成 manager，它是 testlib 交互器，接口不同。

## 本地检查

用正确解测试 `0`、`42`、`1000000000000000000`，再测试错误解、非法长度和非法字符，确认 manager 分别输出 `1` 或 `0`。

## `contest.yaml` 怎么处理

`contest.yaml` 是比赛（contest）层配置，不是题目的评测配置。通信题的
`type`、`num_processes`、`manager`、测试点和时空限制应写在该题目录的
`config.yaml`（或 Hydro 题目编辑页对应的题目配置）中，不要写进
`contest.yaml`。

如果你用 Hydro 的比赛导入功能，`contest.yaml` 只需要把题目加入比赛。例如：

```yaml
title: ECUST 2026
problems:
  - id: communication
    file: warm up/Communication/config.yaml
```

不同 Hydro 版本对比赛文件的字段名称可能不同；若导入器已经生成了
`contest.yaml`，保留原文件，只在题目列表中添加通信题。不要把
`manager_hydro.cpp` 的内容复制到 `contest.yaml`，也不要把
`num_processes` 当作比赛级并发数。

最小的题目 `config.yaml` 可以写成：

```yaml
type: communication
num_processes: 2
manager: manager_hydro.cpp
time: 1000ms
memory: 1024MB
```
