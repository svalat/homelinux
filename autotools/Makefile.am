######################################################
#            PROJECT  : homelinux                    #
#            VERSION  : 2.0.0-dev                    #
#            DATE     : 06/2017                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4
SUBDIRS = extern-deps/jsoncpp/dist src manpages

######################################################
#install
install-data-hook:
	$(mkinstalldirs) $(DESTDIR)$(prefix)/homelinux
	$(mkinstalldirs) $(DESTDIR)$(prefix)/homelinux/install-db
	$(mkinstalldirs) $(DESTDIR)$(prefix)/homelinux/distfiles
	$(mkinstalldirs) $(DESTDIR)$(prefix)/lib
	$(mkinstalldirs) $(DESTDIR)$(prefix)/bin
	cd $(DESTDIR)$(prefix) && ln -sf lib lib64
	cp -r $(srcdir)/../bin/protected $(DESTDIR)$(prefix)/bin
	cp -r $(srcdir)/../packages $(DESTDIR)$(prefix)/homelinux
	cp -r $(srcdir)/../compilers $(DESTDIR)$(prefix)/homelinux
	cp -r $(srcdir)/../bin/hl-py-env.sh $(DESTDIR)$(bindir)
	test -f $(DESTDIR)$(prefix)/homelinux.json || cp -r $(srcdir)/../configs/homelinux.json $(DESTDIR)$(prefix)
	$(DESTDIR)$(bindir)/hl --no-user-config -p $(DESTDIR)$(prefix) update-cache
