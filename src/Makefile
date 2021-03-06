#
# This file is part of DGD, https://github.com/dworkin/dgd
# Copyright (C) 1993-2010 Dworkin B.V.
# Copyright (C) 2010-2012 DGD Authors (see the commit log for details)
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
OS=$(shell uname -s)
ifeq ($(OS),Darwin)
  HOST=DARWIN
endif
ifeq ($(OS),Linux)
  HOST=LINUX
endif
ifeq ($(OS),FreeBSD)
  HOST=FREEBSD
endif
ifeq ($(OS),NetBSD)
  HOST=FREEBSD
endif
ifeq ($(OS),OpenBSD)
  HOST=FREEBSD
endif
ifeq ($(OS),SunOS)
  HOST=SOLARIS
endif
ifndef HOST
  $(error HOST is undefined)
endif

DEFINES=-D$(HOST)	# -DSLASHSLASH -DNETWORK_EXTENSIONS -DCLOSURES -DCO_THROTTLE=50
DEBUG=	-O -g
CCFLAGS=$(DEFINES) $(DEBUG)
CFLAGS=	-I. -Icomp -Ilex -Ied -Iparser -Ikfun $(CCFLAGS)
LDFLAGS=
LIBS=	-ldl
LINTFLAGS=-abcehpruz
CC=	gcc
LD=	$(CC)
YACC=	yacc
BIN=	../bin

ifeq ($(HOST),FREEBSD)
  LIBS=
endif
ifeq ($(HOST),SOLARIS)
  LIBS+=-lsocket -lnsl
endif

SRC=	alloc.c error.c hash.c swap.c str.c array.c object.c sdata.c data.c \
	path.c editor.c comm.c call_out.c interpret.c config.c ext.c dgd.c
OBJ=	alloc.o error.o hash.o swap.o str.o array.o object.o sdata.o data.o \
	path.o editor.o comm.o call_out.o interpret.o config.o ext.o dgd.o
COMPOBJ=alloc.o error.o hash.o path.o str.o array.o object.o sdata.o data.o \
	interpret.o config.o ext.o

a.out:	$(OBJ) always
	cd comp; $(MAKE) 'CC=$(CC)' 'CCFLAGS=$(CCFLAGS)' 'YACC=$(YACC)' dgd
	cd lex; $(MAKE) 'CC=$(CC)' 'CCFLAGS=$(CCFLAGS)' dgd
	cd ed; $(MAKE) 'CC=$(CC)' 'CCFLAGS=$(CCFLAGS)' dgd
	cd parser; $(MAKE) 'CC=$(CC)' 'CCFLAGS=$(CCFLAGS)' dgd
	cd kfun; $(MAKE) 'CC=$(CC)' 'CCFLAGS=$(CCFLAGS)' dgd
	cd lpc; $(MAKE) 'CC=$(CC)' 'HOST=$(HOST)' 'CCFLAGS=$(CCFLAGS)' \
			'YACC=$(YACC)' 'LIBS=$(LIBS)' dgd
	cd host; $(MAKE) 'CC=$(CC)' 'HOST=$(HOST)' 'CCFLAGS=$(CCFLAGS)' dgd
	$(LD) $(DEBUG) $(LDFLAGS) -o $@ $(OBJ) `cat comp/dgd` `cat lex/dgd` \
	      `cat ed/dgd` `cat parser/dgd` `cat kfun/dgd` `cat lpc/dgd` \
	      `cat host/dgd` $(LIBS)

always:
	@rm -f a.out

all:	a.out

$(BIN)/driver: a.out
	-mv $(BIN)/driver $(BIN)/driver.old
	cp a.out $(BIN)/driver

install: $(BIN)/driver

lint:
	lint $(LINTFLAGS) $(CFLAGS) $(SRC)
	@cd comp; $(MAKE) 'LINTFLAGS=$(LINTFLAGS)' 'CCFLAGS=$(CCFLAGS)' lint
	@cd lex; $(MAKE) 'LINTFLAGS=$(LINTFLAGS)' 'CCFLAGS=$(CCFLAGS)' lint
	@cd ed; $(MAKE) 'LINTFLAGS=$(LINTFLAGS)' 'CCFLAGS=$(CCFLAGS)' lint
	@cd parser; $(MAKE) 'LINTFLAGS=$(LINTFLAGS)' 'CCFLAGS=$(CCFLAGS)' lint
	@cd kfun; $(MAKE) 'LINTFLAGS=$(LINTFLAGS)' 'CCFLAGS=$(CCFLAGS)' lint
	@cd lpc; $(MAKE) 'LINTFLAGS=$(LINTFLAGS)' 'CCFLAGS=$(CCFLAGS)' lint
	@cd host; $(MAKE) 'LINTFLAGS=$(LINTFLAGS)' 'CCFLAGS=$(CCFLAGS)' lint

comp.sub: $(COMPOBJ)
	@for i in $(COMPOBJ); do echo ../$$i; done > comp.sub

lex.sub: $(LEXOBJ)
	@for i in $(LEXOBJ); do echo ../$$i; done > lex.sub

ed.sub:	$(EDOBJ)
	@for i in $(EDOBJ); do echo ../$$i; done > ed.sub

comp/parser.h: comp/parser.y
	cd comp; $(MAKE) 'YACC=$(YACC)' parser.h

comp/a.out:
	cd comp; $(MAKE) 'CC=$(CC)' 'HOST=$(HOST)' 'CCFLAGS=$(CCFLAGS)' \
			 'YACC=$(YACC)' 'LIBS=$(LIBS)' a.out

clean:
	rm -f a.out $(OBJ) comp.sub lex.sub ed.sub
	cd comp; $(MAKE) clean
	cd lex; $(MAKE) clean
	cd ed; $(MAKE) clean
	cd parser; $(MAKE) clean
	cd kfun; $(MAKE) clean
	cd lpc; $(MAKE) clean
	cd host; $(MAKE) clean


path.o config.o dgd.o: comp/node.h comp/compile.h
config.o data.o interpret.o: comp/csupport.h
config.o: comp/parser.h
config.o sdata.o interpret.o ext.o: comp/control.h

config.o: lex/macro.h lex/token.h lex/ppcontrol.h

editor.o: ed/edcmd.h

data.o: parser/parse.h

interpret.o ext.o: kfun/table.h

$(OBJ):	dgd.h config.h host.h alloc.h error.h
error.o str.o array.o object.o data.o path.o comm.o: str.h array.h object.h
editor.o call_out.o interpret.o config.o ext.o dgd.o: str.h array.h object.h
array.o data.o call_out.o interpret.o path.o config.o ext.o dgd.o: xfloat.h
error.o array.o object.o data.o path.o editor.o comm.o call_out.o: interpret.h
interpret.o config.o ext.o dgd.o: interpret.h
error.o str.o array.o object.o data.o path.o comm.o call_out.o: data.h
interpret.o config.o ext.o dgd.o: data.h
path.o config.o: path.h
hash.o str.o: hash.h
swap.o object.o data.o: swap.h
editor.o config.o dgd.o: editor.h
call_out.o config.o dgd.o: call_out.h
error.o comm.o config.o dgd.o: comm.h
comm.o config.o: version.h
