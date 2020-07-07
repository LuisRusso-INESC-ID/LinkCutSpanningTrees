/* BSD 2-Clause License */

/* Copyright (c) 2020, Luís M. S. Russo */
/* All rights reserved. */

/* Redistribution and use in source and binary forms, with or without */
/* modification, are permitted provided that the following conditions are met: */

/* 1. Redistributions of source code must retain the above copyright notice, this */
/*    list of conditions and the following disclaimer. */

/* 2. Redistributions in binary form must reproduce the above copyright notice, */
/*    this list of conditions and the following disclaimer in the documentation */
/*    and/or other materials provided with the distribution. */

/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" */
/* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE */
/* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE */
/* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE */
/* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL */
/* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR */
/* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER */
/* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, */
/* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE */
/* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */


/** @file Header file for the Link Cut Tree ADT. */

#ifndef LINKCUTTREE_H
#define LINKCUTTREE_H

#include <stdio.h>

typedef struct LCT* LCT; /** Link-Cut tree abstract data type. */

/** @return Empty LCT, over vertexes labelled 1 to V */
LCT
allocLCT(int V /**< [in] Number of vertexes of the underlying graph */
         );

/** @return Empty LCT, over vertexes labelled 1 to V */
void
cleanLCT(LCT t /** [in] The tree to clean up */
         );

/** Makes a copy of the LCT tree. */
LCT
clone(LCT t /** [in] Original tree. */
      );

/** Make an LCT from array info */
void
array2LCT(int v /** [in] number of vertexes */,
	  int* A,
	  LCT O /** [in] original tree to keep hash values */
	  );

/** Free the LCT */
void
freeLCT(LCT t /** [in] */
        );

void
displayRepTree(LCT t /** [in] */,
	       FILE *f /** [in] */
               );

/** @return Number of vertexes in t. */
int
vertexNr(LCT t
         );

/** Contain the path from root to v in a splay tree. */
void
access(LCT t /**< [in] */,
       int v /**< [in] */
       );

/** Changes the root of the LCT to node u. Out of range values checked by
    asserts. */
void
reRoot(LCT t /**< [in] */,
       int v /**< [in] The node that will become the new root */
       );

/** @return 1 if there is a path linking u to v.
            0 otherwise */
int
linkedQ(LCT t /**< [in] */,
        int u /**< [in] */,
        int v /**< [in] */
        );

/** Add the edge (r,v) to the LCT. IMPORTANT: Sub-trees containing r and v
    must not be linked. Therefore linkedQ(t, r, v) must be false. */
void
link(LCT t /**< [in] */,
     int r /**< [in] Becomes the root of its sub-tree, before linking. */,
     int v /**< [in] */
     );

/** Contains the cycle between u and v in a splay tree. IMPORANT: this is
    only possible if linkedQ(t, u, v) is true, otherwise results are
    inconsistent.

    In general this function could be called pathify. I use only on cycles,
    but coding is general.

    @return the size of the resulting cycle. Number of vertexes involved. */
int
cycle(LCT t /**< [in] */,
      int u /**< [in] */,
      int v /**< [in] */
      );

/** Obtains the size of an existing path, where a certain vertex exists.

    @return the size of the existing path.
 */
int
sizePath(LCT t /**< [in] */,
	 int u /**< [in] */
	 );

/** @return The position of v in its tree. */
int
position(LCT t /** [in] */,
         int v /** [in] */
         );

/** @return the i-th vertex of the tree containing vertex v. Vertex 1 is
    the root. If out of bounds returns 0. */
int
selectAux(LCT t /**< [in] */,
          int v /**< [in] */,
          int i /**< [in] The index of the edge, starting at 1 */
          );

/** @return The next element in depth order. If out of bounds returns 0. */
int
successor(LCT t /**< [in] */,
          int v
          );

/** @return The previous element in depth order. If out of bounds returns 0. */
int
predecessor(LCT t /**< [in] */,
            int v
            );

/** @return 0 if (u,v) is not on the LCT
            1 otherwise */
int
edgeQ(LCT t /**< [in] */,
      int u /**< [in] */,
      int v /**< [in] */
      );

/** IMPORTANT: before this function can be applied, the aux tree containing
    (u,v) must be selected, with a cycle call. Works across sub-trees.
    @return 0 if (u,v) is not on this Aux tree.
            1 otherwise. */
int
edgeQAux(LCT t /**< [in] */,
         int s /**< [in] vertex from the aux tree. */,
         int u /**< [in] */,
         int v /**< [in] */
         );

/** Remove the edge (u,v) from the LCT, from whatever aux tree that
    contains it. Does nothing if edge is not there. */
void
cut(LCT t /**< [in] */,
    int u /**< [in] Becomes the root of its sub-tree. */,
    int v /**< [in] */
    );

#endif /* LINKCUTTREE_H */
