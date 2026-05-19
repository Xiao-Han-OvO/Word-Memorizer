# Word Memorizer 3.0 - alapha rc1

本版本已将原 GTKmm 界面重写为 **C++ 逻辑层 + QML 表现层**：

- C++：词库读取、随机出题、判题、错词本、进度统计、导入导出、编辑器保存。
- QML：现代化 Qt Quick 界面、卡片布局、深色/浅色主题、抽屉式错词本、词库编辑器。

## 词库格式

每行一个单词，使用 `|` 分隔：

```txt
word|pos|meaning|example
abandon|v.|放弃；抛弃|Never abandon your dream.
```

## 构建

需要 Qt 6.2+（Core / Gui / Qml / Quick / QuickControls2）和 CMake 3.16+。

```bash
cmake -S . -B build
cmake --build build -j
./build/bin/word_memorizer
```

也兼容从旧目录构建：

```bash
cmake -S "Word Memorizer" -B build
cmake --build build -j
./build/bin/word_memorizer
```

## 快捷键

- `Alt+O`：打开词库
- `Alt+S`：查看答案
- `Ctrl+Tab`：下一个单词
- `Ctrl+Shift+D`：切换深色/浅色主题

## 功能

- 打开 `.txt` 单词库并开始拼写练习
- 答错自动加入错词本
- 查看答案会加入错词本
- 导出错词本
- 重置学习进度
- 新建、导入、编辑、保存词库
