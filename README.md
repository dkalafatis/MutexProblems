#  Set of solved process and thread synchronization problems using mutually exclusive variables (mutexes) in the Unix/Linux operating system:

13: Demonstrates a process that reads characters from a shared buffer while a separate thread writes to it using a mutex for synchronization. The writer_thread function writes characters to the shared buffer with a delay, simulating a writing process. The main function reads from the buffer by calling read_buffer() in a loop until the writer thread signals it's done.

14: Demonstrates a simple reader-writer implementation where a reader and a writer communicate using a shared buffer. Access to the shared buffer is controlled using mutexes and condition variables to ensure proper synchronization and prevent race conditions.

15: Creates two threads that read two shared variables (var1 and var2). Each thread locks a mutex to safely read its respective variable. The main process waits until both threads have read their variables, using a mutex and condition variable for synchronization. Once both threads have read the variables, the main process locks both mutexes and swaps the contents of var1 and var2, ensuring thread-safe access.

16: Implements the producer-consumer problem with a warehouse of size 1, using mutexes and condition variables for synchronization. The producer thread generates one data item per unit time, and the consumer thread processes one data item per unit time. The add and remove_item functions manage the warehouse and ensure that threads wait appropriately when the warehouse is full or empty.

### Dependencies

* C and Unix/Linux Operating System.
