all: hl-targets

hl_sys-libs_ncurses_0:
	@bash hl_sys-libs_ncurses_0.sh hl_start
	@bash hl_sys-libs_ncurses_0.sh hl_prebuild
	@bash hl_sys-libs_ncurses_0.sh hl_build $PWD/hl_sys-libs_ncurses_0hl-is-default-build.notify
	@if test -f $PWD/hl_sys-libs_ncurses_0hl-is-default-build.notify; then make -C `cat $PWD/hl_sys-libs_ncurses_0hl-is-default-build.notify` fi
	@bash hl_sys-libs_ncurses_0.sh hl_postbuild
	@bash hl_sys-libs_ncurses_0.sh hl_finish
hl_app-shells_bash_0:
	@bash hl_app-shells_bash_0.sh hl_start
	@bash hl_app-shells_bash_0.sh hl_prebuild
	@bash hl_app-shells_bash_0.sh hl_build $PWD/hl_app-shells_bash_0hl-is-default-build.notify
	@if test -f $PWD/hl_app-shells_bash_0hl-is-default-build.notify; then make -C `cat $PWD/hl_app-shells_bash_0hl-is-default-build.notify` fi
	@bash hl_app-shells_bash_0.sh hl_postbuild
	@bash hl_app-shells_bash_0.sh hl_finish
hl_app-shells_bash_0: hl_app-shells_bash_0

hl-targets: hl_sys-libs_ncurses_0 hl_app-shells_bash_0

.PHONY: hl-targets all hl_sys-libs_ncurses_0 hl_app-shells_bash_0
