#include "loader.h"

Elf32_Ehdr *ehdr; // ELF file header
Elf32_Phdr *phdr; // Program header table
int fd; // File descriptor object

/*
 * release memory and other cleanups
 */
void loader_cleanup() {
  
}

/*
 * Load and run the ELF executable file
 */
void load_and_run_elf(char** exe) {
  fd = open(argv[1], O_RDONLY); // argv[1] contains the path to the ELF file
// 1. Load entire binary content into the memory from the ELF file.
  off_t fd_size = lseek(fd, 0, SEEK_END); // We calculate the size of the file by offsetting to the end 
  lseek(fd, 0, SEEK_SET);

  char *heap_mem; 
  heap_mem = (char *)malloc(fd_size); // Heap memory with file size 

  
  if (!heap_mem) // Checking if memory is allocated 
  {
    perror("Error:  Heap Memory allocation failed"); 
    exit(1);
  }

  ssize_t f_read = read(fd, heap_mem, fd_size); // Reading the file descriptor
  
  // Checking if file read was successful
  if (file_read < 0 )
  {
    perror("Error: File read operation failed");
    free(heap_mem);
    exit(1);
  }
  else if((size_t)file_read != fd_size)
  {  perror("Error: File read operation failed");
    free(heap_mem);
    exit(1);}
   
  ehdr = (Elf32_Ehdr *)heap_mem; // ELF header

  
  if (ehdr->e_type != ET_EXEC) // Checking if file is executable format or not
  {
    printf("Error: Unsupported ELF file");
    exit(1);
  }

  
  phdr = (Elf32_Phdr *)(heap_mem + ehdr->e_phoff); // Program header , using program offset to get program header

  
  unsigned int entry_p = ehdr->e_entry; // Getting the entry point from ELF header 

  Elf32_Phdr *tmp = phdr; // Creating a temporary program header to iterate 
  int tot_phdr = ehdr->e_phnum;
  void *vir_mem;
  void *entry_addrs;
  int i = 0;
  // 2. Iterate through the PHDR table and find the section of PT_LOAD 
  //    type that contains the address of the entrypoint method in fib.c
  // 3. Allocate memory of the size "p_memsz" using mmap function 
  //    and then copy the segment content
  // 4. Navigate to the entrypoint address into the segment loaded in the memory in above step
  // 5. Typecast the address to that of function pointer matching "_start" method in fib.c.
  // 6. Call the "_start" method and print the value returned from the "_start"
  int result = _start();
  printf("User _start return value = %d\n",result);
}
