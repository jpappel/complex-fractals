# Complex Fractals

Final Project for Moravian University CSCI 392: High Performance Computing.

## Program

Each version of the complex fractal generator has its own dependencies.
The serial version should compile on all systems that support complex arithmetic.

The shared version requires a compiler with [OpenMP](https://www.openmp.org/) support.

The image plotting program works with [ImageMagick](https://www.openmp.org/) 7+. Earlier versions may work but are not supported.

### Building

To build all versions simply run

```bash
make
```

### Running

## Animations

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
