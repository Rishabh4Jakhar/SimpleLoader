#include "loader.h"

Elf32_Ehdr *ehdr; // ELF file header
Elf32_Phdr *phdr; // Program header table
int fd; // File descriptor object

/*
 * release memory and other cleanups
 */
void loader_cleanup() {
  ehdr = NULL;
  free(ehdr);
  phdr = NULL;
  free(phdr);
  // Freeing all the memory allocated
}

/*
 * Load and run the ELF executable file
 */
void load_and_run_elf(char** exe) {
  fd = open(*exe, O_RDONLY); // argv[1] contains the path to the ELF file
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
  if (f_read < 0 )
  {
    perror("Error: File read operation failed");
    free(heap_mem);
    exit(1);
  }
  else if((size_t)f_read != fd_size)
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
  
  while (i < tot_phdr) {
    // Checking if the type is PT_LOAD
    if (tmp->p_type == PT_LOAD) {
      vir_mem = mmap(0, tmp->p_memsz, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
      // 3. Allocate memory of the size "p_memsz" using mmap function 
      //    and then copy the segment content
      memcpy(vir_mem, heap_mem + tmp->p_offset, tmp->p_memsz); 
      // Copying the segment content into vir_mem

      if (vir_mem == MAP_FAILED) // Checking if memory mapping was successful
      { perror("Error: Memory mapping failed");
        exit(1); }

      // 4. Navigate to the entrypoint address into the segment loaded in the memory in above step

      entry_addrs = vir_mem + (entry_p - tmp->p_vaddr); // Calculating the entry address

      // Breaking if entry address is not inside vir_mem
      if (entry_addrs <= (vir_mem + tmp->p_memsz) && entry_addrs >= vir_mem) // Checking if entry address is within the segment
      { 
        break;
       }
    }
  i++;
  tmp++;
}

if (entry_addrs == NULL) // Checking if entry address is NULL
{ printf("Error: Entry address is NULL");
  free(heap_mem);
  exit(1);
}
else {
  // 5. Typecast the address to that of function pointer matching "_start" method in fib.c.  
  int (*_start)() = (int (*)())entry_addrs; 

  // 6. Call the "_start" method and print the value returned from the "_start"
  int result = _start(); // Calling _start
  printf("User _start return value = %d\n",result); 
}
close(fd); // Closing file descriptor
}

int main(int argc, char** argv) 
{
  // Wrong Usage of Command
  if(argc != 2) {
    printf("Usage: %s <ELF Executable> \n",argv[0]);
    exit(1);
  }
  // 1. carry out necessary checks on the input ELF file
  
  FILE *elf = fopen(argv[1], "rb");
  // Opening ELF file from argv[1] (path to ELF file)
  if (!elf) // If file did not open
  {
    printf("Error: ELF File didn't open\n");
    exit(1);
  }
  // Closing the file
  fclose(elf);
  // 2. passing it to the loader for carrying out the loading/execution
  load_and_run_elf(argv[1]);
  // 3. invoke the cleanup routine inside the loader  
  loader_cleanup();
  return 0;
}
 