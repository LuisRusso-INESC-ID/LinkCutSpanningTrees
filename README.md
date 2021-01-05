# LinkCutSpanningTrees 0.1.0-alpha

Implementation of algorithms for generating uniform spanning trees. This
package contains three algorithms. A simple random walk
([Broder],[Aldous]), Wilson's algorithm ([Wilson]) and the edge swap Markov
chain implemented with link-cut trees ([paper],[arxiv]). The main goal of
these prototypes is to compare the performance of the respective
algorithms. Hence they are provided with a simple cli interface.

## Table of contents

- [Getting Started]
   - [Prerequisites]
   - [Installing]
   - [Running]
- [Contributing]
- [Versioning]
- [Authors]
- [License]
- [Acknowledgments]

## Getting Started

To get a copy of this software download or clone the GitHub repository.

Download:

```
wget https://github.com/LuisRusso-INESC-ID/LinkCutSpanningTrees/archive/master.zip
```

Clone:

```
git clone git@github.com:LuisRusso-INESC-ID/LinkCutSpanningTrees.git
```

### Prerequisites

This package was tested with [Debian], it will most likely work with other
Linux distributions and UNIX variants. Some version of the following
components must exist in the system.

For Linux:

* C compiler, [gcc] or [clang]
* [GNU Make]
* bsd/stdlib, for arc4random
* glibc

### Installing

First check that you have the bsd/stdlib library is installed. In [Debian]
this can be obtained by running:

```
apt-get install libbsd0 libbsd-dev
```

Once this is installed you can check if everything is ok by compiling the
project. Simply execute

```
make
```

If all went well your build system is working. This should produce three
binaries randomWalk, Wilson and edgeSwap.

### Running

If you execute one of the binaries without arguments it outputs the
respective synopsis. For example when you execute the `./edgeSwap`
procedure it should produce the following:

```
SYNOPSIS

./edgeSwap <input> <output> [<repetitions> [<extra>]]

DESCRIPTION

To use this program specify a file name as <input>. This file
should contain a correct graph description. The <output> string is
used to store the resulting uniformly sampled spanning trees. The
<repetitions> value is optional and it indicates how many trees
should be generated. The default value is 1. The resulting trees
are stored in the output file, as lists of edges and are
separated by blank lines.

The <extra> parameter is also optional and can only be used if the
<repetitions> argument is given. This parameter is used to increase
the number of steps that are executed. It is a floating point
value. The expression for the number of steps is m*(extra +
log(m)), where m is the number of edges in the graph and log is
the natural logarithm. The default is 0.0.

These spanning trees are generated using the edge swapping Markov chain.
```

This is actually the most complicated of the three binaries. The simplest
invocation is the following:

```
./edgeSwap K5 trees
```

This should produce a file named `trees` that contains the description of a
random tree. Here is a sample output, which can be obtained with `cat trees`

```
2 4
3 5
4 1
5 1

```

This file represent one tree. Each line of the output represents an edge,
in no particular order. In this example node 2 is connected to node 4; node
3 is connected to node 5, etc. The file `K5` is the complete graph with 5
nodes. This is a sample graph provided in the repo. Let us take a look at
the graph representation `cat K5`

```
5 10
1 2
1 3
1 4
1 5
2 3
2 4
2 5
3 4
3 5
4 5
```

The first line gives general graph parameters. The first number is the
number of vertexes and second number the number of edges. All the numbers
in the file must be integers. In this example the graph has 5 vertexes and
10 edges. Each one of the next 10 lines contains an edge description, which
is a pair of vertexes in any order, separated by a space. The vertexes are
numbered starting at `1`.

We can use the repetition argument to obtain more than one tree. For
example the following command generates 4 trees:

```
./edgeSwap K5 trees 4
```

All the trees are stored in the `trees` file, each tree is separated from
the next by an empty line.

This particular prototype contains an extra argument. This argument is used
to specify extra steps that the chain should execute. In our [paper] we
described how to implement this chain using link-cut trees. We also studied
the mixing time for cycle graphs and gave complexity bounds for the time
performance of the algorithm based on the mixing time of the
chain. Recently the mixing time of the chain was proven to be
O(m\*log(m/e)), for a graph with m edges and a desired (1/e) variational
distance to uniform, see [CGM] and [AGV]. Hence by default the algorithm
executes m\*log(m) steps, where log is the natural logarithm. Extra steps
may be indicated with the extra parameter. For example to execute
m\*(2.3+log(m)) steps we can use the following command:

```
./edgeSwap K5 trees 1 2.3
```

The value of the extra steps parameter is a floating point number. It is
possible to use negative extra steps, thus decreasing the total amount of
steps.

## Contributing

If you found this project useful please share it, also you can create an
[issue] with comments and suggestions.

## Versioning

We use [SemVer] for versioning. For the versions available, see the [tags]
on this repository.

## Authors

* **Luís M. S. Russo** - *Initial work* - [LuisRusso]

See also the list of [contributors] who participated in this project.

## License

This project is licensed under the BSD 2-Clause "Simplified" License - see
the [LICENSE file] for details

## Acknowledgments

* This software was produced for research that was funded in part by
European Union’s Horizon 2020 research and innovation programme under the
Marie Skłodowska-Curie grant agreement No 690941 and by national funds
through Fundação para a Ciência e Tecnologia ([FCT]) with reference
UID/CEC/50021/2013.
* Thanks to [PurpleBooth] for the [README-Template].
* The [grip] tool by [Joe Esposito] was very handy for producing this file.


[Getting Started]: #getting-started
[Prerequisites]: #prerequisites
[Installing]: #installing
[Running]: #running
[Contributing]: #contributing
[Versioning]: #versioning
[Authors]: #authors
[License]: #license
[Acknowledgments]: #acknowledgments

[arxiv]: https://arxiv.org/abs/1801.06846
[paper]: https://doi.org/10.3390/a11040053
[Wilson]: https://dl.acm.org/doi/10.1145/237814.237880
[CGM]: https://arxiv.org/abs/1903.06081
[AGV]: https://arxiv.org/abs/2004.07220
[FCT]: https://www.fct.pt/
[Broder]: https://www.cs.cmu.edu/~15859n/RelatedWork/Broder-GenRanSpanningTrees.pdf
[Aldous]: https://doi.org/10.1137/0403039

[Debian]: https://www.debian.org/
[gcc]: https://gcc.gnu.org/
[clang]: https://clang.llvm.org/
[GNU Make]: https://www.gnu.org/software/make/
[Make]: https://www.freebsd.org/doc/en/books/developers-handbook/tools-make.html
[issue]: ../../issues
[lmsrusso@gmail.com]: mailto:lmsrusso@gmail.com
[SemVer]: http://semver.org/
[tags]: ../../tags
[LuisRusso]: https://github.com/LuisRusso-INESC-ID
[contributors]: ../../contributors
[LICENSE file]: ./LICENSE
[PurpleBooth]: https://gist.github.com/PurpleBooth
[README-Template]: https://gist.github.com/PurpleBooth/109311bb0361f32d87a2
[grip]: https://github.com/joeyespo/grip
[Joe Esposito]: https://github.com/joeyespo
