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
#include <stdio.h>
#include "graph.h"

#define STR_(X) #X
#define STR(X) STR_(X)

int
main(int argc, char **argv)
{
  if(3 != argc && 4 != argc){
    printf("SYNOPSIS\n");
    printf("\n");
    printf("./Wilson <input> <output> [<repetitions>]\n");
    printf("\n");
    printf("DESCRIPTION\n");
    printf("\n");
    printf("To use this program specify a file name as <input>. This file\n");
    printf("should contain a correct graph description. The <output> file name is\n");
    printf("used to store the resulting uniformly sampled spanning trees. The\n");
    printf("<repetitions> value is optional and indicates how many trees\n");
    printf("should be generated. The default value is 1. The resulting trees\n");
    printf("are stored in the <output> file, as lists of edges and are\n");
    printf("separated by blank lines.\n");
    printf("\n");
    printf("These spanning trees are generated using the random walk algorithm.\n");
    printf("\n");
  } else {
    printf("BSD 2-Clause License\n");
    printf("\n");
    printf("Copyright (c) 2020, Luís M. S. Russo\n");
    printf("All rights reserved.\n");
    printf("\n");
    printf("Redistribution and use in source and binary forms, with or without\n");
    printf("modification, are permitted provided that the following conditions are met:\n");
    printf("\n");
    printf("1. Redistributions of source code must retain the above copyright notice, this\n");
    printf("   list of conditions and the following disclaimer.\n");
    printf("\n");
    printf("2. Redistributions in binary form must reproduce the above copyright notice,\n");
    printf("   this list of conditions and the following disclaimer in the documentation\n");
    printf("   and/or other materials provided with the distribution.\n");
    printf("\n");
    printf("THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\"\n");
    printf("AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE\n");
    printf("IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE\n");
    printf("DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE\n");
    printf("FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL\n");
    printf("DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR\n");
    printf("SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER\n");
    printf("CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,\n");
    printf("OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE\n");
    printf("OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n");
    printf("\n");
    printf("Wilson algorithm, version %s\n", STR(_LIBUSTREE_VERSION) );
    printf("\n");
    printf("\n");

    unsigned long long int iterations = 1;
    if(4 == argc)
      sscanf(argv[3], "%ld", &iterations);  /* Read number of iterations */

    unsigned long long int i = 0; /* Number of executions */
    char *fileName = argv[1];
    FILE *fin = fopen(fileName, "r");
    fileName = argv[2];
    FILE *fout = fopen(fileName, "w");
    if(NULL != fin){
      if(NULL != fout){
	/* Load graph */
	graph G = loadGraph(fin);
	fclose(fin);

	int *A = (int*)malloc(graphVertexNr(G)*sizeof(int));
	if(NULL != A){
	  printf("All Ok. Generating trees.\n");

	  while(i < iterations){
	    printf("%d\n",i);
	    Wilson(G, &A[-1]);

	    int j = 0;
	    while(j < graphVertexNr(G)){
	      if(0 != A[j])
		fprintf(fout, "%d %d\n", j+1, A[j]);
	      j++;
	    }
 	    fprintf(fout, "\n");

	    i++;
	  }
	  fclose(fout);
	  free(A);
	} else {
	  printf("Error reading graph\n");
	}
	freeGraph(G);
      } else
	printf("Error opening output file\n");
    } else
      printf("Error opening input file\n");
  }
}
