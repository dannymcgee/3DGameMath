### 3.3.1 Dual Perspectives

Two useful ways of imagining coordinate space transformations

* **Active transformation paradigm**: Fix our perspective with the coordinate space, and the vectors and objects move around as their coordinates change.
* **Passive transformation paradigm**: Fix out perspective relative to the thing being transformed, making it appear as if we are transforming the coordinate space used to measure the coordinates.

Transforming an object has the same effect on the coordinates as performing the _opposite_ transformation to the coordinate space itself.

### 3.3.2 Specifying Coordinate Spaces

We specify a coordinate space by describing its **origin** and **axes**.

* The **origin** is a point that defines the position of the space.
* The **axes** are vectors that describe the orientation of the space (and possibly other information, such as scale).

The coordinates we use to measure the origin and axes must be relative to some _other_ coordinate space.

### 3.3.3 Basis Vectors

> **w** = **o** + _b<sub>x</sub>_**p** + _b<sub>y</sub>_**q** + _b<sub>z</sub>**r**_
>
> where:
> * **b** = some arbitrary point whose body-space coordinates are known
> * **w** = the world-space coordinates of that same point
> * **o** = the world-space coordinates for the body origin
> * **p**, **q**, **r**  = the basis vectors (representing, e.g., the up, right, and forward directions of the body's coordinate space)

The coordinates of basis vectors are measured in terms of a reference frame that is different from the one for which they are a basis. Thus basis vectors are intimately linked with coordinate space transformations.

The numeric coordinates of a vector with respect to a given basis are the coefficients in the expansion of that vector as a linear combination of the basis vectors. For example:

> **v** = _v<sub>x</sub>_**p** + _v<sub>y</sub>_**q** + _v<sub>z</sub>_**r**

When the basis vectors are **orthogonal** (mutually perpendicular), the coordinates are uncoupled. Any given coordinate of a vector **v** can be determined solely from **v** and the corresponding basis vector. For example, we can compute _v<sub>x</sub>_ knowing only **p**, provided that the other basis vectors are perpendicular to **p**.

In an **orthonormal** basis (all basis vectors are unit-length), each coordinate of a vector **v** is the signed displacement **v** measured in the direction of the corresponding basis vector. This can be computed directly by taking the dot product of **v** with that basis vector.

Thus, in the special circumstance of an orthonormal basis, we have a simple way to determine the body-space coordinates, knowing only the world coordinates of the body axes:

> _b<sub>x</sub>_ = **u** &middot; **p** <br>
> _b<sub>y</sub>_ = **u** &middot; **q** <br>
> _b<sub>z</sub>_ = **u** &middot; **r**

Assuming a coordinate space is not mirrored:

> **p** &times; **q** = **r**
