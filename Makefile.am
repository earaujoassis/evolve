EXTRA_DIST = evolve.def README.md LICENSE

pkginclude_HEADERS = \
	evolve.h \
	evolve_common.h \
	evolve_ev.h \
	evolve_ga.h \
	evolve_gaes.h \
	evolve_info.h \
	evolve_intstring.h \
	evolve_ivfga.h \
	evolve_permutation.h \
	evolve_randomkey.h \
	evolve_repr_real.h \
	evolve_repr_integer.h \
	evolve_repr_sorting.h \
	evolve_rkgaes.h \
	evolve_rkivfgaes.h \
	evolve_rng.h \
	evolve_settings.h \
	evolve_stats.h
lib_LTLIBRARIES = libevolve.la
libevolve_la_SOURCES = \
	config.h \
	evolve_common.c \
	evolve_ev.c \
	evolve_ga.c \
	evolve_gaes.c \
	evolve_info.c \
	evolve_intstring.c \
	evolve_ivfga.c \
	evolve_permutation.c \
	evolve_randomkey.c \
	evolve_repr_integer.c \
	evolve_repr_real.c \
	evolve_repr_sorting.c \
	evolve_rkgaes.c \
	evolve_rkivfgaes.c \
	evolve_rng.c \
	evolve_settings.c \
	evolve_stats.c
libevolve_la_LDFLAGS = \
	-no-undefined \
	-export-symbols-regex '^evolve_' \
	-version-info 11:0:7

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = evolve.pc

.PHONY: thirdparty examples tests clean

thirdparty:
	mkdir -p build
	$(MAKE) -C thirdparty build

examples:
	mkdir -p build
	$(MAKE) -C examples build

tests:
	$(MAKE) -C tests build

clean:
	$(MAKE) -C tests clean
	$(MAKE) -C examples clean
	rm -rf build
	rm -rf .deps
	rm -rf .libs
	rm -rf autom4te.cache
	rm -rf build
	rm -rf m4
	rm -f .DS_Store *.lo *.o aclocal.m4 autoscan.log
	rm -f compile config.guess config.h config.h.in~ config.log config.status config.sub configure
	rm -f depcomp install-sh libevolve.la libtool ltmain.sh
	rm -f Makefile Makefile.in missing evolve.pc stamp-h1
