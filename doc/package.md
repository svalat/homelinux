Packages
========

This document provide synthetic documentation on how to write your own packages.

Example
-------

```json
	{
		"name": "app-shells/bash",
		"homepage": "http://tiswww.case.edu/php/chet/bash/bashtop.html",
		"inherit": "autotools",
		"versions": [
			"4.0",
			"4.1",
			"4.2",
			"4.3"
		],
		"provide": [
			"gfortran"
		],
		"vfetcher": {
			"mode": "ftp",
			"url": "ftp://ftp.cwru.edu/pub/bash",
			"regexp": "bash-([0-9]+.[0-9]+).tar.gz"
		},
		"md5" : {
			"4.0": "azekljqsmfkljsfmkljsdmlfkqflj",
			"4.3": "81348932d5da294953e15d4814c74dd1"
		},
		"subdir": "bash-${VERSION}",
		"urls" : [
			"ftp://ftp.cwru.edu/pub/bash/bash-${VERSION}.tar.gz",
			"ftp://otherftp.cwru.edu/pub/bash/bash-${VERSION}.tar.gz"
		],
		"deps": [
			"make",
			"gcc > 3.4 < 4.8 ! 4.5.8",
			"install = 4.5",
			"curses@dev[+shell,-gui] > 3.4 < 4.8 ! 4.5.8"
		],
		"host": {
			"default": true,
			"debian8": {
				"std": "bash",
				"dev": "bash",
				"use": ["shell"]
			}
		},
		"configure": {
			"": [ "--enable-always", "--with-curses=$(hl_prefix curses)" ],
			"debug": ["--$enable-debug"],
			"+profile": ["--enable-profile"],
			"-profile": ["--disable-profile"]
		},
		"vspecific": {
			"4.2": {
				"configure": {
					"": [ "--enable-bugfix" ]
				}
			},
			"4.2 on debian8": {
				"configure": {
					"": [ "!--enable-bugfix" ]
				}
			}
		},
		"steps": {
			"test": []
		}
	}
```