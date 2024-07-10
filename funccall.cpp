#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <cstring>
#include <unistd.h>
#include <sys/mman.h>

struct MemoryPages {
    uint8_t *mem;       // Pointer to the start of the executable memory
    size_t page_size;   // OS defined memory page size (typically 4096 bytes)
    size_t pages = 0;   // Number of pages requested from the OS
    size_t position = 0;    // Current position to the non-used memory space

    MemoryPages(size_t pages_requested = 1) {
        page_size = sysconf(_SC_PAGE_SIZE);
        mem = (uint8_t*) mmap(NULL, page_size * pages_requested, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS ,-1, 0);
        if (mem == MAP_FAILED) {
            throw std::runtime_error("Can't allocate enough executable memory!");
        }
        pages = pages_requested;
    }

    ~MemoryPages() {
        munmap(mem, pages * page_size);
    }

    // Push a uint8_t number to the memory
    void push(uint8_t data) {
        check_available_space(sizeof data);
        mem[position] = data;
        position++;
    }

    // Push a function pointer to the memory
    void push(void (*fn)()) {
        size_t fn_address = reinterpret_cast<size_t>(fn);
        check_available_space(sizeof fn_address);

        std::memcpy((mem + position), &fn_address, sizeof fn_address);
        position += sizeof fn_address;
    }

    // Push a vector of uint8_t numbers to the memory
    void push(const std::vector<uint8_t> &data) {
        check_available_space(data.size());

        std::memcpy((mem + position), &data[0], data.size());
        position += data.size();
    }

    void check_available_space(size_t data_size) {
        if (position + data_size > pages * page_size) {
            throw std::runtime_error("Not enough virtual memory allocated!");
        }
    }
};

