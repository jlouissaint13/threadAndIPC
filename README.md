# threadAndIPC

This project includes two parts: Multithreaded Account Transactions (Project A) and Inter-Process Communication using Pipes (Project B).

Project A Multithreaded Account Transactions
The goal was to simulate a banking system with multithreading, ensuring thread safety during transactions. A User class tracked user data and transactions were processed by multiple threads, randomly sending and receiving money. To prevent race conditions, mutexes were used to synchronize access to user data and CSV files. Challenges included ensuring shared data across threads, fixing race conditions, and managing memory with smart pointers. The project demonstrated the importance of thread synchronization and locking mechanisms.


Project B: Inter-Process Communication (IPC) using Pipes

This project focused on using pipes for IPC between a parent and a child process in a Linux environment. The child process sent data to the parent using a pipe, with proper error handling for communication issues. The project reinforced concepts of forking, process IDs, and pipe-based data transfer between processes.
