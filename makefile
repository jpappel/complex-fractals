CC := gcc
CPPFLAGS := #-DEXTENDED_PRECISION
CFLAGS := -Wall -O3 -march=native
LDFLAGS := -lm

NVCC := nvcc
NVCFLAGS := -arch=sm_86 -O3 --compiler-options -march=native
NVLDFLAGS := $(LDFLAGS)

SRC_DIR := src
BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/objects

TARGET := fractal-render serial-fractals shared-fractals cuda-fractals
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))


.PHONY: all presentation analysis clean test

all: $(addprefix $(BUILD_DIR)/, $(TARGET))

##############
#  Programs  #
##############

$(BUILD_DIR)/fractal-render: $(OBJ_DIR)/grids.o $(OBJ_DIR)/fractal_render.o $(OBJ_DIR)/renderers.o
	$(CC) $(CFLAGS) $^ -o $@ -lgd

$(BUILD_DIR)/serial-fractals:  $(OBJ_DIR)/serial-fractals.o $(OBJ_DIR)/grids.o $(OBJ_DIR)/fractals.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/shared-fractals: $(OBJ_DIR)/shared-fractals.o $(OBJ_DIR)/grids.o $(OBJ_DIR)/fractals.o 
	$(CC) $(CFLAGS) -fopenmp $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/cuda-fractals: $(OBJ_DIR)/cuda-fractals.o $(OBJ_DIR)/grids.o $(OBJ_DIR)/fractals.o
	$(NVCC) $(NVCFLAGS) $^ -o $@ $(NVLDFLAGS)

$(OBJ_DIR)/shared-fractals.o: $(SRC_DIR)/shared-fractals.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -fopenmp -c -o $@ $<

$(OBJ_DIR)/cuda-fractals.o: $(SRC_DIR)/cuda-fractals.cu | $(OBJ_DIR)
	$(NVCC) $(CPPFLAGS) $(NVCFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c  | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR):
	mkdir -p $@

###########
#  Tests  #
###########

examples/julia.png: examples/julia.grid $(BUILD_DIR)/fractal-render
	$(BUILD_DIR)/fractal-render -i $< -r png -o $@

examples/julia.grid: $(BUILD_DIR)/shared-fractals
	$< -c 0.285+0.01i -i 255 -r 2 -x 8192 -y 8192 -o $@ -f julia

examples/mandelbrot_%.grid: $(BUILD_DIR)/%-fractals
	$< -x 100 -y 100 -o $@

tests/mandelbrot: examples/mandelbrot_serial.grid examples/mandelbrot_shared.grid examples/mandelbrot_cuda.grid
	cmp -l $< $(word 2, $^) > $@
	cmp -l $(word 2, $^) $(word 3, $^) >> $@
	cmp -l $(word 3, $^) $< >> $@

################
#  Animations  #
################

##################
#  Presentation  #
##################

presentation: presentation/presentation.html analysis/analysis.html

presentation/presentation.html: presentation/presentation.md
	pandoc -t revealjs -so $@ $<

##############
#  Analysis  #
##############

analysis: analysis/analysis.html

analysis/analysis.html: analysis/analysis.Rmd analysis/data.csv
	Rscript -e "setwd('analysis'); rmarkdown::render('analysis.Rmd')"

clean:
	rm -rf $(BUILD_DIR)/* presentation/presentation.html analysis/analysis.html
