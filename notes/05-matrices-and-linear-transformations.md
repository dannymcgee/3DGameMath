### 5.7.7 Summary of Types of Transformations

Transform  | Linear  | Affine  | Invertible | Angles Preserved | Orthogonal | Rigid Body | Lengths Preserved | Areas/Volumes Preserved | Determinant
-----------|---------|---------|------------|------------------|------------|------------|-------------------|-------------------------|-------------
Linear     | &check; | &check; |            |                  |            |            |                   |                         |
Affine     |         | &check; |            |                  |            |            |                   |                         | &ne; 0
Invertible |         |         | &check;    |                  |            |            |                   |                         |
Angle-Preserving |   | &check; | &check;    | &check;          |            |            |                   |                         |
Orthogonal |         | &check; | &check;    |                  | &check;    |            |                   |                         | &plusmn;1
Rigid Body |         | &check; | &check;    | &check;          | &check;    | &check;    | &check;           | &check;                 | 1
Translation|         | &check; | &check;    | &check;          | &check;    | &check;    | &check;           | &check;                 | 1
Rotation   | &check; | &check; | &check;    | &check;          | &check;    | &check;    | &check;           | &check;                 | 1
Uniform Scale | &check; | &check; | &check; | &check;          |            |            |                   |                         | _k<sup>n</sup>_ 3
Non-Uniform Scale | &check; | &check; | &check; |              |            |            |                   |                         |
Ortho Projection | &check; | &check; |      |                  |            |            |                   |                         | 0
Reflection | &check; | &check; | &check;    |                  | &check;    |            | *                 | &check;                 | -1
Shearing   | &check; | &check; | &check;    |                  |            |            |                   | &check;                 | 1
