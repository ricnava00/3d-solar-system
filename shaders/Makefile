.PHONY: all

FRAGIN:=$(wildcard *.frag)
FRAG:=$(FRAGIN:.frag=.spv)
VERTIN:=$(wildcard *.vert)
VERT:=$(VERTIN:.vert=.spv)

all:$(FRAG) $(VERT)

$(VERT):%.spv:%.vert
	glslc $< -o $@

$(FRAG):%.spv:%.frag
	glslc $< -o $@
