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


#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

#include "linkCutTree.h"

/*** file scope macro definitions *************************/

/*** file scope typedefs  *********************************/

typedef struct LCT* LCTAux; /** Used to represent splay trees. */

struct LCT { /** This structure is used to represent a node in the LCT. */
  LCT left; /** Child */
  LCT right; /** Child */
  LCT* hook; /** Sort of parent pointer. Points directly to the position
                 that points back to this node. If NULL means that it is the
                 root of its sub-tree. It may point to a position that does
                 not point back, in wich case it is an accross Aux parent
                 pointer. */
  int sum; /** For now the size of this sub-tree. Can be changed to sum of
               weights. The value is assume to be >= 0. Negative values are
               used to indicate that left and right pointers should be
               swapped on the sub-tree. */
};

/*** global variables (externs or externables) ************/

/*** file scope variables (static) ************************/

static int ct = 1;
static char sstat[100];

/*** file scope functions declarations (static) ***********/

/** Resets invariant properties of v */
static void
resetNode(LCTAux v /**< [in] */
          );

/** @return 1 if this struct is flipped, 0 otherwise */
static int
flippedQ(LCTAux v /**< [in] */
         );

/** Changes a logical bit in the struct so that it behaves as flipped */
static void
flip(LCTAux v /**< [in] */
         );

/** Physically changes struct so that flip bit can be disabled. */
static void
unflip(LCTAux v /**< [in] */
     );

/** @return The size of the aux sub-tree rooted at v. */
static int
sizeAux(LCTAux v /**< [in] */
        );

/** IMPORTANT: This function only applies if v is the root of a splay tree.
    Changes the lower pointer of v to to. */
static void
setLower(LCTAux v /**< [in] Must be root of a splay tree. */,
         LCTAux to /**< [in] */
         );

/** @return 0 if s is NOT the root of the splay tree
    1 otherwise. */
static int
auxRootQ(LCTAux s /**< [in] */
         );

/** IMPORTANT: Assumes that the node is not flipped. Rotates node v, upwards. */
static void
rotate(LCT t /**< [in] */,
       LCTAux v /**< [in] */
       );

/** Splays u within its aux tree*/
static void
splay(LCT t /**< [in] */,
      int v /**< [in] */
      );

/** @return r, so that p points inside t[r]. */
static int
ptrToIndex(LCT t /**< [in] */,
           void *p /**< [in] */
           );

/** @return The aux parent of s. */
static int
auxParent(LCT t /**< [in] */,
          LCTAux s /**< [in] */
          );

/*** implementation ***************************************/

static void
resetNode(LCTAux v /**< [in] */
          )
{
  int b;

  b = 1;
  if (flippedQ(v))
    b = -1;

  v->sum = 0;

  if (NULL != v->left) {
    v->sum += sizeAux(v->left);
    v->left->hook = &(v->left);
  }

  v->sum++; /* Adding the node */

  if (NULL != v->right) {
    v->sum += sizeAux(v->right);
    v->right->hook = &(v->right);
  }

  v->sum *= b; /* Flip back */
}

static int
flippedQ(LCTAux v
         )
{
  return 0 > v->sum;
}

static void
flip(LCTAux v
     )
{
  v->sum *= -1;
}

static void
unflip(LCTAux v
     )
{
  LCTAux t;
  uint64_t ti;

  if (flippedQ(v))
    {
      flip(v);
      t = v->left;
      v->left = v->right;
      v->right = t;

      if (NULL != v->left){
        flip(v->left);
	v->left->hook = &(v->left);
      }
      if (NULL != v->right){
        flip(v->right);
	v->right->hook = &(v->right);
      }
    }
}

static int
sizeAux(LCTAux v
        )
{
  int r;

  r = 0;
  if (NULL != v)
    r = abs(v->sum);

  return r;
}

static void
setLower(LCTAux v,
         LCTAux to
         )
{
  unflip(v); /* Now branches are not flipped */
  v->sum -= sizeAux(v->right);
  v->right = to;
  v->sum += sizeAux(v->right);

  if (NULL != v->right)
    v->right->hook = &(v->right);

}

static int
auxRootQ(LCTAux s
         )
{
  int r;
  r = 0;
  if(NULL == s->hook)
    r = 1;
  else if (s != *(s->hook))
    r = 1;

  return r;
}

static void
rotate(LCT t,
       LCTAux v
       )
{
  int p; /* Parent node */
  int vs; /* Temporary v size */

  assert(!flippedQ(v) && "Trying to rotate flipped node.");

  if (!auxRootQ(v)) { /* Rotation is possible */
    p = auxParent(t, v);
    assert(!flippedQ(&t[p]) && "Trying to rotate flipped node.");

    vs = sizeAux(v);
    /* Reseting copies values for v */
    v->sum = t[p].sum;

    /* Reseting updates values for v */
    t[p].sum -= vs;

    if (v->hook == &(t[p].left)) {
      t[p].sum += sizeAux(v->right);

      *(v->hook) = v->right;
      if(NULL != *(v->hook))
	(*(v->hook))->hook = v->hook;

      v->hook = t[p].hook;
      if (NULL != v->hook && (LCT)&t[p] == *v->hook)
	*(v->hook) = v;

      v->right = &(t[p]);
      v->right->hook = &(v->right);
    } else {
      t[p].sum += sizeAux(v->left);

      *(v->hook) = v->left;
      if(NULL != *(v->hook))
	(*(v->hook))->hook = v->hook;

      v->hook = t[p].hook;
      if (NULL != v->hook && (LCT)&t[p] == *v->hook)
	*(v->hook) = v;

      v->left = &(t[p]);
      v->left->hook = &(v->left);
    }

    assert(0 != t[p].sum && "Size set to 0");
    assert(0 != v->sum && "Size set to 0");
  }
}

static void
splay(LCT t,
      int v
      )
{
  int p; /* Parent node */
  int g; /* Grand parent node */
  intptr_t pt; /* Integer pointer */

  while(0 == auxRootQ(&t[v])) { /* Not at the root of a splay tree */
    p = auxParent(t, &t[v]);

    if (0 == auxRootQ(&t[p])) { /* Then v has a grand parent */
      g = auxParent(t, &t[p]);
      unflip(&t[g]);
      unflip(&t[p]);
      unflip(&t[v]);

      pt = (intptr_t) t[v].hook;
      pt -= (intptr_t) t[p].hook;
      pt %= sizeof(struct LCT);

      if (0 == pt) /* Zig Zig case */
        rotate(t, &t[p]);
      else /* Zig Zag case */
        rotate(t, &t[v]);
    }
    unflip(&t[p]); /* Zig case precaution */
    unflip(&t[v]);
    rotate(t, &t[v]); /* Both cases and Zig */
  }

}

static int
ptrToIndex(LCT t,
           void *p
           )
{
  int r;
  intptr_t pr; /* Integer pointer */
  r = 0;

  if (NULL != p) {
    pr = (intptr_t) p;
    pr -= (intptr_t) t;
    pr /= sizeof(struct LCT);
    r = (int) pr;
  }

  return r;
}

static int
auxParent(LCT t,
          LCTAux s
          )
{
  return ptrToIndex(t, s->hook);
}

/* End of Static Functions */

LCT
allocLCT(int V /**< [in] Number of vertexes of the underlying graph */
         )
{
  LCT t;
  int i;

  t = (LCT) malloc((V+1)*sizeof(struct LCT));
  *(int*)t = V; /* Store number of vertexes */

  cleanLCT(t);

  return t;
}

void
cleanLCT(LCT t /** [in] The tree to clean up */
         )
{
  int i;
  int V;
  V = vertexNr(t);

  i = V;
  while(0 < i){
    t[i].left = NULL;
    t[i].right = NULL;
    t[i].hook = NULL;
    t[i].sum = 1;
    i--;
  }
}

LCT
clone(LCT t
      )
{
  LCT c; /** The clone */
  int i;
  int V;
  ptrdiff_t delta;

  V = vertexNr(t);
  c = (LCT) malloc((V+1)*sizeof(struct LCT));
  *(int*)c = V; /* Store number of vertexes */

  delta = (ptrdiff_t)c - (ptrdiff_t)t;

  i = V;
  while(0 < i) {
      c[i].left = NULL;
      if(NULL != t[i].left)
        c[i].left = (LCT)(delta + (ptrdiff_t)t[i].left);
      c[i].right = NULL;
      if(NULL != t[i].right)
        c[i].right = (LCT)(delta + (ptrdiff_t)t[i].right);
      c[i].hook = NULL;
      if(NULL != t[i].hook)
        c[i].hook = (LCT*)(delta + (ptrdiff_t)t[i].hook);

      c[i].sum = t[i].sum;
      i--;
    }

  return c;
}

void
array2LCT(int v,
	  int* A,
	  LCT O
	  )
{
  int i; /* counter */

  cleanLCT(O);
  i = 1;
  while(i <= v){
    if(0 < A[i])
      link(O, i, A[i]);
    i++;
  }
}

int
vertexNr(LCT t
         )
{
  return *(int*)t;
}

void
freeLCT(LCT t)
{
  free(t);
}

void
displayRepTree(LCT t,
	       FILE *f
               )
{
  int V;
  int i;
  int p;

  V = vertexNr(t);

  i = 1;
  while(i <= V)
    {
      if (0 != successor(t, i))
        fprintf(f, "%d %d\n", i, successor(t, i));
      if (auxRootQ(&t[i]) && NULL != t[i].hook) {
        p = auxParent(t, &t[i]);
        fprintf(f, "%d %d\n", selectAux(t, i, 1), p);
      }
      i++;
    }
  fprintf(f, "\n");
}

void
access(LCT t,
       int v
       )
{
  int w;

  splay(t, v);
  setLower(&t[v], NULL);

  while(NULL != t[v].hook) {
    w = auxParent(t, &t[v]);
    splay(t, w);
    setLower(&t[w], &t[v]);
    splay(t, v);
  }

  assert(flippedQ(&t[v]) || (NULL == t[v].right && "Failed access invariant."));
  assert(!flippedQ(&t[v]) || (NULL == t[v].left && "Failed access invariant."));
}

void
reRoot(LCT t,
       int v
       )
{
  access(t, v);
  flip(&t[v]);
  access(t, v);

}

int
linkedQ(LCT t,
       int u,
       int v
       )
{
  cycle(t, u, v);
  return selectAux(t, v, 1) == u;
}

void
link(LCT t,
     int r,
     int v
     )
{
  /* sprintf(sstat, "tree%.4d_Before_Link_%d_%d_dot", ct++, r, v); */
  /* showRepTree(t, NULL); */

  assert(r != v && "Cutting no edge");
  assert(!linkedQ(t, r, v) && "Link failed. Trees already linked.");
  reRoot(t, r);
  access(t, v);
  setLower(&t[v], &t[r]);

  /* sprintf(sstat, "tree%.4d_After_Link_%d_%d_dot", ct++, r, v); */
  /* showRepTree(t, NULL); */

}

int
cycle(LCT t,
      int u,
      int v
      )
{
  reRoot(t, u);
  access(t, v);
  return sizeAux(&t[v]);
}

int
sizePath(LCT t /**< [in] */,
      int u /**< [in] */
      )
{
  splay(t, u);
  return sizeAux(&t[u]);
}

int
position(LCT t,
         int v
         )
{
  int r;
  r = 1;

  splay(t, v);
  unflip(&t[v]);
  if (NULL != t[v].left)
    r += sizeAux(t[v].left);

  return r;
}

int
selectAux(LCT t /**< [in] */,
          int v /**< [in] */,
          int i /**< [in] The index of the edge, starting at 1 */
          )
{
  int r;
  LCTAux s;
  r = 0; /* Default for out of bounds */

  splay(t, v);
  s = &t[v];
  if (0 < i && sizeAux(s) >= i) {
    i--; /* Converted to how many elements do you need on the left */
    unflip(s);
    while (sizeAux(s->left) != i) {
      if (sizeAux(s->left) > i)
        s = s->left;
      else {
        i -= sizeAux(s);
        s = s->right;
        i += sizeAux(s);
      }
      unflip(s);
    }
    r = (int) (s-t);
    splay(t, r);
  }

  return r;
}

int
successor(LCT t,
          int v
          )
{
  splay(t, v);
  unflip(&t[v]);
  return selectAux(t, v, 2 + sizeAux(t[v].left));
}

int
predecessor(LCT t,
            int v
            )
{
  splay(t, v);
  unflip(&t[v]);
  return selectAux(t, v, sizeAux(t[v].left));
}

int
edgeQ(LCT t,
      int u,
      int v
      )
{
  int r;

  r = 0;
  if (u != v)
    {
      cycle(t, u, v);
      unflip(&t[v]);
      if(t[v].left == &t[u]){
      	unflip(t[v].left);
      	r = (NULL == t[v].left->right);
      }

      /* r = (predecessor(t, v) == u); */
    }
  return r;
}

int
edgeQAux(LCT t,
         int s,
         int u,
         int v
         )
{
  int r;
  r = 0;

  splay(t, s);

  if (successor(t, u) == v)
    r = 1;
  else if (predecessor(t, u) == v)
    r = 1;

  if(1 == r && s != u && s != v)
    r = !auxRootQ(&t[s]);

  return r;
}

void
cut(LCT t,
    int u,
    int v
    )
{
  /* sprintf(sstat, "tree%.4d_Before_Cut_%d_%d_dot", ct++, u, v); */
  /* showRepTree(t, NULL); */

  assert(u != v && "Cutting no edge");
  assert(edgeQ(t, u, v) && "Cutting edges that are not there");

  if(1 == edgeQ(t, u, v)) {
    cycle(t, u, v);
    unflip(&t[v]);
    assert(NULL != t[v].left && "Cut guard.");

    t[v].sum -= sizeAux(t[v].left);

    assert(0 != t[v].sum && "Size set to 0");

    t[v].left->hook = NULL;
    t[v].left = NULL;

    assert(sizeAux(&t[v]) > sizeAux(t[v].right) && "Size order");
    assert(sizeAux(&t[v]) > sizeAux(t[v].left) && "Size order");
  }
}
