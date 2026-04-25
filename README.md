# FreeRTOS Scheduling Extension

## 📌 Overview
This project implements a **modular real-time scheduler extension for FreeRTOS**, adding support for multiple scheduling policies:

- Rate Monotonic (RM)
- Deadline Monotonic (DM)
- Earliest Deadline First (EDF)
- FIFO

FreeRTOS natively supports **fixed-priority preemptive scheduling**, but does not provide built-in support for these real-time scheduling algorithms.  
This project extends the kernel with minimal modifications to enable dynamic and configurable scheduling policies.

---

## 🎯 Objectives

- Implement **RM, DM, EDF scheduling policies** inside FreeRTOS
- Maintain compatibility with the existing kernel
- Provide a **modular and extensible architecture**
- Validate behavior through:
  - PC simulation (logs + Gantt charts)
  - STM32 embedded execution (oscilloscope)

---

## 🧠 Key Concepts

Each real-time task is defined by:

- **Period (Ti)**
- **Deadline (Di)**
- **WCET (Ci)** (Worst Case Execution Time)

The scheduler ensures tasks meet their deadlines depending on the selected policy.

---

## 🏗️ Architecture

The project follows a layered architecture:
- **Application → main.c**
- **Scheduler Engine → scheduler.c / scheduler.h**
- **Policies → policies.c**
- **FreeRTOS Kernel → tasks.c (modified)**


### 🔧 Key Design Choices

- Only **one FreeRTOS file modified**: `tasks.c`
- Use of **hooks** to inject scheduling logic:
  - Task registration
  - Tick hook (task activation)
  - Priority update before context switch
- Full encapsulation: **TCB is never exposed**

---

## ⚙️ Scheduling Policies

### 🔵 Rate Monotonic (RM)
- Fixed priority
- Shorter period ⇒ higher priority

### 🟢 Deadline Monotonic (DM)
- Fixed priority
- Shorter deadline ⇒ higher priority

### 🔴 Earliest Deadline First (EDF)
- Dynamic priority
- Earliest absolute deadline ⇒ highest priority

### 🟡 FIFO
- Non-preemptive
- First activated ⇒ first executed

---

## 🔄 Execution Flow

1. Tasks are created using `xRTTaskCreate()`
2. Tasks are initially suspended
3. At each system tick:
   - Tasks are released if needed
4. Before each context switch:
   - Priorities are recomputed
5. FreeRTOS selects the highest-priority ready task

---

## 🧩 Key Features

- Minimal kernel intrusion
- Policy selection at compile time:
```c
#define configUSE_RM   1
#define configUSE_DM   0
#define configUSE_EDF  0
#define configUSE_FIFO 0
```

- Real-time task parameters stored inside TCB:
```c
typedef struct {
    TickType_t period;
    TickType_t deadline;
    TickType_t wcet;
    TickType_t next_release;
    TickType_t absolute_deadline;
    UBaseType_t release_order; 
} RT_Params_t;
```

