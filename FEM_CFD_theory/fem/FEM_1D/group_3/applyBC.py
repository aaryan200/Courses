import numpy as np

def apply_bc(K_global, F_global, prescribed_dofs):
  """
  Modify global stiffness matrix and force vector based on
  prescribed boundary conditions.
  """
  K_t = K_global.copy()
  F_t = F_global.copy()
  for dof in prescribed_dofs:
    node, dir, val = map(int, dof[:3])
    temp = (node - 1) * 2 + (dir - 1)
    K_t[temp, :] = 0.0
    K_t[:, temp] = 0.0
    K_t[temp, temp] = 1.0
    F_t[temp] = 0.0
  return K_t, F_t