The Makefile consists of necessary statements to compile the three source files (mapper, reducer, combiner)

-Use "make" to generate executable files for all source codes at once\
-Use "make source-file-name" to compile and make executable only one at a time\
  Example: make mapper

-Run the combiner executable file as shown:\
		./combiner ./mapper ./reducer
