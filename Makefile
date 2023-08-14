CC		= gcc
CXX		= g++

# -fsanitize=address,undefined,bounds-strict
CFLAGS		?= -O0 -g -fsanitize=address,undefined,bounds-strict -fstack-protector
CFLAGS		+= -Wall -Wextra -pedantic
CXXFLAGS	?= $(CFLAGS) -std=c++23

PREFIX		?= /usr/local
BINDIR		?= $(PREFIX)/bin
MANDIR		?= $(PREFIX)/man/man1
INSTALL		?= install -s

PROG		= cpplox
MAN		= $(PROG).1
CXXFILES	!= ls *.cc
CFILES		!= ls *.c 2> /dev/null
COBJS		= ${CFILES:.c=.o}
CXXOBJS		= ${CXXFILES:.cc=.oo}


all: $(PROG)

$(PROG): $(COBJS) $(CXXOBJS)
	$(CXX) $(COBJS) $(CXXOBJS) -o $@ $(LDFLAGS) $(CXXFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)
%.oo: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LDFLAGS)

install: all
	mkdir -p $(DESTDIR)$(BINDIR) $(DESTDIR)$(MANDIR)
	$(INSTALL) $(PROG) $(DESTDIR)$(BINDIR)
	$(INSTALL) -m 644 $(MAN) $(DESTDIR)$(MANDIR)/$(MAN)

clean:
	rm -f $(PROG) $(COBJS) $(CXXOBJS)

.PHONY: all install clean
