# Complex Fractal

Final Project for Moravian University CSCI 392: High Performance Computing.

## Program

Each version of the complex fractal generator has its own dependencies.
The serial version should compile on all systems that support complex arithmetic.

The shared version requires a compiler with [OpenMP](https://www.openmp.org/) support.
The CUDA version requires the `nvcc` compiler and `thrust` libraries.
For better performance on your machine, change the flag `-arch=sm_86` in `NVCFLAGS` in the makefile to your gpu's compute capability.

### Building

To build all versions simply run

```bash
make
```

If you wish to compile with additional floating point precision, add `-DEXTENDED_PRECISION` to `CPPFLAGS` in the makefile.

**NOTE:** extended precision is **NOT** supported in the CUDA version.

### Running

Any version of the program can be used to generate a visualizer compatible `.grid` file.
All versions of the program support the same command line arguments.

The performance flag outputs information in the format of:
```
<PROGRAM>,<FRACTAL>,<DEGREE>,<CONSTANT_REAL>,<CONSTANT_IMAG>,<RADIUS>,<MAX_ITERATIONS>,<HORIZONTAL_SAMPLES>,<VERTICAL_SAMPLES>,<LOWER_REAL>,<LOWER_IMAG>,<UPPER_REAL>,<UPPER_IMAG>,<RUNTIME>
```

Note that the runtime is an average runtime from multiple runs.
The number of runs can be adjusted directly in `src/fractals.c` in `NUM_RUNS` or set in `CPPFLAGS` by adding `-DNUM_RUNS=N`.

```
Usage: <PROGRAM> [-v] [-i iterations] [-x x_res] [-y y_res] [-z magnification] [-d degree] [-c constant] [-r radius] [-l lower_left] [-u upper_right] [-o output_grid] -f fractal
Options:
  -i, --iterations <value>        the number of iterations (default: 100)
  -x, --x-res <value>             the horizontal resolution (default: terminal width)
  -y, --y-res <value>             the vertical resolution (default: terminal height)
  -l, --lower-left <value>        Set the lower left corner of the fractal area (default: -2-2i)
  -u, --upper-right <value>       Set the upper right corner of the fractal area (default: 2+2i)
  -z, --magnification <value>     Set the magnification factor (default: 1)
  -d, --degree <value>            Set the degree for fractals that use it (default: 2)
  -c, --constant <value>          Set the constant for fractals that use it (default: 0+0i)
  -r, --radius <value>            Set the radius for fractals that use it (default: 2)
  -o, --output <filename>         the output filename (default: fractal.grid)
  -f, --fractal <type>            the fractal type (default: mandelbrot)
      supported fractals: mandelbrot, tricorn, multibrot, multicorn, burning_ship, julia
  -p, --performance               print performance info
  -v, --verbose                   verbose output
  -h, --help                      prints this help message
```

#### Examples

`build/shared-fractals -x2000 -y2000 -z 2 -o burning_ship.grid -f burning_ship`

Generates a 2000x2000 burning ship fractal grid zoomed in 2x and saves it to burning_ship.grid

`build/serial-fractals -x500 -y500 -i35 -c 0.285+0.01i -r 20 -o julia.grid -f julia`

Generates a 500x500 julia fractal grid which has a maximum of 30 iterations for $c = 0.285 + 0.01i$ and a radius of 20 to julia.grid.

## Visualizations

The program `fractal-render` renders `.grid` files into txt, png's, and animated gifs.
The renderer program requires [LibGD](https://libgd.github.io/) to be installed.

```
Usage: fractal-render -i input.grid [-r renderer] [-o output.ext]
Options:
  -i, --input <input grid>        the grid to be rendered, if the file name is '-' reads from stdin
  -r, --renderer <renderer>       the renderer to use, defaults to the text renderer
      renderers:   txt, png, gif (TODO, with additional features)
  -d, --delay <delay>             the delay between animation frames in 1/100 s
  -o, --output <output file>      the file to output the result of rendering, if not given defaults to fractal.out.
  -v, --verbose                   verbose output
  -h, --help                      prints this help and exits
```

### Building

To build simply run

```bash
make
```

### Examples

Some example visualizations are provided in `examples`, they can be regenerated with

```bash
make examples/julia.png
make examples/multicorn.gif
```


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
