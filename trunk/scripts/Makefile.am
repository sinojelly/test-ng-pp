
TESTNGPPGEN = testngppgen

pkgdata_SCRIPTS=$(top_builddir)/scripts/testngppgen.py

bin_SCRIPTS = $(TESTNGPPGEN)
CLEANFILES = $(TESTNGPPGEN)

$(TESTNGPPGEN) : 
	@echo '#!/bin/sh' > $@
	@echo '$(PYTHON) $(pkgdatadir)/testngppgen.py $$*' >> $@
