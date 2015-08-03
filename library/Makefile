
#    This file is a part of the wiringBone library
#    Copyright (C) 2015 Abhraneel Bera

#    This library is free software; you can redistribute it and/or
#    modify it under the terms of the GNU Lesser General Public
#    License as published by the Free Software Foundation; either
#    version 3 of the License, or (at your option) any later version.

#    This library is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#    Lesser General Public License for more details.

#    You should have received a copy of the GNU Lesser General Public
#    License along with this library; if not, write to the Free Software
#    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
#    USA


OBJ_DIR = ../BUILD/

SRC_SEARCH_DIR = ../

INCLUDE_DIR = .

CPPFLAGS = -Wall

CPPFLAGS += -lpthread

CFLAGS = -Wall

CFLAGS += -lpthread

include ../Makefile.mk

INCLUDE := $(addprefix -I , $(INCLUDE_DIR) $(SRC_SEARCH_DIR))

LIB_SRC := $(wildcard *.cpp)

C_SRC += $(wildcard $(addsuffix *.c, $(SRC_SEARCH_DIR)))

CPP_SRC += $(wildcard $(addsuffix *.cpp, $(SRC_SEARCH_DIR)))

INO_SRC += $(wildcard $(addsuffix *.ino, $(SRC_SEARCH_DIR)))

PDE_SRC += $(wildcard $(addsuffix *.pde, $(SRC_SEARCH_DIR)))

MAIN_SRC = $(C_SRC) $(CPP_SRC) $(INO_SRC) $(PDE_SRC)

LIB_OBJ = $(LIB_SRC:.cpp=.o)

C_OBJ = $(C_SRC:.c=.o) 

CPP_OBJ = $(CPP_SRC:.cpp=.o) 

INO_OBJ = $(INO_SRC:.ino=.o) 

PDE_OBJ = $(PDE_SRC:.pde=.o)

MAIN_OBJ = $(addprefix $(OBJ_DIR), $(addsuffix .o, $(basename $(notdir $(LIB_SRC) $(MAIN_SRC)))))

all: start main

main: $(LIB_OBJ) $(C_OBJ) $(CPP_OBJ) $(INO_OBJ) $(PDE_OBJ)
	@echo Compiling Main executable
	@g++ $(MAIN_OBJ) $(INCLUDE) $(CPPFLAGS) -o $(OBJ_DIR)$@

start: pru
	@mkdir -p $(OBJ_DIR)


$(LIB_OBJ): %.o : %.cpp
	@echo Compiling $<
	@g++ -c $< $(INCLUDE) $(CPPFLAGS) -o $(OBJ_DIR)$@

$(C_OBJ): %.o : %.c
	@echo Compiling $(notdir $<)
	@g++ -c $< $(INCLUDE) $(CFLAGS) -o $(OBJ_DIR)$(notdir $@)

$(CPP_OBJ): %.o : %.cpp
	@echo Compiling $(notdir $<)
	@g++ -c $< $(INCLUDE) $(CPPFLAGS) -o $(OBJ_DIR)$(notdir $@)

$(INO_OBJ): %.o : %.ino
	@echo Compiling $(notdir $<)
	@g++ -x c++ -c $< $(INCLUDE) $(CPPFLAGS) -o $(OBJ_DIR)$(notdir $@)

$(PDE_OBJ): %.o : %.pde
	@echo Compiling $(notdir $<)
	@g++ -x c++ -c $< $(INCLUDE) $(CPPFLAGS) -o $(OBJ_DIR)$(notdir $@)


pru: PRU0.p PRU1.p
	@echo Compiling PRU Assembly
	@pasm -V3 -c -CPRU0code PRU0.p
	@pasm -V3 -c -CPRU1code PRU1.p
