OUT_NAME    := rfpos_test
#EXEC_EXT   := .exe
#OUT_DIR     := .
#INC_DIRS    := ../libs/include 
#INC_FLAGS   := -I/usr/include/foo -I `wx-config --cxxflags`
#CLEAN_FILES := "data.txt"

## 1-st way to select source files
#SRC_DIRS := . libs/la

# 2-nd way to select source files
SRCS := \
	libs/rnd/rnd.c \
	libs/la/la.c \
	rfpos.c \
	rfpos_test.c

#----------------------------------------------------------------------------
DEFS := -DLA_USE_EXIT -DLA_DOUBLE -DLA_DEBUG -DLA_MEMORY_DEBUG -DLA_PARANOIC
OPT  := -Os
CFLAGS   := $(DEFS) $(OPT) -Wall -pipe $(CFLAGS)
CXXFLAGS := $(CFLAGS) $(CXXFLAGS)
LDFLAGS  := -lm -lrt $(LDFLAGS)
#----------------------------------------------------------------------------
_CC  := @gcc
_CXX := @g++
_LD  := @gcc
#----------------------------------------------------------------------------
include libs/Makefile.skel

