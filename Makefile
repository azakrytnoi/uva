
MODULES = $(wildcard u[0-9]*)

.PHONY: starter ${MODULES}

starter: $(MODULES)
	$(MAKE) -C shared

$(MODULES):
	$(MAKE) -C $@

all:	$(TARGET)
#	$(MAKE) -C shared
	$(MAKE) -j -l5
#	$(foreach m,$(MODULES),$(MAKE) -C $(m) && ) true

depend:	$(TARGET)
	$(MAKE) -C shared depend
	$(foreach m,$(MODULES),$(MAKE) -C $(m) depend && ) true

clean:
	$(MAKE) -C shared clean
	$(foreach m,$(MODULES),$(MAKE) -C $(m) clean && ) true

strip:
	strip -s -K invoke -x -X -v lib/*
	strip -s -x -X -v shared/starter
