# Cache Coherence Simulator

This project simulates cache coherence mechanisms in a distributed system, providing an environment for understanding and testing coherence protocols.

---
Avik Halder (Roll No. 21EC39033) | 
Supreet Sahu (Roll No. 21EC39052) | 
Rounak Mandal (Roll No. 21EC39053) | 
Sankalp N Bharadwaj (Roll No. 21EC10063) | 
Hardik Chauhan (Roll No. 21EC39011) | 

## Getting Started

### Prerequisites
Before you begin, ensure you have the following installed on your system:
- **Git**: Download and install from [Git](https://git-scm.com/).
- **G++ Compiler**: Part of the GCC toolchain. Install via:
  - **Windows**: Install [MinGW](http://www.mingw.org/) and ensure `g++` is added to your PATH.
  - **Linux**: Use `sudo apt install g++`.
  - **MacOS**: Install via Xcode Command Line Tools: `xcode-select --install`.

---

### Steps to Clone, Compile, and Run

You can copy and paste the following commands as a single block to get started:

```bash
git clone https://github.com/AvikH08/Cache_Coherence.git
cd Cache_Coherence
g++ main.cpp Processor.cpp Cache.cpp Bus.cpp Directory.cpp MainMemory.cpp AddressUtils.cpp -o main
```
### For Windows
```bash
.\main
```
### For Linux/MacOS
```bash
./main
```

