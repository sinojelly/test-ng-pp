
TESTCPPGEN = testcppgen

pkgdata_SCRIPTS=$(top_builddir)/scripts/testcppgen.py

bin_SCRIPTS = $(TESTCPPGEN)
CLEANFILES = $(TESTCPPGEN)

$(TESTCPPGEN) : 
	@echo '#!/bin/sh' > $@
	@echo '$(PYTHON) $(pkgdatadir)/testcppgen.py $$*' >> $@
