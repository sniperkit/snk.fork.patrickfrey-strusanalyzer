include variables.mk

all:
	cd 3rdParty; make all; cd -
	cd src; make all; cd ..

test: all

clean:
	cd 3rdParty; make clean; cd -
	cd src; make clean; cd ..

install:
	cd 3rdParty; make install; cd -
	cd src; make install; cd ..
	@mkdir -p $(INSTALL_INCDIR)/analyzer
	@mkdir -p $(INSTALL_INCDIR)/tokenizer
	@cp include/strus/analyzer/*.hpp $(INSTALL_INCDIR)/analyzer
	@cp include/strus/tokenizer/*.hpp $(INSTALL_INCDIR)/tokenizer
	@cp include/strus/*.hpp $(INSTALL_INCDIR)

uninstall:
	cd 3rdParty; make uninstall; cd -
	cd src; make uninstall; cd ..
	@-rm $(INSTALL_INCDIR)/analyzer/term.hpp
	@-rm $(INSTALL_INCDIR)/analyzer/metaData.hpp
	@-rm $(INSTALL_INCDIR)/analyzer/attribute.hpp
	@-rm $(INSTALL_INCDIR)/analyzer/document.hpp
	@-rm $(INSTALL_INCDIR)/analyzerInterface.hpp
	@-rm $(INSTALL_INCDIR)/analyzerLib.hpp
	@-rm $(INSTALL_INCDIR)/normalizerInterface.hpp
	@-rm $(INSTALL_INCDIR)/strusAnalyzer.hpp
	@-rm $(INSTALL_INCDIR)/tokenizer/position.hpp
	@-rm $(INSTALL_INCDIR)/tokenizerInterface.hpp
	@-rm $(INSTALL_INCDIR)/tokenMinerFactory.hpp
	@-rm $(INSTALL_INCDIR)/tokenMiner.hpp
	@-rm $(INSTALL_INCDIR)/tokenMinerLib.hpp
	@rmdir $(INSTALL_INCDIR)
	@rmdir $(INSTALL_LIBDIR)
	@rmdir $(INSTALL_BINDIR)
