#pragma once
#include "types.h"
#include <cfloat>
#include <immintrin.h>

// Returns hit t values or 0 depending on if this ray hit this sphere or not
[[nodiscard]] __m256 sphere_hit(const RayCluster* rays, float t_max, Sphere sphere);

void create_hit_record(HitRecords* hit_rec, const RayCluster* rays, __m256 t_vals,
                       SphereCluster* sphere_cluster);

/*
  UPDATING SPHERES
  1. create mask where less condition is met
  2. mask new sphere vals to ignore where t's were higher than previous
  3. negate the mask to find where the less condition wasn't met
  4. AND the existing spheres with it to only preserve where it isn't less
  5. add the two masked registers together to form updated sphere cluster
 */
// updates a sphere cluster with a sphere given a mask of where to insert the new sphere's values
void update_sphere_cluster(SphereCluster* curr_cluster, Sphere curr_sphere, __m256 update_mask);

void find_sphere_hits(SphereCluster* sphere_cluster, HitRecords* hit_rec, const Sphere* spheres,
                      const RayCluster* rays, float t_max);
