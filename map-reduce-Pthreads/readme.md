This code uses **Pthread mutexes and condition variables** to avoid race conditions and deadlocks

The make file consists of necessary statements to compile the source files (mapper, reducer, combiner)

- Use "make && ./multithreaded <buffer_size> <user_num> <file_num> to compile and execute the code

Example: make && ./multithreaded 10 5 input.txt
