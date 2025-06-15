# 🖥️ System Monitor (Cross-Platform CLI)

A lightweight, cross-platform system monitor written in modern C++.
Displays real-time CPU and memory usage via a minimal CLI interface, using native APIs on Windows and Linux.

---

## 🚀 Features

- **Cross-platform** support (Windows & Linux)
- **Real-time** updates (every 1 second)
- Minimal, fast **CLI-based** output
- Modern C++ design (`std::thread`, RAII, clean encapsulation)
- Direct system access:
  - Linux: `/proc/stat` & `/proc/meminfo`
  - Windows: PDH & `GlobalMemoryStatusEx`

---

## 🛠️ Technologies

- **C++17**
- **Windows API**: PDH, `GlobalMemoryStatusEx`
- **Linux**: POSIX `/proc/` file parsing
- **Build tools**: Visual Studio 2022 (Windows), `g++` + `make` (Linux/WSL)

---

## 📂 Folder Structure

```
System Monitor/
├── CPUMonitor.{h,cpp}       # High-level CPU monitor
├── CPUReader.{h,cpp}        # Platform-specific CPU stats
├── MemoryMonitor.{h,cpp}    # High-level memory monitor
├── MemoryReader.{h,cpp}     # Platform-specific memory stats
├── main.cpp                 # Entry point
├── Makefile                 # Linux build script
├── System Monitor.sln       # Visual Studio solution
├── System Monitor.vcxproj   # Visual Studio project
└── README.md                # Project documentation
```

---

## 💻 Build & Run

### Windows (Visual Studio)

1. Open `System Monitor.sln` in Visual Studio  
2. Select **Release** or **Debug** configuration  
3. Build and run the solution

### Linux (Ubuntu / WSL)

```bash
sudo apt update
sudo apt install g++

git clone https://github.com/<your-username>/System-Monitor.git
cd "System Monitor"
make
./sysmon
```

---

## 📷 Sample Output

```
=== System Monitor Test ===
CPU Usage:    12.53%
Memory Usage: 37.80%
```

---

## 📚 References

- 📄 [Linux `/proc/stat` documentation](https://www.kernel.org/doc/Documentation/filesystems/proc.txt)
- 📄 [GlobalMemoryStatusEx – Microsoft Docs](https://learn.microsoft.com/windows/win32/api/sysinfoapi/nf-sysinfoapi-globalmemorystatusex)
- 📄 [PDH API – Microsoft Docs](https://learn.microsoft.com/windows/win32/perfctrs/performance-data-helper)

---

## 👨‍💻 Author

**Kacper Feister**  
[github.com/Thund3rborn](https://github.com/Thund3rborn)
