#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void grid(int nx, double xst, double xen, double *x, double *dx)
{
  int i;
  
  *dx = (xen-xst)/(double)(nx-1);

  for(i=0; i<nx; i++)
    x[i] = (double)i * (*dx); // ensure x[0] == 0.0 and x[nx-1] == 1.0
}

void enforce_bcs(int nx, double *x, double *T)
{
  T[0] = -1.0;
  T[nx-1] = 1.0;
}

void set_initial_condition(int nx, double *x, double *T)
{
  int i;

  for(i=0; i<nx; i++)
    T[i] = tanh((x[i]-0.5)/0.05);

  enforce_bcs(nx,x,T); //ensure BCs are satisfied at t = 0
}

void get_rhs(int nx, double dx, double *x, double *T, double *rhs)
{
  int i;
  double dxsq = dx*dx;

  // compute rhs. For this problem, d2T/dx2
  for(i=1; i<nx-1; i++) {
    if (i==1) {
      rhs[i] = (10.0*T[i-1]-4.0*T[i+1]+14.0*T[i+2]-6.0*T[i+3]+T[i+4]-15.0*T[i])/(12.0*dxsq);
    }
    else if (i==nx-2) {
      rhs[i] = (10.0*T[i+1]-4.0*T[i-1]+14.0*T[i-2]-6.0*T[i-3]+T[i-4]-15.0*T[i])/(12.0*dxsq);
    }
    else {
      rhs[i] = (4.0*(T[i+1]+T[i-1])/3.0 - (T[i+2]+T[i-2])/12.0 - 5.0*T[i]/2.0)/dxsq;
    }
  }
}

void timestep_Euler(int nx, double dt, double dx, double *x, double *T, double *rhs)
{

  int i;

  // compute rhs
  get_rhs(nx,dx,x,T,rhs);

  // (Forward) Euler scheme
  for(i=0; i<nx; i++)
    T[i] = T[i] + dt*rhs[i];  // T^(it+1)[i] = T^(it)[i] + dt * rhs[i];

  // set Dirichlet BCs
  enforce_bcs(nx,x,T);

}

double get_l2err_norm(int nx, double *arr1, double *arr2)
{
    double l2err = 0.0, val;
    int i;

    for (i = 0; i < nx; i++)
      {
          val = arr1[i] - arr2[i];
          l2err += val * val;
      }
    l2err = l2err / ((double)(nx));
    l2err = sqrt(l2err);

    return l2err;
}

void get_exact_soln(int nx, double *x, double *Tex, double t)
{
    int i;

    for (i = 0; i < nx; i++) {
      Tex[i] = erf((x[i]-0.5)/(2.0*sqrt(t)));
    }
}

void output_soln(int nx, int it, double tcurr, double *x, double *T, double* Tex)
{
  int i;
  FILE* fp;
  char fname[100];

  sprintf(fname, "T_x_%04d.dat", it);
  // printf("\n%s\n", fname);

  fp = fopen(fname, "w");
  for(i=0; i<nx; i++)
    fprintf(fp, "%lf %lf %lf\n", x[i], T[i], Tex[i]);
  fclose(fp);

  printf("Done writing solution for time step = %d\n", it);
}

int main()
{

  int nx;
  double *x, *T, *Tex, *rhs, tst, ten, xst, xen, dx, dt, tcurr;
  int i, it, num_time_steps, it_print;
  FILE* fp;  

  // read inputs
  fp = fopen("input.in", "r");
  fscanf(fp, "%d\n", &nx);
  fscanf(fp, "%lf %lf\n", &xst, &xen);
  fscanf(fp, "%lf %lf\n", &tst, &ten);
  fclose(fp);

  printf("Inputs are: %d %lf %lf %lf %lf\n", nx, xst, xen, tst, ten);

  x = (double *)malloc(nx*sizeof(double));
  T = (double *)malloc(nx*sizeof(double));
  Tex = (double *)malloc(nx*sizeof(double));
  rhs = (double *)malloc(nx*sizeof(double));
  

  grid(nx,xst,xen,x,&dx);         // initialize the grid

  set_initial_condition(nx,x,T);  // initial condition
  get_exact_soln(nx, x, Tex, ten);

  // prepare for time loop
  dt = 3.125e-6;                               // Ensure r satisfies the stability condition
  num_time_steps = (int)((ten-tst)/dt) + 1; // Since difference results in timesteps between tst and ten (including ten), we also have to include tst, that's why we add 1
  it_print = num_time_steps/10;             // write out approximately 10 intermediate results

  // start time stepping loop
  for(it=0; it<num_time_steps; it++)
  {
    tcurr = tst + (double)it * dt;

    timestep_Euler(nx,dt,dx,x,T,rhs);    // update T

    // output soln every it_print time steps
    if(it%it_print==0)
      output_soln(nx,it,tcurr,x,T, Tex);
  }
  double l2err = get_l2err_norm(nx, T, Tex);
  printf("For dt = %.9lf, nx = %d, l2err = %lf\n", dt, nx, l2err);
  // printf("%lf\n", l2err);

  free(rhs);
  free(T);
  free(Tex);
  free(x);

  return 0;
}

