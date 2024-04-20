CC := gcc-13
CPPFLAGS :=
CFLAGS := -Wall
LDFLAGS :=

SRC_DIR := src
BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/objects

TARGET := serial-fractal
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))


.PHONY: all presentation analysis clean test

all: $(BUILD_DIR)/$(TARGET)

##############
#  Programs  #
##############

build/mandelbrot: $(OBJ_DIR)/serial-fractal.o $(OBJ_DIR)/grids.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c  | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR):
	mkdir -p $@

################
#  Animations  #
################

##################
#  Presentation  #
##################

presentation: presentation/presentation.html

presentation/presentation.html: presentation/presentation.md
	pandoc -t revealjs -so $@ $<

##############
#  Analysis  #
##############

analysis: analysis/analysis.html

analysis/analysis.html: analysis/analysis.Rmd # TODO: add compile command

clean:
	rm -rf presentation/presentation.html analysis/analysis.html $(OBJS)
