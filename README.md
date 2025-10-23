# 🧮 OS-OEL-NED

This repository contains the **Dot Product Program** implemented for the **Operating Systems Open-Ended Lab (OEL)** at **NED University of Engineering & Technology**.

---

## 📘 Brief Description

The **Open-Ended Lab (OEL)** in the Operating Systems course allows students to apply theoretical concepts in a practical way.  
In this particular OEL, we implement a **Parallel Dot Product** program in **C** using **POSIX Threads (pthreads)**.  
The goal is to understand **multithreading**, **mutex locks**, and how **race conditions** can affect shared data in concurrent execution.

---

## ⚙️ HOW TO COMPILE AND RUN THE CODE

### 💾 Save the File
Save your source code with a `.c` extension.

Example:

---

### 🧱 Step 1: Compile the Program
Run this command in your terminal:

```bash
gcc "file.c" -pthread -o dot

Example:

gcc dot_productthread.c -pthread -o dot

▶️ Step 2: Run the Executable File

After compilation, run the executable:

./dot


