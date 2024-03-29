#include "Task2.h"
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void print_prompt() { printf("db > "); }

// Struct defns given in Task2.h
// Function declarations are also given in Task2.h
// You need not use the same fxns, modify according to your implementation
// and edit the declarations in .h later
// You can add/delete things from .h file according to your code

// add all fxns from task-1 here
void close_input_buffer(InputBuffer *input_buffer)
{
    free(input_buffer->buffer);
    free(input_buffer);
}

void pager_flush(Pager *pager, uint32_t page_num, uint32_t size);
{
    off_t offset = lseek(pager->file_descriptor, page_num * PAGE_SIZE, SEEK_SET);
    int status = write(pager->file_descriptor,pager->pages[page_num],(size_t)size);
    if(status == -1)
    printf("Error while flushing pages\n");
    if(status == 0)
    printf("Reached the end of file\n");
    pager->pages[page_num] = NULL;
}

void db_close(Table* table)
{
    int i;
    for( i = 0; i < TABLE_MAX_PAGES ;i++)
    {
        if((table->pager)->pages[i] != NULL)
        {
            if((table->num_rows) - (i+1)*ROWS_PER_PAGE >= 0)
             pager_flush(table->pager,i,PAGE_SIZE);
            else
             pager_flush(table->pager,i,((table->num_rows)%ROWS_PER_PAGE)*ROW_SIZE);
        }
    }    
    close((table->pager)->file_descriptor);
    free(table->pager);
    free(table);
}

MetaCommandResult do_meta_command(InputBuffer *input_buffer, Table *table)
{
    if(!strcmp((input_buffer->buffer+1),"exit "))
    {
        close_input_buffer(input_buffer);
        db_close(table);
        return META_COMMAND_SUCCESS; 
    }
    else return META_COMMAND_UNRECOGNIZED_COMMAND;
}
// add pager fxns with the functionalities given in Pager_template
Pager *pager_open(const char* filename)
{
    int fd = open(filename, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR)
    if (fd == -1) {
    printf("Unable to open file\n");
    exit(EXIT_FAILURE);
   }
    Pager* temp = Pager* calloc(1,sizeof(Pager));
   temp->file_descriptor = fd;
   struct stat st;
   if(stat(filename,&st) == 0)
   temp->file_length = st.st_size;
   return temp; 
}

Table *db_open(const char *filename)
{
    Table* temp = (Table*)calloc(1,sizeof(Table));
    temp->pager = pager_open(filename);
    table -> num_rows = ((table->pager)->file_length)/ROW_SIZE;
    return temp; 
}

// add cursor fxns with the functionalities given in Cursor_template

int main(int argc, char *argv[]) {
}