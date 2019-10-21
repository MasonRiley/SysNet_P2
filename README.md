# SysNet_P2
## Mason Riley, and Cesar Santiago.

- Issues
-   Race conditions were a challenge, to counter this a global counter was set up which tracked the counter for all threads
-A lock was used during the changing of this critical section which is accessed by all threads.
