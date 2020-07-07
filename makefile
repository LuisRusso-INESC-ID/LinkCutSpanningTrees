# BSD 2-Clause License

# Copyright (c) 2020, Luís M. S. Russo
# All rights reserved.

# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:

# 1. Redistributions of source code must retain the above copyright notice, this
#    list of conditions and the following disclaimer.

# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.

# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

_LIBUSTREE_VERSION=0.1.0-alpha
CFLAGS=-O3 -D NDEBUG
#CFLAGS=-g -ggdb

.PHONY: all

all: randomWalk Wilson edgeSwap

libustree.a: graph.o linkCutTree.o mixer.o
	ar rcs libustree.a graph.o linkCutTree.o mixer.o

graph.o: graph.h graph.c
	cc $(CFLAGS) -fPIC -c graph.h graph.c

linkCutTree.o: linkCutTree.h linkCutTree.c
	cc $(CFLAGS) -fPIC -c linkCutTree.h linkCutTree.c

mixer.o: mixer.h mixer.c
	cc $(CFLAGS) -fPIC -c mixer.h mixer.c

TIMELIMIT=10

randomWalk: randomWalk.c graph.h graph.c
	cc $(CFLAGS) -o randomWalk -D_LIBUSTREE_VERSION=$(_LIBUSTREE_VERSION) randomWalk.c graph.c -lbsd

Wilson: Wilson.c graph.h graph.c
	cc $(CFLAGS) -o Wilson -D_LIBUSTREE_VERSION=$(_LIBUSTREE_VERSION) Wilson.c graph.c -lbsd

edgeSwap: edgeSwap.c libustree.a
	cc $(CFLAGS) -o edgeSwap -D_LIBUSTREE_VERSION=$(_LIBUSTREE_VERSION) edgeSwap.c -L. -lustree -lbsd -lm

clean:
	-rm libustree.a \
	graph.o graph.h.gch \
	linkCutTree.o linkCutTree.h.gch \
	mixer.o mixer.h.gch \
	randomWalk \
	Wilson \
	edgeSwap \
	*~
