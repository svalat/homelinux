######################################################
#            PROJECT  : homelinux                    #
#            VERSION  : 2.0.0-dev                    #
#            DATE     : 06/2017                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4
SUBDIRS = extern-deps/jsoncpp/dist extern-deps/re2-2017-06-01/ src manpages

######################################################
#install
install-data-hook:
	$(mkinstalldirs) $(DESTDIR)$(prefix)/homelinux
	$(mkinstalldirs) $(DESTDIR)$(prefix)/homelinux/install-db
	$(mkinstalldirs) $(DESTDIR)$(prefix)/homelinux/distfiles
	$(mkinstalldirs) $(DESTDIR)$(prefix)/homelinux/lib
	cd $(DESTDIR)$(prefix)/homelinux/ && ln -sf lib lib64
	cp -r $(srcdir)/../packages $(DESTDIR)$(prefix)/homelinux
	cp -r $(srcdir)/../compilers $(DESTDIR)$(prefix)/homelinux
	cp -r $(srcdir)/../bin/hl-py-env.sh $(DESTDIR)$(bindir)
	test -f $(DESTDIR)$(prefix)/homelinux.json || cp -r $(srcdir)/../configs/homelinux.json $(DESTDIR)$(prefix)
	$(DESTDIR)$(bindir)/hl --no-user-config -p $(DESTDIR)$(prefix) update-cache
