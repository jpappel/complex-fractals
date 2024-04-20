CC := gcc-13
CPPFLAGS :=
CFLAGS := -Wall
LDFLAGS :=

all: 

##############
#  Programs  #
##############

SRCS := mandelbrot.c plotting.c
OBJS := $(SRC:.c=.o)
OBJS_DIR := build/objs

build/mandelbrot: $(addprefix $(OBJS_DIR)/, $(OBJS))
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJS_DIR)/%.o: src/%.c $(OBJS_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJS_DIR):
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
	rm -rf presentation/presentation.html analysis/analysis.html

.PHONY: all presentation analysis clean
