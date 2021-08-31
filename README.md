# malloc_implementation
Memory management project, malloc heap implementation

todo:
- finish mmalloc, free, print_heap, memadd
- implement mcalloc(n, size)
- implement error-checking
- implement bfree(p, n)
- implement garbage-collection
- find a way to implement several algorithms and types of heap management and a way to choose/switch between them
      couple of ideas: 
            mmalloc_bf //best fit or mmalloc_ff //first fit...
            mmalloc(n, algorithm)
            #define __MMALLOC_BEST_FIT__
            #include "mmalloc_bf_ilt.h"  //mmalloc | best-fit algorithm | implicit-list-type structure
