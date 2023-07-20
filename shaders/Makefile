.PHONY: all

FRAGIN:=$(wildcard *.frag)
FRAG:=$(FRAGIN:.frag=Frag.spv)
VERTIN:=$(wildcard *.vert)
VERT:=$(VERTIN:.vert=Vert.spv)

all:$(FRAG) $(VERT)

$(VERT):%Vert.spv:%.vert
	glslc $< -o $@

$(FRAG):%Frag.spv:%.frag
	glslc $< -o $@
