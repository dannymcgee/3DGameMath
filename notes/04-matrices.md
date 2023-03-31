### 4.1.7 Multiplying a Vector and a Matrix

```
          | m11  m12  m13 |
[ x y z ] | m21  m22  m23 |  ==  [ xm11 + ym21 + zm31   xm12 + ym22 + zm32   xm13 + ym23 + zm33 ]
          | m31  m32  m33 |
```
```
| m11  m12  m13 | | x |      | xm11 + ym12 + zm13 |
| m21  m22  m23 | | y |  ==  | xm21 + ym22 + zm23 |
| m31  m32  m33 | | z |      | xm31 + ym32 + zm33 |
```
Interesting observations:

1. Each element in the resulting vector is the dot product of the original vector with a single row or column from the matrix.

2. Each element in the matrix determines how much "weight" a particular element in the input vector contributes to an element in the output vector. (E.g., in the first example, _m_<sub>12</sub> controls how much of input _x_ goes toward output _y_.)

3. Vector &times; matrix multiplication distributes over vector addition:
	> (**v** + **w**)**M** == **vM** + **wM**

4. The result of the multiplication is a linear combination of the rows or columns of the matrix. E.g., in the first example, the resulting vector can be interpreted as a linear combination of the rows of the matrix, where the coefficients come from the vector operand:
	> let **a** = [ _m_<sub>11</sub> _m_<sub>12</sub> _m_<sub>13</sub> ]<br>
	> let **b** = [ _m_<sub>21</sub> _m_<sub>22</sub> _m_<sub>23</sub> ]<br>
	> let **c** = [ _m_<sub>31</sub> _m_<sub>32</sub> _m_<sub>33</sub> ]
	>
	> **result** = _v<sub>x</sub>_**a** + _v<sub>y</sub>_**b** + _v<sub>z</sub>_**c**

### 4.2 Geometric Interpretation of a Matrix

What is the relationship between a particular 3D transformation (e.g., rotation) and the nine values inside a 3&times;3 matrix? How can we construct a matrix to perform a given transform?

Watch what happens when the standard basis vectors are multiplied by an arbitrary matrix **M**:

> **iM** = [ 1 0 0 ]**M** = [ _m_<sub>11</sub> _m_<sub>12</sub> _m_<sub>13</sub> ];<br>
> **jM** = [ 0 1 0 ]**M** = [ _m_<sub>21</sub> _m_<sub>22</sub> _m_<sub>23</sub> ];<br>
> **kM** = [ 0 0 1 ]**M** = [ _m_<sub>31</sub> _m_<sub>32</sub> _m_<sub>33</sub> ];

Once we know what happens to the basis vectors, we know everything about the transformation! This is because any vector can be written as a linear combination of the standard basis, as:

> **v** = _v<sub>x</sub>_**i** + _v<sub>y</sub>_**j** + _v<sub>z</sub>_**k**

Multiplying this expression by our matrix on the right, we have:

> **vM** = (_v<sub>x</sub>_**i** + _v<sub>y</sub>_**j** + _v<sub>z</sub>_**k**)**M**<br>
> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;= (_v<sub>x</sub>_**i**)**M** + (_v<sub>y</sub>_**j**)**M** + (_v<sub>z</sub>_**k**)**M**<br>
> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;= _v<sub>x</sub>_(**iM**) + _v<sub>y</sub>_(**jM**) + _v<sub>z</sub>_(**kM**)<br>
> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;= _v<sub>x</sub>_[ _m_<sub>11</sub> _m_<sub>12</sub> _m_<sub>13</sub> ] + _v<sub>y</sub>_[ _m_<sub>21</sub> _m_<sub>22</sub> _m_<sub>23</sub> ] + _v<sub>z</sub>_[ _m_<sub>31</sub> _m_<sub>32</sub> _m_<sub>33</sub> ]

By understanding how the matrix transforms the standard basis vectors, we know everything there is to know about the transformation. Since the results of transforming the standard basis are simply the rows of the matrix, **we interpret those rows as the basis vectors of a coordinate space**.

#### Key Concepts in Review
* The rows of a square matrix can be interpreted as the basis vectors of a coordinate space.
* To transform a vector from the original coordinate space to the new coordinate space, we multiply the vector by the matrix.
* The transformation from the original coordinate space to the coordinate space defined by these basis vectors is a linear transformation. A linear transformation preserves straight lines, and parallel lines remain parallel. However, angles, lengths, areas, and volumes may be altered after transformation.
* Multiplying the zero vector by any square matrix results in the zero vector. Therefore the linear transformation represented by a square matrix has the same origin as the original coordinate space &mdash; the transformation does not contain translation.
* We can visualize a matrix by visualizing the basis vectors of the coordinate space after transformation. These basis vectors form an 'L' in 2D, and a tripod in 3D. Using a box or auxiliary object also helps in visualization.

### 4.3 The Bigger Picture of Linear Algebra

One excellent resource for learning the bigger picture of linear algebra and scientific computing is Professor Gilbert Strang's series of lectures, which can be downloaded free from MIT OpenCourseWare at [ocw.mit.edu](https://ocw.mit.edu). He offers a basic undergraduate linear algebra course as well as graduate courses on computational science and engineering. The companion textbooks he writes for his classes <sup>1</sup> <sup>2</sup> are enjoyable books aimed at engineers (rather than math sticklers) and are recommended, but be warned that his writing style is a sort of shorthand that you might have trouble understanding without the lectures.

<hr>

<sup>1</sup> Gilbert Strang. _Introduction to Linear Algebra_, Fourth edition. Cambridge, UK: Wellesley-Cambridge, 2009.

<sup>2</sup> Gilbert Strang. _Computational Science and Engineering_. Cambridge, UK: Wellesley-Cambridge, 2007.
