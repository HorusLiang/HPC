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

# In assignment submitted

code.c is (second_try: final version but no comment or doc) but delete some ??? commments.\
.markdown: document based on the javadoc from code.c.

