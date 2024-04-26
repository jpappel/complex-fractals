# Final Presentation Notes

## Outline

* definition of complex numbers
* definition of mandelbrot set
    * examples of fractals (Romanesco, Sripenski triangle, Menger sponge)
* question: how do we handle recursion in computer science
    * we avoid it!
* Mention how when Mandelbrot initially tried to have his computed image printed, the printers kept removing the "dust" thinking it was a misprint
* cpus support greater precision than gpus
    * "precision.h"
* linking cuda object code with c code
    * c++ mangles names, functions that are to be linked against must be declared extern
* serial algorithm for computing Mandelbrot set
    * runtime
    * animation where the number of frames are dependent on runtime
* shared algorithm for computing mandelbrot set
    * runtime
    * unique challenges of the shared version
    * benefits gained from the shared version
    * animation where the number of frames are dependent on runtime
* gpu algorithm for computing mandelbrot set
    * runtime
    * unique challenges of the gpu version
    * benefits gained from the gpu version
    * animation where the number of frames are dependent on runtime
* overall comparison of implementations
