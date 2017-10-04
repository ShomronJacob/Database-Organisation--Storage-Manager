# Programming Assignment 1 -- Storage Manager

This program creates a simple storage manager that can read and write blocks from a file on disk, create files, open files, and close files.

## Contributors
Anna Cole Crosbie (A20367740)

Francesco Fanizza (A20079172)

Patrick Flavel (A20355341) - Official Group Submitter

Shomron Jacob (A20370729)

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

## Method Descriptions
Note: Each function has comments written throughout the source code so please refer the inline comments in the code to see the technical details for each of these methods/algorithms.

initStorageManager()
- This function deals with initializing the Storage Manager i.e, allows the pages to be created.

createPageFile()
- Creates a new page file with initial page size being 1. The data used later is written to this.

initStorageManager()
- Initializes the storage manager program.

createPageFile()
- This function deals with creating a new file.

openPageFile()
- This function deals with opening an existing page file with a given name. It also checks for if the given file exists or not.
If the file does not exists, it returns the error message. The file pointer is used to store information which is then used
while closing the page file.

closePageFile()
- This function closes the page file pointed by the file pointer. If it fails, then the error message is displayed.

destroyPageFile()
- This deals with removing a page file. If the file is absent, it gives the error message.

validate_pageNum_write()
- Validates whether the input page number is more than or equal to zero and less than or equal to the total number of pages in the file, i.e. whether it is a valid page number to write to: pageNum ~ [0,totalNumPages]. The reason pageNum can be equal to the total number of pages here is because a page can be written to the end of a file, which is done by appending a page at the index equal to the total number of pages.

validate_pageNum_read()
- Validates whether the input page number is more than or equal to zero and less than the total number of pages in the file, i.e. whether it is a valid page number to read: pageNum ~ [0,totalNumPages - 1].

find_page()
- Sets the position of the file to be read as the input page number. This is achieved by shifting the position to be read by X bytes from the start of the file, where X = pageNum * PAGE_SIZE * sizeof(char).

read_page()
- Reads the page that the input file is pointing to and stores its content in the memory pointed to by the input page handle, i.e. memPage.

readBlock()
- Validates the input page number, finds the page position for the input page number, and then reads this page and stores its content in the memory pointed to by the input page handle, i.e. memPage. If validating, finding, or reading the page fails then this method returns RC_READ_NON_EXISTING_PAGE.

getBlockPos()
- Return the current page position in a file

readFirstBlock()
- Read the first page in a file

readPreviousBlock()
- Read the previous page relative to the curPagePos of the file. The curPagePos should be moved to the page that was read. If the user tries to read a block before the first page of after the last page of the file, the method should return RC_READ_NON_EXISTING_PAGE.

readCurrentBlock()
- Read the current page relative to the curPagePos of the file. The curPagePos should be moved to the page that was read. If the user tries to read a block before the first page of after the last page of the file, the method should return RC_READ_NON_EXISTING_PAGE.

readNextBlock()
- Read the next page relative to the curPagePos of the file. The curPagePos should be moved to the page that was read. If the user tries to read a block before the first page of after the last page of the file, the method should return RC_READ_NON_EXISTING_PAGE.

readLastBlock()
- Read the last page in a file

write_Page()
- Write a page to disk using an absolute position.

writeBlock()
- Use write_Page() to write a page to disk using an absolute position and then increase the curPagePos.

writeCurrentBlock()
- Use writeBlock() to write a page to disk using the current position.

appendEmptyBlock()
- Uses write_Page() to increase the number of pages in the file by one. The new last page is filled with zero bytes.

ensureCapacity()
- If the file has less than numberOfPages pages, increases the size to numberOfPages.
