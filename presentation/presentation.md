---
title: "HPC Complex Fractal Generation"
author:
- "JP Appel"
- "David Marrero"
---

# Prerequisite Knowledge

## Complex Numbers

$$i^2 = -1$$
$$z = x + iy$$

::: notes

* complex numbers are an extension of real numbers, stemming from the square root of $-1$
* a complex number is just a pair of two real numbers (x,y) with different ways to add and multiply
* in computer science we model real numbers with a single float or double, so we will need 2 floats or doubles to model a complex number

:::

. . .

### Addition

$$z_1 + z_2 = (x_1 + x_2) + i (y_1 + y_2)$$

::: notes

* addition behaves as you expect
* multiplication involves multiplying then distributing, and using the fact that $i^2 = -1$
* so adding two complex numbers is 2 float additions
* and multiplying them is 4 multiplications and 2 additions

:::

### Multiplication

$$z_1z_2 = (x_1x_2 - y_1y_2) + i(x_1y_2 + x_2y_1)$$

## What is the Mandelbrot Set

. . .

$$z_n = z^2_{n-1} + z_0$$


::: notes


* this sequence is used to generate the mandelbrot set
* if for some complex number $z_0$ the sequence remains bounded as it $n$ approaches infinity then $z_0$ lies within the mandelbrot set
* there are many recursive sequences related to this, where you modify that happens with the recursive term

:::

## Fractals

::::: {.columns}

:::: {.column width=40%}
* infinite self-similar geometric shape
* have "fractional dimension"
::::

:::: {.column width=60%}
![Sripenski Triangle](https://upload.wikimedia.org/wikipedia/commons/thumb/4/45/Sierpinski_triangle.svg/1920px-Sierpinski_triangle.svg.png){ width=80% }
::::
:::::

. . .

The Mandelbrot set is a fractal in the complex plane

::: notes

* fractals are a infinite self-similar geometric shape
* so if you zoom in on any one part it will look like the entire object
* the Sripenski triangle is an example of a fractal
* the mandelbrot set forms a fractal in the complex plane

:::

## Fractal in Nature

![Romanesco Cauliflower](https://www.rocketgardens.co.uk/wp-content/uploads/2016/02/Cauliflower20Romanesco.jpg){ width=50%}

::: notes

* fractals often show up in nature

:::


## Escape Time Algorithm

::: incremental

* Inputs
    * Maximum Number of iterations
    * Upper bound
1. Create a grid of points to sample
2. For each point in the sample space
    1. Compute the next term in the sequence
    2. if greater than the upper bound return the number of iterations
    3. else repeat until the maximum number of iterations and return

:::

::: notes

* to compute the complex sets we used an escape time algorithm
* in general it takes in a maximum number of iterations, an initial value, and an upper bound
* the escape time algorithm is as follows (read of the slides)
* note that each sampled point is completely independent of any other point
    * this hints to us that the problem will parallelize well
* for most of the sets we looked at, there is a proven bound
    * ie if the sequence is ever larger than a value we know it diverges
:::

# Implementation

## Program Structure

![](diagram.png)

::: notes

* the translation units are roughly as pictured here
* black ellipses are for serial code, color is for parallel
* each version of the program fills in an array with the number of iterations it took the sequence to grow too large
* that array along with some extra data is a grid object, which can be serialized and deserialized
* the main fractals unit handles cli argument parsing for the sampling resolution, fractal type etc
* a separate renderer program handles creating images from the `.grid` file
* the `.grid` file format is really simple, it's a magic number, the grid dimensions, the maximum number of iterations, the lower left and upper right most points of the region, and then the data

:::

## Mandelbrot

[Image](../examples/mandelbrot.png)

::: notes

* when mandelbrot initially tried to have this printed, the printers kept removing the "dust" thinking it was an error in their printing process

:::

## Tricorn

[Image](../examples/tricorn.png)

## Burning Ship

[Image](../examples/burning_ship.png)

## Multibrot

[Image](../examples/multibrot.png)

## Multicorn

[GIF](../examples/multicorn.gif)


## Julia

[Image](../examples/julia.png)

# Analysis

##

[Interactive Plots](../analysis/analysis.html)
