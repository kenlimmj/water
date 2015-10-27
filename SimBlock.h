#ifndef SIMBLOCK_H
#define SIMBLOCK_H

#include <cstdio>
#include <cmath>
#include <cassert>
#include <vector>

#include "central2dBaseClass.h"
#include "shallow2d.h"
#include "minmod.h"
#include "meshio.h"

typedef Central2D<Shallow2D, MinMod<Shallow2D::real>> Sim;
typedef typename Shallow2D::real real;
typedef typename Shallow2D::vec  vec;

class SimBlock : public Sim
{
    public:
      SimBlock(real w, real h, int nx, int ny, real cfl) : Sim(w, h, nx, ny, cfl) {}
      friend class BlockedSimulation;

    private:
      __declspec(target(mic)) static constexpr int nghost = 3;

      // Remove the original apply_periodic by replacing it with empty function
      void apply_periodic(){}

      // Define new copy operations for ghost cells, this got tedious
      __declspec(target(mic)) void copy_ghosts_from_left (const SimBlock&);
      __declspec(target(mic)) void copy_ghosts_from_topleft (const SimBlock&);
      __declspec(target(mic)) void copy_ghosts_from_bottomleft (const SimBlock&);
      __declspec(target(mic)) void copy_ghosts_from_right (const SimBlock&);
      __declspec(target(mic)) void copy_ghosts_from_topright (const SimBlock&);
      __declspec(target(mic)) void copy_ghosts_from_bottomright (const SimBlock&);
      __declspec(target(mic)) void copy_ghosts_from_top (const SimBlock&);
      __declspec(target(mic)) void copy_ghosts_from_bot (const SimBlock&);
};

void SimBlock::copy_ghosts_from_left(const SimBlock& source) {
  for (int i = nghost; i < ny_all-nghost; i++) {
    for (int j = 0; j < nghost; j++) {
      u(i, j) = source(i-nghost, nx-nghost+j);
    }
  }
}

void SimBlock::copy_ghosts_from_right(const SimBlock& source) {
  for (int i = nghost; i < ny_all-nghost; i++) {
    for (int j = 0; j < nghost; j++) {
      u(i, nx+nghost+j) = source(i-nghost, j);
    }
  }
}

void SimBlock::copy_ghosts_from_top(const SimBlock& source) {
  for (int i = 0; i < nghost; i++) {
    for (int j = nghost; j < nx_all-nghost; j++) {
      u(i, j) = source(ny-nghost+i,j-nghost);
    }
  }
}

void SimBlock::copy_ghosts_from_bot(const SimBlock& source) {
  for (int i = 0; i < nghost; i++) {
    for (int j = nghost; j < nx_all-nghost; j++) {
      u(ny+nghost+i, j) = source(i,j-nghost);
    }
  }
}

void SimBlock::copy_ghosts_from_topleft(const SimBlock& source) {
  for (int i = 0; i < nghost; i++) {
    for (int j = 0; j < nghost; j++) {
      u(i, j) = source(ny-nghost+i, nx-nghost+j);
    }
  }
}

void SimBlock::copy_ghosts_from_bottomleft(const SimBlock& source) {
  for (int i = 0; i < nghost; i++) {
    for (int j = 0; j < nghost; j++) {
      u(ny+nghost+i, j) = source(i, nx-nghost+j);
    }
  }
}

void SimBlock::copy_ghosts_from_topright(const SimBlock& source) {
  for (int i = 0; i < nghost; i++) {
    for (int j = 0; j < nghost; j++) {
      u(i, nx+nghost+j) = source(ny-nghost+i, j);
    }
  }
}

void SimBlock::copy_ghosts_from_bottomright(const SimBlock& source) {
  for (int i = 0; i < nghost; i++) {
    for (int j = 0; j < nghost; j++) {
      u(ny+nghost+i, nx+nghost+j) = source(i,j);
    }
  }
}

#endif /* SIMBLOCK_H*/
