#include "kernel_syscalls.h"

#include "drivers/keyboard.h"
#include "kernel_filesystem.h"
#include "kernel_stdio.h"
#include "stdlib/syscalls.h"

uint32_t sys_write_to_screen(char* s) {
  fprintf(LOG, "sys_write_to_screen(%s)\n", s);
  fprintf(SCREEN, s);
  return 0;
}

uint32_t sys_count_files() {
  struct file_t* file = get_first_file();
  uint32_t count = 0;
  while (file) {
    count++;
    file = file->next_sibling;
  }

  return count;
}

uint32_t sys_list_files(struct file_t* buffer) {
  struct file_t* file = get_first_file();
  while (file) {
    *buffer = *file;
    buffer++;
    file = file->next_sibling;
  }

  return 0;
}

uint32_t sys_register_input_handler(InputHandler handler) {
  set_input_handler(handler);
  return 0;
}

uint32_t handle_syscall(struct cpu_state* cpu) {
  uint32_t syscall_num = cpu->eax;

  fprintf(LOG, "--------------------\nSYSCALL (%i)\n", syscall_num);

  switch (syscall_num) {
    case (1):
      return sys_write_to_screen((char*) cpu->ebx);
    case (2):
      return sys_count_files();
    case (3):
      return sys_list_files((struct file_t *) cpu->ebx);
    case (4):
      return sys_register_input_handler((InputHandler) cpu->ebx);
    default:
      fprintf(LOG, "Unknown syscall: %x\n", syscall_num);
      while(1){}
  }

  fprintf(LOG, "--------------------\n");
}