OUT_NAME    := la_test
#EXEC_EXT   := .exe
#OUT_DIR     := .
#INC_DIRS    := ../libs/include 
#INC_FLAGS   := -I/usr/include/foo -I `wx-config --cxxflags`
CLEAN_FILES := "data.txt"

## 1-st way to select source files
#SRC_DIRS := . libs/src

# 2-nd way to select source files
SRCS := \
	../la.c \
	../la_filt.c \
	../la_cine.c \
	../../rnd/rnd.c \
	la_test.c

#----------------------------------------------------------------------------
DEFS := -DLA_USE_EXIT -DLA_FLOAT -DLA_DEBUG -DLA_MEMORY_DEBUG -DLA_PARANOIC
OPT  := -O3
CFLAGS   := $(DEFS) $(OPT) -Wall -pipe $(CFLAGS)
CXXFLAGS := $(CFLAGS) $(CXXFLAGS)
LDFLAGS  := -lm -lrt $(LDFLAGS)
#----------------------------------------------------------------------------
_CC  := @gcc
_CXX := @g++
_LD  := @gcc
#----------------------------------------------------------------------------
include ../../Makefile.skel

