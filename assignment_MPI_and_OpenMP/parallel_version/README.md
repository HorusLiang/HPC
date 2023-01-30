# Future directions
srun can be applied. ( in job.sh )\
#SBATCH --nodes=16 ( how to change this parameter value in a loop?)\
why [ $tasks -ge $nodes ] received no result?\
why OpenMP code has no effect?\
how to use OpenMp to optimize inner loops(if there is two loops?)\
map can change the type from int to uint8 to reduce the requirement of memory, assume can have a great impact.
# Short description
simple_try: with scatter and gather often used.\
second_try: final version but no comment or doc.\ ( with error handling)
test.c: with optimized algorithm, with comments and docs, but don't use scatter and gather, use send receive, which can optimize.
compilepi.sh: files used to compile c files.\
job.sh: files to submit the job.


test_10.100.5000 are all in one node (a mistake made in the content of .out), also for all .out files.

Final experiment data:(in 100th generations, with more nodes or one node)
```
Nodes:  8  Tasks:  8  THREADS:  4  CPU:  4
Test passed: count_live_cell(a) returned 138, # of cell alive in the 100th generation
Obtained in 1.310264 seconds
DONE

Nodes:  4  Tasks:  4  THREADS:  4  CPU:  4
Test passed: count_live_cell(a) returned 138, # of cell alive in the 100th generation
Obtained in 2.900473 seconds
DONE

Nodes:  2  Tasks:  2  THREADS:  4  CPU:  4
Test passed: count_live_cell(a) returned 138, # of cell alive in the 100th generation
Obtained in 17.700215 seconds
DONE

Nodes:  1  Tasks:  1  THREADS:  4  CPU:  4
Test passed: count_live_cell(a) returned 138, # of cell alive in the 100th generation
Obtained in 35.216037 seconds
DONE

Nodes:  4  Tasks:  4  THREADS:  4  CPU:  4
Test passed: count_live_cell(a) returned 138, # of cell alive in the 100th generation
Obtained in 2.903459 seconds
DONE
Nodes:  4  Tasks:  8  THREADS:  4  CPU:  4
Test passed: count_live_cell(a) returned 138, # of cell alive in the 100th generation
Obtained in 1.531716 seconds
DONE

Nodes:  4  Tasks:  16  THREADS:  4  CPU:  4
Test passed: count_live_cell(a) returned 138, # of cell alive in the 100th generation
Obtained in 0.890457 seconds
DONE

Nodes:  4  Tasks:  4  THREADS:  4  CPU:  4
Test passed: count_live_cell(a) returned 138, # of cell alive in the 100th generation
Obtained in 2.423483 seconds
DONE
```


# In assignment submitted

code.c is (second_try: final version but no comment or doc) but delete some ??? commments.\
.markdown: document based on the javadoc from code.c.

