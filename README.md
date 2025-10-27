
# Qt 定时器演示项目

## 🌟 项目简介
本项目是基于《Qt6 C++开发指南》中的示例练习改编的 Qt 小项目，主要用于学习和交流，更多内容推荐购买原书  
原书作者：王维波  
出版信息：人民邮电出版社
> ⚠️ 本项目仅用于学习参考，非商业用途，侵权可联系删除

这个项目主要展示了 **QTimer** 的使用方法，包括连续计时、单次计时、定时精度控制以及用户界面交互功能。通过该项目可以学习如何在 Qt 界面程序中使用定时器，并实现对计时事件的响应和日志记录。

项目界面主要使用 **QWidget** 和 **UI Designer** 构建，包含了实时显示当前时间和流逝时间的功能。

---

##  🧩 功能特点
1. **连续计时与单次计时**
   - 支持连续触发的定时器，也支持单次触发的定时器。
   - 可设置定时间隔（毫秒）。

2. **定时精度控制**
   - 提供三种定时器类型：
     - 精确计时（PreciseTimer）
     - 粗略计时（CoarseTimer）
     - 非常粗略计时（VeryCoarseTimer）

3. **时间显示**
   - 实时显示当前时间（小时/分钟/秒）。
   - 显示从计时开始经过的时间（毫秒级）。

4. **日志输出**
   - 将计时事件和流逝时间输出到 `QPlainTextEdit`。
   - 支持自动滚动到底部，用户查看历史记录时不会自动滚动打扰。

5. **UI 状态控制**
   - 根据程序运行状态自动启用/禁用按钮。
   - 运行中：只允许停止操作。
   - 停止时：允许启动或单次计时。

6. **声音提示**
   - 定时器触发时播放系统提示音。
### 下面大概了解一下本项目的几个模块

### **1️⃣ 用户滚动状态检测与自动滚动：**

```cpp
connect(ui->plainTextEdit->verticalScrollBar(), &QScrollBar::valueChanged,
        this, [this](int value)
        {
            QScrollBar *bar = ui->plainTextEdit->verticalScrollBar();
            m_userAtBottom = (value == bar->maximum());
        });
```

**精彩点：**

* **自动滚动智能化**

  * 这里通过 `valueChanged` 信号和 `QScrollBar::maximum()` 判断用户是否滚动到最底部。
  * 如果用户在查看历史记录（没到最底部），程序就不会打扰用户，自动滚动被禁用。
  * 如果用户在底部，程序会自动滚动到最新日志，实现“日志跟随”效果。
* **Lambda 表达式使用**

  * 使用了现代 C++ lambda 捕获 `[this]`，写法简洁，不需要单独定义槽函数。
  * 这体现了 Qt 与 C++11/14/17 的结合优势，代码可读性和可维护性都很高。
* **语法小巧思**

  * 注释里提醒 `(a == b)` 是计算结果，而不是字符串，这对初学者理解表达式非常有用。

> 这一段实现了 **“智能日志滚动”**，既不会打扰用户，又能保持界面自动更新，非常实用，也体现了 UI 设计的人性化。

---

### **2️⃣ UI 状态管理函数：**

```cpp
void Widget::setUiMode(UiMode mode)
{
    switch(mode)
    {
    case UiMode::Running:
        ui->btnStart->setEnabled(false);
        ui->btnOneShot->setEnabled(false);
        ui->btnStop->setEnabled(true);
        break;
    case UiMode::Stopped:
        ui->btnStart->setEnabled(true);
        ui->btnOneShot->setEnabled(true);
        ui->btnStop->setEnabled(false);
        break;
    }
}
```

**精彩点：**

* **集中管理 UI 状态**

  * 所有按钮的启用/禁用逻辑都统一到一个函数里，而不是在每个操作里分散写 `setEnabled`。
  * 提高了可维护性和可扩展性，例如以后加新模式，只需要在这里修改即可。
* **清晰的状态模式**

  * 定义 `UiMode::Running` 和 `UiMode::Stopped`，让程序状态一目了然。
  * UI 控件状态直接跟程序逻辑绑定，避免用户在不合适的时候操作按钮（比如正在计时时点击 Start）。
* **可扩展性**

  * 如果未来要增加更多模式（比如暂停、倒计时等），只需在 `switch` 里增加 case，非常方便。



---

## 项目结构
- `widget.h` / `widget.cpp`：核心逻辑，包括定时器启动、停止、单次触发以及 UI 更新。
- `ui_widget.h`：由 Qt Designer 自动生成的 UI 文件。
- `main.cpp`：程序入口，初始化主窗口。

---

## 运行环境
- Qt 5.x 或 Qt 6.x
- C++11 或以上
- 支持 Windows、Linux 和 macOS

---



## 项目截图

<img width="278" height="411" alt="972a7931-d9bf-4190-a149-4ea7531dbbec" src="https://github.com/user-attachments/assets/6726b6a5-8a64-4b63-ae67-7603a54cafac" /><img width="338" height="411" alt="0c1572bd-2b8e-47df-8496-12995e598d70" src="https://github.com/user-attachments/assets/406e68af-ff24-450a-9061-cce72363bc6e" />





