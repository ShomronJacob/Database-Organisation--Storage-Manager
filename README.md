# Database-Organisation--Storage-Manager
This program creates a simple storage manager that can read and write blocks from a file on disk, create files, open files, and close files. The goal of this assignment is to implement a simple storage manager - a module that is capable of reading blocks from a file on disk into memory and writing blocks from memory to a file on disk. The storage manager deals with pages (blocks) of fixed size (PAGE_SIZE). In addition to reading and writing pages from a file, it provides methods for creating, opening, and closing files. The storage manager has to maintain several types of information for an open file: The number of total pages in the file, the current page position (for reading and writing), the file name, and a POSIX file descriptor or FILE pointer

## Instructions to run the script

1) To execute the create, open, close, and destroy test cases (i.e. execute the test_assign1_1.c file):

i) SSH into the Fourier environment.

ii) Navigate to the following directory in the Fourier environment: /home/class/fall-17/cs525/pflavel/assign1_group1

iii) Run the test script by executing the following command:
	./test_assign1_1 

2) To execute additional test cases:

i) SSH into the Fourier environment.

ii) Navigate to the following directory in the Fourier environment: /home/class/fall-17/cs525/pflavel/assign1_group1

iii) Place your test file in this directory.

iv) Edit the makefile such that 'test_assign1_1.c' is replaced in every instance with the name of your test script instead.

Read the Readme.txt for more information on how each function works within the code.
