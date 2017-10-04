//
// Created by Anna, Fran, Shom, and Pat
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "storage_mgr.h"
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>

/********************/
/* GLOBAL VARIABLES */
/********************/

int starting_value;

/***************************/
/* manipulating page files */
/***************************/

void initStorageManager (void)
{
    starting_value=1; // initilizing storage manager
}

//Create a new page file fileName. The initial file size should be one page.
//This method should fill this single page with '\0' bytes.
RC createPageFile (char *fileName) // This function creates a new page file
{
    if (starting_value!=1) {       //If the starting_value, defined at Storage Manager initialization, is not equal to 1,
        return RC_STORAGE_MANAGER_NOT_INITIALIZED;  //then return Storage Manager Not Initialized error
    }
    FILE *newfile;
    if (fileName == NULL || (strcmp(fileName,"") == 0)) {   //If the fileName is empty,
        return RC_NO_FILENAME;                              //return No File Name error
    }
    newfile=fopen(fileName,"w+b");                          //If the fopen method returns a NULL value,
    if (newfile == NULL) {                                  //then return File Not Initialized error
        return RC_FILE_NOT_INITIALIZED;
    }
    char psize[PAGE_SIZE] = {'\0'}; //initialiazing psize to an empty page
    if (fwrite(psize, sizeof(char), PAGE_SIZE, newfile) < PAGE_SIZE) {  //writing psize into the newly created file
        return RC_FILE_CREATION_FAILED;                                 //If the fwrite method returns a value lower
    }                                                                   //than the PAGE_SIZE, then the File Creation
    fclose(newfile); //closing file
    return RC_OK;
}

//Opens an existing page file. Should return RC_FILE_NOT_FOUND if the file does
//not exist. The second parameter is an existing file handle. If opening the
//file is successful, then the fields of this file handle should be initialized
//with the information about the opened file. For instance, you would have to
//read the total number of pages that are stored in the file from disk.
RC openPageFile (char *fileName, SM_FileHandle *fHandle)
{
    FILE *filepointer;
    if (fileName == NULL || (strcmp(fileName,"") == 0)) { //If the fileName is empty,
        return RC_NO_FILENAME;                            //return No File Name error
    }
    filepointer=fopen(fileName,"r+b"); //opening a file
    if(filepointer != NULL)
    {
        fseek(filepointer,0,SEEK_END); //sets the file position of the stream to the end of the file.
        long int filesize=ftell(filepointer); //returns the current file position of the given stream.
        fHandle->fileName=fileName;
        fHandle->totalNumPages=filesize/PAGE_SIZE; // to calculate the total number of pages
        fHandle->curPagePos=0; //current page position is 0
        fHandle->mgmtInfo=(void*)filepointer;
        return RC_OK;
    }
    else
    {
        return  RC_FILE_NOT_FOUND; // return if page is not found
    }
}


RC closePageFile (SM_FileHandle *fHandle) // to close an open page file
{
    fHandle->curPagePos=-1; // setting current position of page
    fHandle->totalNumPages=-1; // setting number of pages
    return RC_OK; //return ok
}


RC destroyPageFile (char *fileName) // delete a page file
{
    //fclose(fileName); //close file pointer
    if (fileName == NULL || (strcmp(fileName,"") == 0)) { //If the fileName is empty,
        return RC_NO_FILENAME; //return No File Name error
    }
    if (remove(fileName) == 0) {// delete the page file
        return RC_OK; //return ok
    } else {
        return RC_NO_FILENAME;
    }
}


/****************************/
/* reading blocks from disc */
/****************************/

//check if valid pageNum for a read, return 0 if valid, -1 otherwise
int validate_pageNum_read(int pageNum, SM_FileHandle *fHandle) {
    //pageNum is invalid (<0 or >=totalNumPages)
    if (pageNum >= fHandle->totalNumPages || pageNum < 0){
        return -1;
    }
    else { // pageNum is valid, i.e. ~ [0,totalNumPages - 1]
        return 0;
    }
}
//check if valid pageNum for a write, return 0 if valid, -1 otherwise
int validate_pageNum_write(int pageNum, SM_FileHandle *fHandle) {
    //pageNum is invalid (<0 or >totalNumPages)
    if (pageNum > fHandle->totalNumPages || pageNum < 0){
        return -1;
    }
    else { // pageNum is valid, i.e. ~ [0,totalNumPages]
        return 0;
    }
}
//set the position of the open page to pageNum and return 0, or return -1 if unsuccessful
int find_page(int pageNum, FILE *fHandle){
    //set position of fHandle to (0+pageNum*PAGE_SIZE)
    if (fseek(fHandle, pageNum * PAGE_SIZE * sizeof(char), SEEK_SET) == 0){ //SEEK_SET = 0
        return 0;
    }
    else {
        return -1;
    }
}
//reads the page given by temp file fHandle_file into memPage
int read_page(SM_PageHandle memPage, FILE *fHandle_file, SM_FileHandle *fHandle){
    //read PAGE_SIZE elements from fHandle_file into memPage
    if (fread(memPage, sizeof(char), PAGE_SIZE, fHandle_file) >= PAGE_SIZE*sizeof(char)){
        //if successfully read into memPage: return 0
        return 0;
    }
    else { //return -1 if read was unsuccessful
        return -1;
    }
}

//The method reads the pageNum-th block from a file and stores its content in
//the memory pointed to by the memPage page handle. If the file has less than
//pageNum pages, the method returns RC_READ_NON_EXISTING_PAGE.
RC readBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage){
    //declare a new file handle to store/access temporary information
    FILE *fHandle_file;
    fHandle_file = (FILE *)fHandle->mgmtInfo;
    //check if pageNum is valid, return error if invalid
    if (validate_pageNum_read(pageNum, fHandle) == -1){
        return RC_READ_NON_EXISTING_PAGE;
    }
    //set the position of the temp file to pageNum
    if (find_page(pageNum, fHandle_file) == 0) {
        //read the page from temp file into the memPage and update page number
        if (read_page(memPage, fHandle_file, fHandle) == 0) {
            return RC_OK; //return ok RC if read was successful
        }
        else {
            return RC_FILE_READ_FAILED; //return error if file read invalid
        }
    } //return error if pageNum invalid
    else {
        return RC_CURRENT_PAGE_NOT_FOUND;
    }
}

//Return the current page position in a file
int getBlockPos (SM_FileHandle *fHandle){
    return fHandle->curPagePos;
}

//Read the first page in a file
RC readFirstBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
    int pageNum_first = 0;
    return readBlock(pageNum_first, fHandle, memPage);
}

//Read the previous page relative to the curPagePos of the file.
//The curPagePos is moved to the page that was read, i.e. the previous page number.
//If the user tries to read a block before the first page of after the last
//page of the file, the method returns RC_READ_NON_EXISTING_PAGE.
RC readPreviousBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
    int pageNum_prev = getBlockPos(fHandle) - 1;
    RC rc = readBlock(pageNum_prev, fHandle, memPage);
    if (rc == RC_OK) {
        fHandle->curPagePos = pageNum_prev;
    }
    return rc;
}

//Read the current page relative to the curPagePos of the file.
//If the user tries to read a block before the first page of after the last
//page of the file, the method returns RC_READ_NON_EXISTING_PAGE.
RC readCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
    int pageNum_cur = getBlockPos(fHandle);
    return readBlock(pageNum_cur, fHandle, memPage);
}

//Read the next page relative to the curPagePos of the file.
//The curPagePos is moved to the page that was read, i.e. the next page number.
//If the user tries to read a block before the first page of after the last
//page of the file, the method returns RC_READ_NON_EXISTING_PAGE.
RC readNextBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
    int pageNum_next = getBlockPos(fHandle) + 1;
    RC rc = readBlock(pageNum_next, fHandle, memPage);
    if (rc == RC_OK) {
        fHandle->curPagePos = pageNum_next;
    }
    return rc;
}

//Read the last page in a file
RC readLastBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
    int pageNum_last = fHandle->totalNumPages - 1;
    return readBlock(pageNum_last, fHandle, memPage);
}

/*********************************/
/* writing blocks to a page file */
/*********************************/
//Write a page to disk using an absolute position.
RC write_Page(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage){
    if (validate_pageNum_write(pageNum, fHandle) == -1){
        return RC_WRITE_FAILED;
    }

	//Set the pointer to the new position.
	int setPointPos = fseek(fHandle->mgmtInfo, pageNum*PAGE_SIZE*sizeof(char), SEEK_SET);

	//If the new position is found, write the page to the file.
	if(setPointPos == 0 ){
		//Store the number of bytes written.
		int writeCount = fwrite(memPage, sizeof(char), PAGE_SIZE, fHandle->mgmtInfo);

		//If the number of bytes written does not match the number of bytes on a page,
		//then writing to the file failed.
		if(writeCount != PAGE_SIZE){
            return RC_WRITE_TO_OUTPUTSTREAM_FAILED;
        }

		return RC_OK;
	}

	//If the new position is not found, return error.
	else{
		return RC_FILE_OFFSET_FAILED;
	}
}

//Use write_Page() to write a page to disk using an absolute position and then increase the curPagePos.
RC writeBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage){

	RC returnCode = write_Page(pageNum, fHandle, memPage);

	if(returnCode == RC_OK)
		//Make the page just written the current page.
		fHandle->curPagePos = pageNum;

	return returnCode;
}

//Use writeBlock() to write a page to disk using the current position.
RC writeCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
	//Use writeBlock() to write to the current position,
	//using the curPagePos of the file in the method call.
	RC returnCode = writeBlock(fHandle->curPagePos, fHandle, memPage);

	return returnCode;
}

//Increase the number of pages in the file by one.
//The new last page should be filled with zero bytes.
RC appendEmptyBlock (SM_FileHandle *fHandle){
	//Allocate memory of the size of a page.
	SM_PageHandle lastPage = malloc(PAGE_SIZE);

	//If the pointer is empty, the memory allocation failed.
	if(!lastPage) {
        return RC_INCOMPATIBLE_BLOCKSIZE;
    }
	//Fill the memory just allocated with zero bytes and
	memset(lastPage, 0, PAGE_SIZE);

	//Use write_Page() to write that page to the end of file.
	RC returnCode = write_Page(fHandle->totalNumPages, fHandle, lastPage);

    //If block written successfully, increment the total number of pages in the the file
    if (returnCode == RC_OK) {
        fHandle->totalNumPages += 1;
    }

	//Free the allocated memory
	free(lastPage);

	return returnCode;
}
//If the file has less than numberOfPages pages then increase the size to
//numberOfPages.
RC ensureCapacity (int numberOfPages, SM_FileHandle *fHandle){
	//If the file already has more or as many pages as being requested,
	//then there is no need to add pages.
	if(fHandle->totalNumPages >= numberOfPages) {
		return RC_OK;
    }
	//Find the difference between the number of pages being requested and the current total,
	//and use appendEmptyBlock() to add that number.
    int i = 0;
	while (fHandle->totalNumPages < numberOfPages) {
		appendEmptyBlock(fHandle);
        if(fHandle->totalNumPages >= numberOfPages) {
            return RC_OK;
        }
        i++;
	}
    return RC_OK;
}