# Enforce Intel syntax
.intel_syntax noprefix

# Call the "write" system function (man 2 write)
# ssize_t write(int fd, const void *buf, size_t count);
mov rax, 1
# Put the function arguments in the rdi, rsi, rdx, r10, r8, r9 registers
mov rdi, 1
lea rsi, [rip + 0xa]
mov rdx, 17
# Call the function
syscall
ret
.string "Hello, Your Name\n"