
MODULES = $(wildcard u[0-9]*)

all:	$(TARGET)
	$(MAKE) -C shared
	for module in $(MODULES) ; do \
	 $(MAKE) -C $$module ; \
	done

clean:
	$(MAKE) -C shared clean
	for module in $(MODULES) ; do \
	 $(MAKE) -C $$module clean ; \
	done
