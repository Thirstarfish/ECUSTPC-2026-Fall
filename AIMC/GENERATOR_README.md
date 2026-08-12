# Quantum Stabilizer Initialization - Test Generation Guide

本文档说明如何为"Quantum Stabilizer Initialization"题目生成测试数据。

## 文件清单

- `generator.cpp` - 基础随机数据生成器
- `gen-adv.cpp` - 高级生成器（支持多种策略）
- `validator_polygon.cpp` - 输入数据验证器
- `tests_example.txt` - 测试脚本示例

## 生成器使用说明

### 1. 基础生成器 (generator.cpp)

**编译:**
```bash
g++ -std=c++17 -O2 generator.cpp -o gen
```

**用法:**
```
gen <N> <M> <H> <seed> [max_energy]
```

**参数:**
- `N, M, H`: 网格维度 (1 ≤ N,M,H ≤ 20, N×M×H ≤ 8000)
- `seed`: 随机种子（保证可重复性）
- `max_energy`: 最大能量值（可选，默认为 10^9）

**示例:**
```bash
gen 5 5 5 123 > test01.txt
gen 10 10 8 456 1000 > test02.txt
gen 20 20 20 789 > test03.txt
```

### 2. 高级生成器 (gen-adv.cpp)

**编译:**
```bash
g++ -std=c++17 -O2 gen-adv.cpp -o gen-adv
```

**用法:**
```
gen-adv <N> <M> <H> <seed> <strategy> [params...]
```

**策略类型:**

| 策略 | 描述 | 额外参数 |
|------|------|----------|
| 0 | 均匀随机 [0, 10^9] | 无 |
| 1 | 小值 [0, 100] | 无 |
| 2 | 大值 [10^8, 10^9] | 无 |
| 3 | 稀疏（80%为0，20%为大值） | 无 |
| 4 | 偏向性（不同矩阵不同范围） | 无 |
| 5 | 自定义范围 [min, max] | min, max |

**策略4详细说明:**
- 矩阵 A1, A2 (本征态): 中等值 [1000, 100000]
- 矩阵 A3, A4 (S1稳定簇): 高值 [100000, 10^9]
- 矩阵 A5, A6 (S2稳定簇): 低值 [0, 10000]
- 矩阵 A7, A8 (S3稳定簇): 中等值 [10000, 500000]

**示例:**
```bash
gen-adv 10 10 10 123 0 > test04.txt          # 均匀随机
gen-adv 15 15 10 456 1 > test05.txt          # 小值
gen-adv 20 20 20 789 2 > test06.txt          # 大值
gen-adv 12 12 12 111 3 > test07.txt          # 稀疏
gen-adv 8 8 8 222 4 > test08.txt             # 偏向性
gen-adv 10 10 10 333 5 1000 5000 > test09.txt  # 自定义范围
```

### 3. 验证器 (validator_polygon.cpp)

**编译:**
```bash
g++ -std=c++17 -O2 validator_polygon.cpp -o validator
```

**用法:**
```bash
./validator < test01.txt
```

验证器会检查：
- N, M, H 在 [1, 20] 范围内
- N×M×H ≤ 8000
- 所有能量值在 [0, 10^9] 范围内
- 输入格式正确（空行位置、行数等）

## 在 Polygon 上使用

### 步骤1: 上传文件

在 Polygon 中上传以下文件到 "Files" 部分：
- `testlib.h` (如果还没有)
- `generator.cpp`
- `gen-adv.cpp`
- `validator_polygon.cpp`

### 步骤2: 配置资源

在 "Resources" 部分：
1. 添加生成器：
   - Name: `gen`
   - Source: `generator.cpp`
   - Type: `generator`

2. 添加高级生成器：
   - Name: `gen-adv`
   - Source: `gen-adv.cpp`
   - Type: `generator`

3. 设置验证器：
   - Name: `validator`
   - Source: `validator_polygon.cpp`
   - Type: `validator`

### 步骤3: 编写测试脚本

在 "Tests" 部分的脚本中使用：

```bash
# 手动测试（如果有）
# 在 "Manual Tests" 部分上传文件后引用

# 使用生成器
gen 1 1 1 1001 100 > 1
gen 5 5 5 1002 > 2
gen 10 10 8 2001 > 3
gen-adv 15 15 10 3001 3 > 4
gen-adv 20 20 20 4001 5 0 1000000 > 5
```

### 步骤4: 生成测试

点击 "Generate Tests" 按钮，Polygon 会：
1. 编译生成器
2. 运行每行命令
3. 使用验证器验证输出
4. 运行标准程序生成答案

## 测试数据设计建议

### 基础测试 (1-5)
- 极小网格 (1×1×1, 2×2×2)
- 小网格 (3×3×3, 5×5×5)
- 手动构造的特殊情况

### 中等测试 (6-20)
- 不同形状的网格 (10×10×8, 15×10×5, 20×10×4)
- 不同策略的数据分布
- 不同能量值范围

### 边界测试 (21-30)
- 最大网格 (20×20×20)
- 一维形状 (20×1×1, 1×20×1, 1×1×20)
- 二维形状 (20×20×1, 20×1×20, 1×20×20)
- 极端能量值（全0、全最大值）

### 随机测试 (31-40)
- 使用不同随机种子
- 覆盖各种策略
- 确保数据多样性

## 常见问题

**Q: 如何确保测试数据的多样性？**
A: 使用不同的随机种子和策略组合，包括均匀分布、偏向分布、稀疏分布等。

**Q: 如何测试极端情况？**
A: 使用 gen-adv 的策略2（大值）和策略3（稀疏），以及一维/二维网格配置。

**Q: 生成器运行失败怎么办？**
A: 检查参数是否符合约束（N×M×H ≤ 8000），确保 testlib.h 已上传。

**Q: 如何调试生成的数据？**
A: 在本地运行生成器并用验证器检查：
```bash
./gen 5 5 5 123 > test.txt
./validator < test.txt
```

## 性能考虑

- 最大网格 20×20×20 = 8000 个量子比特
- 每个测试有 8 个矩阵，共 8×8000 = 64000 个整数
- 生成器应在1秒内完成
- 验证器应在1秒内完成

## 联系与反馈

如有问题或建议，请联系题目作者。
