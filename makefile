CC :=
CPPFLAGS :=
LDFLAGS :=

all:


################
#  Animations  #
################

##################
#  Presentation  #
##################

presentation: presentation/presentation.html

presentation/presentation.html: presentation/presentation.md
	pandoc -t revealjs -so $@ $<

clean:
	rm -rf presentation/presentation.html

.PHONY: all presentation clean
