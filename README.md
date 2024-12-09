# HyperLTL-Models

To compile run 
```
make
```
The only requirement is C++ 17+.

Run 
```
./ltl-conversion --help
``` 
to get an overview of options. 
Currently only the congestion model for TAPAAL is fully implemented.
Generating a Query requires specifying an output path and can not be done over `stdout`.
