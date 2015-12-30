######################################################
#           Project : HomeLinux                      #
#           Version : 1.0.0                          #
#           Date    : 12/2015                        #
#           Licence : BSD                            #
#           Authors : Sebastien Valat                #
######################################################

all:

check:
	@if test -z "$(PREFIX)"; then echo "ERROR : Please provide PREFIX as parameter to setup install directory"; exit 1; fi

install: check
	mkdir -p $(PREFIX)
	cp prefix.json $(PREFIX)/homelinux.json
	mkdir -p $(PREFIX)/bin
	cp bin/homelinux.sh $(PREFIX)/bin/homelinux
	cp bin/homelinux.sh $(PREFIX)/bin/hl
	cp bin/hl-env.sh $(PREFIX)/bin/hl-env
	cp bin/hl-build-cache.sh $(PREFIX)/bin/hl-build-cache
	mkdir -p $(PREFIX)/lib/homelinux
	cp -r index.js libs $(PREFIX)/lib/homelinux
	mkdir -p $(PREFIX)/homelinux
	cp -r packages $(PREFIX)/homelinux
	cp -r node_modules $(PREFIX)/lib/homelinux
	mkdir -p $(PREFIX)/homelinux/install-db
	$(PREFIX)/bin/hl build-cache
	

.PHONY: check install
