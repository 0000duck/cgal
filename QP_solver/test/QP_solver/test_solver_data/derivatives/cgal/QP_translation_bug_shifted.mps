* Number-type: integer
* Description: Shifted instance of original file
* Generated-by: master_mps_to_derivatives
NAME QPE_solver_example_bug
ROWS
  N obj
  G c0
  G c1
COLUMNS
  x0  obj  -64
  x0  c0  -4
  x0  c1  0
  x1  obj  33
  x1  c0  2
  x1  c1  1
  x2  obj  -8
  x2  c0  0
  x2  c1  1
RHS
  rhs c0  -8
  rhs c1  7
BOUNDS
  LO  BND  x0  1
  LO  BND  x1  2
  LO  BND  x2  3
QMATRIX
  x0  x0  128
  x0  x1  -32
  x0  x2  0
  x1  x0  -32
  x1  x1  8
  x1  x2  4
  x2  x0  0
  x2  x1  -4
  x2  x2  0
ENDATA
