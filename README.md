# 计算器 (Calculator)

A simple calculator with a graphical interface written in C using GTK3.

使用 GTK3 编写的带图形界面的简易计算器。

---

## 功能 (Features)

- 四则运算：加、减、乘、除
- 小数点支持
- 退格键 (DEL) 删除最后一位
- 清除键 (C) 重置计算器
- Basic arithmetic: add, subtract, multiply, divide
- Decimal point support
- Backspace (DEL) to delete the last digit
- Clear (C) to reset the calculator

---

## 编译 (Build)

需要安装 GTK3 开发库：

```bash
# Debian / Ubuntu
sudo apt-get install libgtk-3-dev

# Fedora
sudo dnf install gtk3-devel

# Arch Linux
sudo pacman -S gtk3
```

编译：

```bash
gcc -o calculator calculator.c $(pkg-config --cflags --libs gtk+-3.0)
```

---

## 运行 (Run)

```bash
./calculator
```

---

## 界面布局 (Layout)

```
┌─────────┬───┬───┬───┐
│    C    │ / │ * │ - │
├─────────┼───┼───┼───┤
│    7    │ 8 │ 9 │ + │
├─────────┼───┼───┼───┤
│    4    │ 5 │ 6 │DEL│
├─────────┼───┼───┼───┤
│    1    │ 2 │ 3 │ = │
├────┬────┼───┼───┤   │
│    0    │ . │   │   │
└────┴────┴───┴───┴───┘
```

- `=` 按钮跨两行
- `0` 按钮跨两列

---

## 许可证 (License)

MIT
