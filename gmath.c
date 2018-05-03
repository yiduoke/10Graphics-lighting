#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gmath.h"
#include "matrix.h"
#include "ml6.h"

//lighting functions
color get_lighting( double *normal, double *view, color alight, double light[2][3], double *areflect, double *dreflect, double *sreflect) {
  color i;
  normalize(normal);
  normalize(light[LOCATION]);
  normalize(view);
  i.red = calculate_ambient(alight, areflect).red + calculate_diffuse(light, dreflect, normal).red + calculate_specular(light, sreflect, view, normal).red;
  i.green = calculate_ambient(alight, areflect).green + calculate_diffuse(light, dreflect, normal).green + calculate_specular(light, sreflect, view, normal).green;
  i.blue = calculate_ambient(alight, areflect).blue + calculate_diffuse(light, dreflect, normal).blue + calculate_specular(light, sreflect, view, normal).blue;
  limit_color(&i);
  return i;
}

color calculate_ambient(color alight, double *areflect ) {
  color a;
  a.red = alight.red * areflect[RED];
  a.green = alight.green * areflect[GREEN];
  a.blue = alight.blue * areflect[BLUE];

  return a;
}

// P * Kd * (N̂ • L̂)
color calculate_diffuse(double light[2][3], double *dreflect, double *normal ) {
  color d;
  d.red = 0;
  d.green = 0;
  d.blue = 0;

  if (dot_product(light[LOCATION], normal) > 0){
    d.red = light[COLOR][RED] * dreflect[RED] * dot_product(normal, light[LOCATION]);
    d.green = light[COLOR][GREEN] * dreflect[GREEN] * dot_product(normal, light[LOCATION]);
    d.blue = light[COLOR][BLUE] * dreflect[BLUE] * dot_product(normal, light[LOCATION]);
  }

  return d;
}

// PKs[(2(Ñ●Ĺ)Ñ-Ĺ)●V]^x
color calculate_specular(double light[2][3], double *sreflect, double *view, double *normal ) {

  double* light_location;
  light_location = light[LOCATION];
  normalize(light_location);

  color s;
  s.red = 0;
  s.green = 0;
  s.blue = 0;

  if (dot_product(normal, light_location) > 0){
    // 2(Ñ●Ĺ)Ñ-Ĺ
    double big[3];
    big[0] = 2*dot_product(normal, light_location) * normal[0] - light_location[0];
    big[1] = 2*dot_product(normal, light_location) * normal[1] - light_location[1];
    big[2] = 2*dot_product(normal, light_location) * normal[2] - light_location[2];

    s.red = light[COLOR][RED] * sreflect[RED] * pow(dot_product(big, view), 13);
    s.green = light[COLOR][GREEN] * sreflect[GREEN] * pow(dot_product(big, view), 13);
    s.blue = light[COLOR][BLUE] * sreflect[BLUE] * pow(dot_product(big, view), 13);
  }
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
  float magnitude = sqrt(pow(vector[0], 2) + pow(vector[1], 2) + pow(vector[2], 2));
  vector[0] = vector[0] / magnitude;
  vector[1] = vector[1] / magnitude;
  vector[2] = vector[2] / magnitude;
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
