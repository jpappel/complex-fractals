CC := gcc
CPPFLAGS :=
CFLAGS := -Wall
LDFLAGS := -lm

SRC_DIR := src
BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/objects

TARGET := serial-fractals shared-fractals
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))


.PHONY: all presentation analysis clean test

all: $(addprefix $(BUILD_DIR)/, $(TARGET))

##############
#  Programs  #
##############

$(BUILD_DIR)/serial-fractals: $(OBJ_DIR)/fractals.o $(OBJ_DIR)/serial-fractals.o $(OBJ_DIR)/grids.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/shared-fractals: $(OBJ_DIR)/fractals.o $(OBJ_DIR)/shared-fractals.o $(OBJ_DIR)/grids.o
	$(CC) $(CFLAGS) -fopenmp $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/shared-fractals.o: $(SRC_DIR)/shared-fractals.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -fopenmp -c -o $@ $<

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
	rm -rf $(BUILD_DIR)/* presentation/presentation.html analysis/analysis.html
