
MODULES = $(wildcard u[0-9]*)

all:	$(TARGET)
	$(MAKE) -C shared
	for module in $(MODULES) ; do \
	 $(MAKE) -C $$module ; \
	done

depend:	$(TARGET)
	$(MAKE) -C shared depend
	for module in $(MODULES) ; do \
	 $(MAKE) -C $$module depend ; \
	done

clean:
	$(MAKE) -C shared clean
	for module in $(MODULES) ; do \
	 $(MAKE) -C $$module clean ; \
	done

strip:
	strip -s -K invoke -x -X -v lib/*
	strip -s -x -X -v shared/starter
