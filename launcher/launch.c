#include "loader.h"

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
  // 2. passing it to the loader for carrying out the loading/execution
  load_and_run_elf(argv[1]);
  // 3. invoke the cleanup routine inside the loader  
  loader_cleanup();
  return 0;
}
 