# malloc_implementation
Memory management project, malloc heap implementation

todo:<br>
- finish mmalloc, free, print_heap, memadd<br>
- implement mcalloc(n, size)<br>
- implement error-checking<br>
- implement bfree(p, n)<br>
- implement garbage-collection<br>
- find a way to implement several algorithms and types of heap management and a way to choose/switch between them<br>
      couple of ideas: <br>
            mmalloc_bf //best fit or mmalloc_ff //first fit...<br>
            mmalloc(n, algorithm)<br>
            #define __MMALLOC_BEST_FIT__<br>
            #include "mmalloc_bf_ilt.h"  //mmalloc | best-fit algorithm | implicit-list-type structure<br>
