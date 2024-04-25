# Complex Fractal

Final Project for Moravian University CSCI 392: High Performance Computing.

## Program

Each version of the complex fractal generator has its own dependencies.
The serial version should compile on all systems that support complex arithmetic.

The shared version requires a compiler with [OpenMP](https://www.openmp.org/) support.

### Building

To build all versions simply run

```bash
make
```

If you wish to compile with additional floating point precision, add `-DEXTENDED_PRECISION` to `CPPFLAGS` in the makefile.

**NOTE:** extended precision is **NOT** supported in the cuda version.

### Running

Any version of the program can be used to generate a visualizer compatible `.grid` file.
All versions of the program support the same command line arguments
The performance flag outputs information in the format of `<PROGRAM>,<FRACTAL>,<RUNTIME>`.
Note that the runtime is an average runtime from multiple runs.
The number of runs can be adjusted directly in `src/fractals.c` in `NUM_RUNS` or passed set in `CPPFLAGS` by adding `-DNUM_RUNS=N`

```
Usage: PROGRAM [-v] [-i iterations] [-x x_res] [-y y_res] [-z magnification] [-l lower_left] [-u upper_right] [-o output_grid] -f fractal
Options:
  -i, --iterations <value>        the number of iterations (default: 100)
  -x, --x-res <value>             the horizontal resolution (default: terminal width)
  -y, --y-res <value>             the vertical resolution (default: terminal height)
  -l, --lower-left <value>        Set the lower left corner of the fractal area (default: -2-2i)
  -u, --upper-right <value>       Set the upper right corner of the fractal area (default: 2+2i)
  -z, --magnification <value>     Set the magnification factor (default: 1)
  -o, --output <filename>         the output filename (default: fractal.grid)
  -f, --fractal <type>            the fractal type (default: mandelbrot)
      supported fractals: mandelbrot, tricorn, multibrot, multicorn, burning_ship, julia
  -p, --performance               print performance info
  -v, --verbose                   verbose output
  -h, --help                      prints this help message
```

## Visualizations

## Presentation

Building the presentation requires a [pandoc](https://pandoc.org/) installation.

To build the presentation run

```bash
make presentation
```

## Resources

* [LibreText on The Mandelbrot Set](https://math.libretexts.org/Bookshelves/Analysis/Complex_Analysis_-_A_Visual_and_Interactive_Introduction_(Ponce_Campuzano)/05%3A_Chapter_5/5.05%3A_The_Mandelbrot_Set)
* [LibreText on Julia Sets](https://math.libretexts.org/Bookshelves/Analysis/Complex_Analysis_-_A_Visual_and_Interactive_Introduction_(Ponce_Campuzano)/05%3A_Chapter_5/5.06%3A_The_Julia_Set)
* [Various Algorithms for Plotting the Mandelbrot Set](https://en.wikipedia.org/wiki/Plotting_algorithms_for_the_Mandelbrot_set)
* [Pseudocode for Julia Set](https://en.wikipedia.org/wiki/Julia_set#Pseudocode)
* [Another HPC implemenation of Mandelbrot set rendering](https://github.com/OakenKnight/MandelbrotSet-HightPerformance-Implementation)
* [Blog Post on CUDA Mandelbrot Set](https://developershell.net/the-mandelbrot-set-with-cuda/)

## Contributors

* JP Appel
