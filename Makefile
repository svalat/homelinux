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
	mkdir -p $(PREFIX)/share/homelinux
	cp -r packages $(PREFIX)/share/homelinux
	cp -r node_modules $(PREFIX)/lib/homelinux
	

.PHONY: check install