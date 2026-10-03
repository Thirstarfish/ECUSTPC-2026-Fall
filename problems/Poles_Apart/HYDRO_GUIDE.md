# G 题「星之彼端」的 Hydro 配置

上传 `manager_hydro.cpp` 到 Hydro 的**评测额外文件**，并在题目配置中引用：

```yaml
type: communication
num_processes: 2
manager: manager_hydro.cpp
time: 8000ms
memory: 1024MB
```

用户额外文件留空。测试数据使用 `generator.cpp` 生成的完整输入格式：

```text
0
200000
<seed>
2000 43600
43600 条边
```

每个测试点的输入文件必须上传到 Hydro 的测试数据区域，并在题目 `config.yaml`
的 `cases` 中列出，例如：

```yaml
subtasks:
  - id: 1
    score: 100
    type: min
    cases:
      - input: '01'
```

manager 会把图发送给第一个进程，读取 200000 个四元环；随后将同一张图和由这些
四元环生成的 800000 个三元组查询发送给第二个进程，并将正确率作为 Hydro 得分。
选手程序应按题面使用 `send` 和 `receive` 两种模式输出结果。
