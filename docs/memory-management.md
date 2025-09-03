# Memory Management

This document describes the custom memory management system used in this project.

## Overview

The project uses a custom memory management system to simplify cleanup and prevent memory leaks. It works by tracking all dynamic memory allocations in a global linked list. A function is registered with `atexit()` to traverse this list and free all tracked memory blocks when the program exits. This ensures that all allocated memory is automatically released, even if `free()` is not explicitly called for every `malloc()`.

This approach is particularly useful for a game, where there might be many small allocations for things like game objects, bullets, or particles, and manually tracking each one can be error-prone.

## How it Works

The core of the system is the `HeapList` struct, which holds a linked list and function pointers for memory allocation and deallocation. A global instance of this struct, `heap_list`, is used to track all memory.

When `heap_list.malloc()` is called:
1.  It calls the standard `malloc()` to allocate memory.
2.  If the allocation is successful, it stores a pointer to the newly allocated memory block in the linked list.
3.  It returns the pointer to the caller, just like the standard `malloc()`.

When `heap_list.free()` is called:
1.  It searches for the corresponding pointer in the linked list.
2.  If found, it removes the entry from the list and calls the standard `free()` on the memory block.
3.  If not found, it logs an error, which can help detect double-free or freeing of non-tracked memory.

At program exit, the `free_all()` function is automatically called (thanks to `atexit()`). This function clears the entire linked list, calling `free()` on every tracked memory block that hasn't been freed yet.

## API Reference

### `HeapList`

```c
typedef struct
{
    List list;
    void *(*malloc)(size_t);
    void (*free)(void *);
} HeapList;
```

A struct that encapsulates the memory tracking list and the custom `malloc` and `free` functions.

### `heap_list`

```c
extern HeapList heap_list;
```

A global instance of `HeapList` that is used by default for all tracked allocations.

### `heap_list_create()`

```c
HeapList heap_list_create();
```

Initializes and returns a new `HeapList`. This is used to initialize the global `heap_list`.

### `free_all()`

```c
void free_all(void);
```

Frees all memory blocks tracked by the global `heap_list`. This function is registered with `atexit()` and should not need to be called manually.

## Usage Example

To use the tracked memory allocation, you should initialize the system at the beginning of your program's execution, typically in `main()`.

```c
#include "utils.h"
#include <stdlib.h>

int main(void) {
    // Initialize the memory management system
    heap_list = heap_list_create();
    atexit(free_all);

    // Use heap_list.malloc instead of malloc
    int *my_array = heap_list.malloc(sizeof(int) * 10);
    if (my_array) {
        my_array[0] = 123;
    }

    // You can still free memory manually if you want
    // heap_list.free(my_array);

    // If not freed manually, my_array will be freed automatically at exit.

    return 0;
}
```

When using this system, all calls to `malloc` that should be tracked must be replaced with `heap_list.malloc`. Similarly, `free` should be replaced with `heap_list.free`.

## Key Files

-   `include/utils.h`: Contains the declarations for `HeapList`, `heap_list`, `heap_list_create`, and `free_all`.
-   `src/utils.c`: Contains the implementation of the memory management system.
