CC :=
CPPFLAGS :=
LDFLAGS :=

all:

##############
#  Programs  #
##############

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
