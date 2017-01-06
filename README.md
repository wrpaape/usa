#usa

##Overview
`usa` approximates a solution to a classic traveling salesman problem -- what is the shortest path that visits each capital of the fifty United States?
`bin/generate_distances`, when run, translates raw coordinates from `data/coordinates.tsv` to a labeled file of all possible distances between any two capitals, `data/distances.tsv`.
This file provides the main program, `bin/usa`, with the data to construct an adjacency matrix graph on which to perform the analysis.
Once a finite number of iterations have passed, or the user interrupts with `CTRL-c`, the resulting step-by-step path and total distance is pretty-printed to `data/solution.txt`.

##Implementation
Rather than attempt a futile exhaustive search for the absolute minimum-distance path within the `50!` solution space, `usa` utilizes [simulated annealing](https://en.wikipedia.org/wiki/Simulated_annealing), a probablistic technique, to gradually improve on a working solution with incremental adjustments.
Once a graph structure representing an initial path with an initial total distance has been setup, the procedure follows:
```
for (an arbitrary finite number of iterations OR interrupt) {
        randomly sample two path nodes;

        examine the effect of swapping the two nodes on total distance;

        if (   (post-swap distance < pre-swap distance)
            OR (biased_coin_flip() == TAILS))
                swap_nodes();

        slightly adjust coin bias towards HEADS;
}
```
The point of the coin flip is to keep the solution state dislodged from potentially misleading [local optimum](https://en.wikipedia.org/wiki/Hill_climbing) early on.
