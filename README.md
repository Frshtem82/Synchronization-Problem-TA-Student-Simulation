# 🧵 Synchronization Problem – TA-Student Simulation

This project is a simulation of a classic synchronization problem in Operating Systems, modeling the interaction between a Teaching Assistant (TA) and undergraduate students using threads, semaphores, and mutexes.

## 🎯 Problem Description

- Only **one student** can get help from the TA at a time.
- Up to **five students** can wait in the office (on chairs).
- If no chair is available, the student leaves and tries again later.
- If the TA is sleeping and a student enters, the student wakes up the TA.
- After helping a student, the TA checks if other students are waiting.
  - If yes, the TA helps the next one.
  - If not, the TA goes back to sleep.

## ⚙️ Technologies Used

- **Language:** C
- **Libraries:**
  - `pthread.h`
  - `semaphore.h`
- **Tools:** Visual Studio Code, GCC, Linux Terminal
