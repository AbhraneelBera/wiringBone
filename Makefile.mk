#****************************************************************
#Usage :

#Source file in this directory will be automatocally compiled.

#To include any other directory use SRC_SEARCH_DIR += followed by 
#directory name (ending with / ).
#For example SRC_SEARCH_DIR += ~/Desktop/example/

#To include selective source files from other directories :
# For .c   files use C_SRC +=   followed by full path
# For .cpp files use CPP_SRC += followed by full path
# For .ino files use INO_SRC += followed by full path
# For .pde files use PDE_SRC += followed by full path

#Optional compile-time flags can be added using CFLAGS += or 
#CPPFLAGS += followed by flags.
#****************************************************************