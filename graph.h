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

/**
 * @file   graph.h
 * @author Luís M. S. Russo <lmsrusso@gmail.com>
 * @date   Wed May  8 09:55:20 2013
 *
 * @brief A definition of a graph ADT, but not very abstract. The graphs
 * are undirected and unweighted. The vertexes are numbered 1 to V.
 *
 */

#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>

/* #warning "TODO: Mark with this." */

/*** typedefs(not structures) and defined constants *******/

typedef struct graph* graph; /* Graph ADT. */
typedef int edge[2]; /* An edge is simply a pair of integers. */

/*** enums ************************************************/

/*** structures declarations (only if really necessary) ***/

/*** declarations of public functions (first inlines) *****/

/** @return Reads a graph from a file. */
graph
loadGraph(FILE *f /** [in] load graph from file. */
          );

/** @return A graph structure from an edge array. */
graph
newGraph(int v /** [in] number of vertexes */,
         int e /** [in] number of edges */,
         edge* E /** [in] edge array that gets owned by the the graph */
         );

/** Frees the graph structure */
void
freeGraph(graph G /** [in] */
          );

/** @return Number of vertexes in the graph. */
int
graphVertexNr(graph G /** [in] */
              );

/** @return Number of edges in the graph. */
int
edgesNr(graph G /** [in] */
       );

/** @return Array of edges */
edge *
edges(graph G /** [in] */
      );

/** @return True if the two edges are equal. */
int
sameE(int au /** [in] */,
      int av /** [in] */,
      int bu /** [in] */,
      int bv /** [in] */
      );

/** @return The index of this edge in E. */
int
edgeIdx(graph G /** [in]  */,
        int u   /** [in]  */,
        int v   /** [in]  */
        );

/** Prints on screen for debugging. */
void
printGraph(graph G /**< [in] The graph */
           );

/** Executes the random walk algorithm */
void
randomWalk(graph G /** [in] The graph to walk over */,
           int* A /** [out] Vertex indexed array to store the tree, needs
		      to be pre-alloced. */
           );

/** Executes Wilson's algorithm */
void
Wilson(graph G /** [in] */,
       int* A /** [out] Vertex indexed array to store the tree, needs to be
		  pre-alloced. */
       );

#endif /* GRAPH_H */
