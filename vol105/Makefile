
MODULES = $(wildcard u[0-9]*)

.PHONY: ${MODULES}

$(MODULES):
	@echo "build single: $@"
	$(MAKE) -C $@

all:	$(MODULES)
	@echo "build all: $(MODULES)"
	$(MAKE) -j -l5

depend:	$(TARGET)
	$(foreach m,$(MODULES),$(MAKE) -C $(m) depend && ) true

clean:
	$(foreach m,$(MODULES),$(MAKE) -C $(m) clean && ) true
