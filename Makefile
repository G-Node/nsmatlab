#Makefile to build neuroshare matlab filter
#some parts are taken from git's Makefile

SOURCES := src/main.c src/ns.c src/mexversion.c

ARCH := $(shell sh -c 'uname -m 2> /dev/null' || echo 'unkown')
OS   := $(shell sh -c 'uname -s 2> /dev/null' || echo 'unkown')
MATLAB_BINDIR = $(dir $(lastword $(shell sh -c 'readlink `which matlab`')))
MEXEXT        = $(shell sh -c '$(MATLAB_BINDIR)mexext')

CFLAGS  = -O2 -Wall -g
LDFLAGS =
ADD_CFLAGS  = $(CFLAGS) -std=c99 -I./ns -fPIC -DMATLAB_MEX_FILE -D_GNU_SOURCE
ADD_LDFLAGS = $(LDFLAGS) -ldl 

OUTDIR	    = $(OS)-$(ARCH)-bin
TARGET_BIN  = mexprog.$(MEXEXT)
TARGET      = $(OUTDIR)/$(TARGET_BIN)
TARNAME     = neuroshare-matlab-$(OS)-$(ARCH)

#Commands
ECHO     = @echo
CP       = cp
RM       = rm
TAR      = tar
GZIP     = gzip
MEX      = $(MATLAB_BINDIR)mex
STRIP   ?= strip

ifeq ($(OS),Linux)
	ADD_CFLAGS += -Dlinux
	ADD_LDFLAGS += -shared

endif
ifeq ($(OS),Darwin)
	ADD_LDFLAGS += -dynamiclib
endif



# Slient make
ifneq ($(findstring $(MAKEFLAGS),s),s)
ifndef V
	QUIET_BUILD	= @echo '   ' BUILD $@;
	QUIET_LNCP	= @echo '   ' LN/CP;
	QUIET_TAR       = @echo '   ' TAR $(TARNAME).tar;
	QUIET_GZIP      = @echo '   ' GZIP $(TARNAME).tar.gz;
	QUIET_STRIP     = @echo '   ' STRIP $(TARNAME)/$(TARGET_BIN);
	export V
endif
endif

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(QUIET_BUILD)$(MEX) CFLAGS="$(ADD_CFLAGS)" LDFLAGS="$(ADD_LDFLAGS)" -outdir $(OUTDIR) $^ -output $@
	$(QUIET_LNCP)$(CP) m-files/* $(OUTDIR)/

clean:
	$(RM) -rf $(OUTDIR)

dist: $(TARGET)
	@mkdir -p $(TARNAME)
	@cp $(OUTDIR)/*.m $(TARNAME)/
	@cp $(TARGET) $(TARNAME)/
	@mkdir -p $(TARNAME)/Examples
	@cp examples/*.m $(TARNAME)/Examples/
	$(QUIET_STRIP)$(STRIP) -x $(TARNAME)/$(TARGET_BIN)
	$(QUIET_TAR)$(TAR) -c -f $(TARNAME).tar $(TARNAME)
	$(QUIET_GZIP)$(GZIP) -f -9 $(TARNAME).tar
	@rm -rf $(TARNAME)

.PHONY: all install clean strip
