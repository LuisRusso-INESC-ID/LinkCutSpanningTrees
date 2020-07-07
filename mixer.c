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

/** @file Implementation of mixing process. */
#include <math.h>
#include <strings.h>
#include <assert.h>
#include <limits.h>
#include <string.h>
#include <sys/param.h>
#include <bsd/stdlib.h>

#include "mixer.h"

/* #warning "TODO: Mark with this." */
/*** typedefs(not structures) and defined constants *******/

/*** enums ************************************************/

/*** structure declarations *******************************/

struct sample {
  LCT A; /** The LinkCut Tree */
  int* P; /** Keeps P mappings for everyone */
  int* Pi; /** Inverse of P */
};

/*** file scope functions declarations (static) ***********/

/* Generates information about one step of the Markov Chain */
static void
stepInfo(int* i,
         int* ou,
         int* ov,
         LCT A,
         int* P,
         graph g
         );

/* Executes one step of the Markov Chain */
static void
stepExecute(int i,
            int ou,
            int ov,
            LCT A,
            int* P,
            graph g
            );

/** Swaps index i and j in the permutation P */
static void
pSwap(int* P /** [in] */,
      int e /** [in] Number of edges */,
      int i /** [in] */,
      int j /** [in] */
      );

#ifndef NDEBUG
/** Verifies if Ex is coerent with the tree */
static void
pAssert(LCT A /** [in]  */,
        int* Ex /** [in] */,
        graph g /** [in] */
        );
#endif /* NDEBUG */

/** Resets the SLP data structure. */

/*** declarations of public functions (first inlines) *****/

static void
stepInfo(int* i,
         int* ou,
         int* ov,
         LCT A,
         int* P,
         graph g
         )
{
  int c; /* Cycle size. */
  int idx; /* Cycle index of removed edge. */
  edge* E; /* Edge array */
  int iu;  /* Insert vertex */
  int iv;  /* Insert vertex */
  int j;
  int e = edgesNr(g);
  int V = vertexNr(A);
  E = edges(g);

  if (-1 == *i) {
    if (NULL != P) {
      j = arc4random_uniform(e-(V-1));
      j+= (V-1);
      *i = P[j];
      assert(!edgeQ(A, E[*i][0], E[*i][1]) && "Improper insert.");
    } else
      *i = arc4random_uniform(e);
  }

  /** Otherwise i is considered input. */
  *ou = 0;
  *ov = 0;
  iu = E[*i][0];
  iv = E[*i][1];
  if (!edgeQ(A, iu, iv)) {
    c = cycle(A, iu, iv);
    idx = arc4random_uniform(c-1)+1;
    *ou = selectAux(A, iu, idx);
    *ov = successor(A, *ou);
  }

#ifndef NDEBUG
  assert(edgeQ(A, *ou, *ov) && "Improper removal.");
  cycle(A, E[*i][0], E[*i][1]);
  assert(edgeQAux(A, E[*i][1], *ou, *ov) && "Improper removal.");
#endif /* NDEBUG */
}

static void
stepExecute(int i,
            int ou,
            int ov,
            LCT A,
            int* P,
            graph g
            )
{
  edge* E; /* Edge array */
  int e = edgesNr(g);
  int* Pi = &P[e];

  /* static int fc=0; */
  /* static char file[20]; */

  /* printf("Step %d, Retira (%d,%d), Insere (%d,%d)\n", ++fc, ou, ov, edges(g)[i][0], edges(g)[i][1]); */
  /* sprintf(file, "antes%d_dot", fc); */
  /* showRepTree(A, file); */
  /* sprintf(file, "antesFull%d_dot", fc); */
  /* showLCT(A, file); */

  E = edges(g);
  cut(A, ou, ov);
  /* sprintf(file, "cortada%d_dot", fc); */
  /* showRepTree(A, file); */
  /* sprintf(file, "cortadaFull%d_dot", fc); */
  /* showLCT(A, file); */

  link(A, E[i][0], E[i][1]);
  if (NULL != P)
    pSwap(P, e, Pi[i], Pi[edgeIdx(g, ou, ov)]);

  /* sprintf(file, "ligada%d_dot", fc); */
  /* showRepTree(A, file); */
  /* sprintf(file, "ligadaFull%d_dot", fc); */
  /* showLCT(A, file); */
}

static void
pSwap(int* P,
      int e,
      int i,
      int j
      )
{
  int* Pi;
  int t; /* Temporary hold */

  /* printf("Swap %d @ %d <-> %d  @ %d\n", P[i], i, P[j], j); */

  Pi = &P[e];

  t = P[i];
  P[i] = P[j];
  P[j] = t;

  Pi[P[i]] = i;
  Pi[P[j]] = j;
}

#ifndef NDEBUG
static void
pAssert(LCT A,
        int* P,
        graph g
        )
{
  int i;
  int e;
  int V;
  edge* E;
  int* Pi;

  if (NULL != P) {
    e = edgesNr(g);
    V = vertexNr(A);
    E = edges(g);
    Pi = &P[e];

    i = 0;
    while (i < V-1) {
      assert(edgeQ(A, E[P[i]][0], E[P[i]][1]));
      assert(Pi[P[i]] == i);
      i++;
    }
    while (i < e) {
      assert(!edgeQ(A, E[P[i]][0], E[P[i]][1]));
      assert(Pi[P[i]] == i);
      i++;
    }
  }
}
#endif /* NDEBUG */

LCT
loadLCT(graph g,
        int* P
        )
{
  LCT t;
  int V;
  int e;
  edge* E;
  int i;
  int* Pi;
  int j;

  V = graphVertexNr(g);
  E = edges(g);
  e = edgesNr(g);
  t = allocLCT(V);
  Pi = &P[e];

  j = V-1;
  i = 0;
  while(i < e && (NULL != P || V > 1)) {
    if (!linkedQ(t, E[i][0], E[i][1])) {
      link(t, E[i][0], E[i][1]);
      V--;
      P[V-1] = i;
      Pi[P[V-1]] = V-1;
    } else {
      P[j] = i;
      Pi[P[j]] = j;
      j++;
    }
    i++;
  }

#ifndef NDEBUG
  pAssert(t, P, g);
#endif /* NDEBUG */
  return t;
}

void
step(LCT A,
     int* P,
     graph g
     )
{
  int i;
  int ou;
  int ov;

  i = -1;
  stepInfo(&i, &ou, &ov, A, P, g);
  if (0 != ou && 0 != ov)
    stepExecute(i, ou, ov, A, P, g);
}

void
mixFixed(LCT A,
         int* P,
         graph g,
         int s
         )
{
  while (0 < s){
    step(A, P, g);
    s--;
  }
}

int *
initPermutation(graph G  /** [in] */
		)
{
  int e = edgesNr(G);

  int *P = (int *)malloc(2*e*sizeof(int));
  int* Pi = &P[e];
  for(int k = 0; k < e; k++){
    P[k] = k;
    Pi[P[k]] = k;
  }

  return P;
}
