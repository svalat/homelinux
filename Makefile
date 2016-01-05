######################################################
#           Project : HomeLinux                      #
#           Version : 1.1.0-dev                      #
#           Date    : 01/2016                        #
#           Licence : BSD                            #
#           Authors : Sebastien Valat                #
######################################################

DEST=$(DESTDIR)/$(PREFIX)

all:

check:
	@if test -z "$(DEST)"; then echo "ERROR : Please provide PREFIX as parameter to setup install directory"; exit 1; fi

install: check
	mkdir -p $(DEST)
	test -f $(DEST)/homelinux.json || cp prefix.json $(DEST)/homelinux.json
	mkdir -p $(DEST)/bin
	cp bin/homelinux.sh $(DEST)/bin/homelinux
	cp bin/homelinux.sh $(DEST)/bin/hl
	cp bin/hl-env.sh $(DEST)/bin/hl-env
	cp bin/hl-py-env.sh $(DEST)/bin/hl-py-env
	cp bin/hl-build-cache.sh $(DEST)/bin/hl-build-cache
	mkdir -p $(DEST)/lib/homelinux
	cp -r index.js libs $(DEST)/lib/homelinux
	sed -i $(DEST)/lib/homelinux/libs/UserConfig.js -e "s#HL_DEFAULT_PREFIX#${PREFIX}#g"
	mkdir -p $(DEST)/homelinux
	cp -r packages $(DEST)/homelinux
	cp -r node_modules $(DEST)/lib/homelinux
	mkdir -p $(DEST)/homelinux/install-db
	$(DEST)/bin/hl build-cache
	

.PHONY: check install
