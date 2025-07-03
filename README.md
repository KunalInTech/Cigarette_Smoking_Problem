# 🚬 Cigarette Smokers Problem – C Solution

This repository provides multiple implementations of the classic **Cigarette Smokers Problem**, a synchronization scenario in operating systems. Implementations are available in both **C** and **C++** using **POSIX threads**, **semaphores**, **monitors**, and **bitmasking** techniques.

## 🔧 Problem Statement
- An agent places **two random ingredients** on a table (chosen from tobacco, paper, and match).
- Three smokers, each possessing **one unique ingredient**, attempt to complete their cigarette.
- The smoker with the missing third item picks up the other two, smokes, and notifies the agent.
- The process repeats, modeling **inter-process communication** and **synchronization**.

## 📁 Project Structure

cigarette_smoker_problem/

├── semaphore_solution.c                  # C solution using binary semaphores

├── monitor_solution.c                    # C monitor-based solution using mutexes & condition variables

├── standard_bitmasking_solution.c        # Bitmask approach in C

└── README.md                             # Documentation file

## 🚀 How to Compile and Run
> 🐧 For **Linux / WSL users**

### Prerequisites

> sudo apt update

> sudo apt install build-essential

### Compilation Commands

## C solutions
gcc semaphore_solution.c -o semaphore_solution -pthread

gcc monitor_solution.c -o monitor_solution -pthread

gcc standard_bitmasking_solution.c -o standard_bitmasking_solution -pthread

### Run

./semaphore_solution

./monitor_solution

./standard_bitmasking_solution

Use \`Ctrl + C\` to terminate, as these simulate infinite smoker-agent interactions.

## 🧠 Concepts Demonstrated
- 🧵 POSIX Threads (\`pthread\`)
- 🔁 Semaphores (\`sem_t\`)
- 🔒 Mutex Locks & Condition Variables
- 🎯 Bitmasking for State Tracking
- ⚙️ Resource Synchronization in Multithreaded Systems

## 📷 Terminal Preview

A sample run showing how the agent and smokers interact:

![Smoker terminal output](assets/terminal_output.png)


## 👨‍💻 Author
**Kunal Purohit**  
Made with ❤️ for operating systems enthusiasts

## 🌐 Connect

<p align="left">
  <a href="https://github.com/KunalInTech" target="_blank">
    <img src="https://img.shields.io/badge/GitHub-KunalInTech-181717?style=for-the-badge&logo=github" alt="GitHub Badge"/>
  </a>
  <a href="https://www.linkedin.com/in/kunal-purohit-a95a422b0/" target="_blank">
    <img src="https://img.shields.io/badge/LinkedIn-Kunal%20Purohit-0A66C2?style=for-the-badge&logo=linkedin&logoColor=white" alt="LinkedIn Badge"/>
  </a>
</p>

> Feel free to fork or star 🌟 the repo if you found this helpful!

