#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gmath.h"
#include "matrix.h"
#include "ml6.h"

//lighting functions
color get_lighting( double *normal, double *view, color alight, double light[2][3], double *areflect, double *dreflect, double *sreflect) {
  color i;
  return i;
}

color calculate_ambient(color alight, double *areflect ) {
  color a;
  a->red = alight->red[0];
  a->green = alight->green[1];
  a->blue = alight->blue[2];

  limit_color(a);
  return a;
}

// P * Kd * (N̂ • L̂)
color calculate_diffuse(double light[2][3], double *dreflect, double *normal ) {
  color light_color;
  light_color -> red = light[COLOR][RED];
  light_color -> green = light[COLOR][GREEN];
  light_color -> blue = light[COLOR][BLUE];

  double* light_location;
  light_location[0] = light[LOCATION][0];
  light_location[1] = light[LOCATION][1];
  light_location[2] = light[LOCATION][2];

  color d;
  d->red = light_color->red * dreflect * dot_product(normal, light_location);
  d->green = light_color->green * dreflect * dot_product(normal, light_location);
  d->red = light_color->blue * dreflect * dot_product(normal, light_location);
  return d;
}

// PKs[(2(Ñ●Ĺ)Ñ-Ĺ)●V]^x
color calculate_specular(double light[2][3], double *sreflect, double *view, double *normal ) {
  color light_color;
  light_color -> red = light[COLOR][RED];
  light_color -> green = light[COLOR][GREEN];
  light_color -> blue = light[COLOR][BLUE];

  double* light_location;
  light_location[0] = light[LOCATION][0];
  light_location[1] = light[LOCATION][1];
  light_location[2] = light[LOCATION][2];

  // 2(Ñ●Ĺ)Ñ-Ĺ
  double* big;
  big[0] = 2*dot_product(normal, light_location) * normal[0] - light_location[0];
  big[1] = 2*dot_product(normal, light_location) * normal[1] - light_location[1];
  big[2] = 2*dot_product(normal, light_location) * normal[2] - light_location[2];

  color s;
  s->red = light_color->red * sreflect * pow(dot_product(big, view), 13);
  s->green = light_color->green * sreflect * pow(dot_product(big, view), 13);
  s->blue = light_color->blue * sreflect * pow(dot_product(big, view), 13);
  return s;
}


//limit each component of c to a max of 255
void limit_color( color * c ) {
  if (c->red >255){
    c->red = 255;
  }
  else if (c->red < 0){
    c->red = 0;
  }
  if (c->green >255){
    c->green = 255;
  }
  else if (c->green < 0){
    c->green = 0;
  }
  if (c->blue >255){
    c->blue = 255;
  }
  else if (c->blue < 0){
    c->blue = 0;
  }
}

//vector functions
//normalize vetor, should modify the parameter
void normalize( double *vector ) {
  magnitude = sqrt(pow(vector[0], 2), pow(vector[1], 2), pow(vector[2], 2));
  vector[0] = vector[0] / magnitude;
  vector[1] = vector[0] / magnitude;
  vector[2] = vector[0] / magnitude;
}

//Return the dot porduct of a . b
double dot_product( double *a, double *b ) {
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

double *calculate_normal(struct matrix *polygons, int i) {

  double A[3];
  double B[3];
  double *N = (double *)malloc(3 * sizeof(double));

  A[0] = polygons->m[0][i+1] - polygons->m[0][i];
  A[1] = polygons->m[1][i+1] - polygons->m[1][i];
  A[2] = polygons->m[2][i+1] - polygons->m[2][i];

  B[0] = polygons->m[0][i+2] - polygons->m[0][i];
  B[1] = polygons->m[1][i+2] - polygons->m[1][i];
  B[2] = polygons->m[2][i+2] - polygons->m[2][i];

  N[0] = A[1] * B[2] - A[2] * B[1];
  N[1] = A[2] * B[0] - A[0] * B[2];
  N[2] = A[0] * B[1] - A[1] * B[0];

  return N;
}
