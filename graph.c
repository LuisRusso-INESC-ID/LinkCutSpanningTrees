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
#include <string.h>
#include <bsd/stdlib.h>
/* #include <stdio.h> */
#include "graph.h"

/*** file scope macro definitions *************************/

/*** file scope typedefs  *********************************/

/*** structure declarations *******************************/

struct graph
{
  int v; /* Number of vertexes in the graph. */
  int* d; /* Array indexed from 1 to v. Value d[i] indicates the begining
             of the interval in the p array such that those pointers point
             to edges that contain i. As a side effect the number of edges
             is d[v+1]/2. */
  int* p; /* Contains indexes to the array E. */
  edge* E; /* Edge array. This array is indexed starting at 0. */
};


/*** global variables (externs or externables) ************/

/*** file scope variables (static) ************************/

/*** file scope functions declarations (static) ***********/

/*** public functions *************************************/

/* Stores a graph with count sort LSD */

graph
loadGraph(FILE *f
          )
{
  int t; /* temporary var for swaps. */
  int i;
  int v;
  int e;
  edge* E;

  fscanf(f, "%d", &v);
  fscanf(f, "%d", &e);

  E = (edge*) malloc(e*sizeof(edge));
  i = 0;
  while (i < e) {
    fscanf(f, "%d", &E[i][0]);
    fscanf(f, "%d", &E[i][1]);
    if(E[i][0] > E[i][1]) {
      t = E[i][0];
      E[i][0] = E[i][1];
      E[i][1] = t;
    }
    i++;
  }

  return newGraph(v, e, E);
}

graph
newGraph(int v,
         int e,
         edge* E
         )
{
  graph G;
  int size;
  int i;
  int* T; /* temporary array */
  int* O; /* temporary array for orientation */

  size = sizeof(struct graph); /* Struct */
  size += (v+3)*sizeof(int);   /* Array d */
  size += 2*e*sizeof(int);     /* Array p */
  size += e*sizeof(edge);      /* Array E */
  E = realloc(E, size);
  G = (graph)&(E[e]); /* Locate the struct at the end of E */
  G->v = v; /* For small structs this value is positive. */
  G->d = (int*)&(G[1]); /* d is at the end of graph struct */
  G->p = &(G->d[v+3]);  /* p is at the end of d */
  G->E = E;

  T = (int*) malloc(2*e*sizeof(int));
  O = (int*) malloc(2*e*sizeof(int));

  /* ********** Count Degrees */
  bzero(G->d, (v+3)*sizeof(int));
  i = 0;
  while(i < e) {
    G->d[E[i][0]+3]++;
    G->d[E[i][1]+3]++;
    i++;
  }

  /* ********** Accumulate */
  i = 1;
  while(i+2 < v+3) {
    G->d[i+2] += G->d[i+1];
    i++;
  }

  /* ********** Radix sort LSD, first step */
  i = 0;
  while(i < e)
    {
      T[G->d[E[i][0]+2]] = i;
      O[G->d[E[i][0]+2]] = 1;
      G->d[E[i][0]+2]++;

      T[G->d[E[i][1]+2]] = i;
      O[G->d[E[i][1]+2]] = 0;
      G->d[E[i][1]+2]++;
      i++;
    }

  /* ********** Radix sort LSD, second step */
  i = 0;
  while(i < 2*e) {
    G->p[G->d[E[T[i]][O[i]]+1]] = T[i];
    G->d[E[T[i]][O[i]]+1]++;
    i++;
  }

  free(O);
  free(T);

  return G;
}

void
freeGraph(graph G
          )
{
  free(G->E);
}

int
graphVertexNr(graph G /** [in] */
         )
{
  return G->v;
}

int
edgesNr(graph G
       )
{
  return G->d[1+abs(G->v)]/2;
}

edge *
edges(graph G
      )
{
  return G->E;
}

int
sameE(int au,
      int av,
      int bu,
      int bv
      )
{
  int r;
  r = 0;
  if(au == bu && av == bv)
    r = 1;
  if(au == bv && av == bu)
    r = 1;

  return r;
}

int
edgeIdx(graph G,
        int u,
        int v
        )
{
  int l; /**< Left for binary search */
  int r; /**< Right for binary search */
  int m; /**< Middle for binary search */
  int t; /**< Test vertex */

  l = -1;
  if (NULL != G && NULL != G->d)
    {
      l = G->d[u];
      r = G->d[u+1];
      while(l+1 < r)
        {
          m = (l+r)/2;
          t = G->E[G->p[m]][0];
          if (u == t)
            t = G->E[G->p[m]][1];
          if (t > v)
            r = m;
          else
            l = m;
        }
    }

  return G->p[l];
}

void
printGraph(graph G
           )
{
  int i;
  int j;
  int v;

  i = 0;
  while(i < G->v)
    {
      printf("[%d] : ", i);
      j = G->d[i];
      while(j < G->d[i+1])
        {
          v = G->E[G->p[j]][0];
          if (i == v)
            v = G->E[G->p[j]][1];
          printf("%d ", v);
          j++;
        }
      printf("\n");
      i++;
    }
}

void
randomWalk(graph G,
           int* A
           )
{
  int i; /* Counter and number of visited nodes. */
  int V; /* Number of vertexes in graph. */
  int u; /* The current vertex */
  int v; /* The next vertex */
  edge* e; /* Edge selected */
  int d; /* Degree of u */
  int j;

  V = graphVertexNr(G);

  i = 1;
  while(i <= V) {
    A[i] = -1;
    i++;
  }

  u = arc4random_uniform(V) + 1;
  A[u] = 0;

  i = 1;
  while(i < V) {
    j = G->d[u];
    d = arc4random_uniform(G->d[u+1] - G->d[u]);
    e = &(G->E[G->p[j + d]]);

    v = (*e)[0];
    if( v == u )
      v = (*e)[1];

    if (-1 == A[v]) { /* Avoid visited nodes */
      A[v] = u;
      i++;
    }
    u = v;
  }
}

/** Swaps values and updates the respective inverse permutations */
static void
swap(int* P,
     int* Pi,
     int i,
     int j
     )
{
  int t; /* Temporary */

  t = P[i];
  P[i] = P[j];
  P[j] = t;

  Pi[P[i]] = i;
  Pi[P[j]] = j;
}

void
Wilson(graph G,
       int* A
       )
{
  int i; /* Number of painted nodes */
  int V; /* Number of vertexes */
  int v;
  int u;
  edge* e;
  int d;
  int j;
  int* C; /* Node color */
  int c;  /* number of colors */
  int* P; /* Permutation for starting */
  int* Pi; /* Inverse permutation */

  V = graphVertexNr(G);
  C = (int*) calloc(V+1, sizeof(int));
  P = (int*) calloc(V, sizeof(int));
  Pi = (int*) calloc(V+1, sizeof(int));

  /* A[0] = -1; */
  i = 1;
  while(i <= V) {
    /* A[i] = -1; */
    P[i-1] = i;
    Pi[i] = i-1;
    i++;
  }

  i = 0;
  d = arc4random_uniform(V);
  u = P[d];

  c = 1;
  A[u] = 0;
  C[u] = c;

  i++;
  swap(P, Pi, d, V-i);

  while(i < V) {
    c++;
    d = arc4random_uniform(V-i);
    u = P[d];
    C[u] = c;
    i++;
    swap(P, Pi, d, V-i);

    v = u;
    while (C[v] == c) {
      j = G->d[u];
      d = arc4random_uniform(G->d[u+1] - G->d[u]);
      e = &(G->E[G->p[j + d]]);

      v = (*e)[0];
      if (v == u)
        v = (*e)[1];

      if (C[v] < c) { /* Join */
        A[u] = v;
      }
      if (0 == C[v]) {
        C[v] = c;
        i++;
        swap(P, Pi, Pi[v], V-i);
        u = v;
      } else if (C[v] == c) { /* Loop erasure */
        while (u != v) {
          C[u] = 0; /* Uncoloring */
          i--;
          u = P[V-i]; /* Poping from the end of P */
        }
      }
    }
  }

  if (NULL != Pi)
    free(Pi);
  if (NULL != P)
    free(P);
  if (NULL != C)
    free(C);
}
