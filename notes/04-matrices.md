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

3. Vector * matrix multiplication distributes over vector addition:
	> (**v** + **w**)**M** == **vM** + **wM**

4. The result of the multiplication is a linear combination of the rows or columns of the matrix. E.g., in the first example, the resulting vector can be interpreted as a linear combination of the rows of the matrix, where the coefficients come from the vector operand:
	> let **a** = [ _m_<sub>11</sub> _m_<sub>12</sub> _m_<sub>13</sub> ]<br>
	> let **b** = [ _m_<sub>21</sub> _m_<sub>22</sub> _m_<sub>23</sub> ]<br>
	> let **c** = [ _m_<sub>31</sub> _m_<sub>32</sub> _m_<sub>33</sub> ]
	>
	> **result** = _v<sub>x</sub>_**a** + _v<sub>y</sub>_**b** + _v<sub>z</sub>_**c**
