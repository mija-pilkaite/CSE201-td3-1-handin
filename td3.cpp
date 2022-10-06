#include <iostream>
#include "td3.hpp"
#include "support.hpp"
#include <stdlib.h>
#include <math.h>       // sin, cos
#include <assert.h>

using namespace std;

using namespace support;

double* extend_array(double* array, int length, int new_size) {
  double* array1 = new double[new_size];
  for (int i=0; i<length; i++){
      array1[i] = array[i];
  }
  for (int j=length; j<new_size; j++){
      array1[j] = 0;
  }
  delete[] array;
  return array1;
}

double* shrink_array(double* array, int length, int new_size) {
  double* array1 = new double[new_size];
  for (int i=0; i<new_size; i++){
      array1[i] = array[i];
  }
  delete[] array;
  return array1;
}

double* append_to_array(double element,
                        double* array,
                        int &current_size,
                        int &max_size) {
  if (current_size>=max_size){
      max_size+=5;
      array = extend_array(array, max_size, max_size+5);
  }
  array[current_size] = element;
  current_size+=1;
  return array;

}

double* remove_from_array(double* array,
                          int &current_size,
                          int &max_size) {

  current_size--;
  if (max_size-current_size>=5){
      array = shrink_array(array, max_size, max_size-5);
      max_size-=5;
  }

  return array;

}

bool simulate_projectile(const double magnitude, const double angle,
                         const double simulation_interval,
                         double *targets, int &tot_targets,
                         int *obstacles, int tot_obstacles,
                         double* &telemetry,
                         int &telemetry_current_size,
                         int &telemetry_max_size) {
  // YOU CAN MODIFY THIS FUNCTION TO RECORD THE TELEMETRY

  bool hit_target, hit_obstacle;
  double v0_x, v0_y, x, y, t;
  double PI = 3.14159265;
  double g = 9.8;

  v0_x = magnitude * cos(angle * PI / 180);
  v0_y = magnitude * sin(angle * PI / 180);

  t = 0;
  x = 0;
  y = 0;


  hit_target = false;
  hit_obstacle = false;
  while (y >= 0 && (! hit_target) && (! hit_obstacle)) {
    telemetry = append_to_array(t, telemetry, telemetry_current_size, telemetry_max_size);
    telemetry = append_to_array(x, telemetry, telemetry_current_size, telemetry_max_size);
    telemetry = append_to_array(y, telemetry, telemetry_current_size, telemetry_max_size);
    double * target_coordinates = find_collision(x, y, targets, tot_targets);
    if (target_coordinates != NULL) {
      remove_target(targets, tot_targets, target_coordinates);
      hit_target = true;
    } else if (find_collision(x, y, obstacles, tot_obstacles) != NULL) {
      hit_obstacle = true;
    } else {
      t = t + simulation_interval;
      y = v0_y * t  - 0.5 * g * t * t;
      x = v0_x * t;

    }
  }

  return hit_target;
}

void merge_telemetry(double **telemetries,
                     int tot_telemetries,
                     int *telemetries_sizes,
                     double* &global_telemetry,
                     int &global_telemetry_current_size,
                     int &global_telemetry_max_size) {
  double one = 0;
  double two = 0;
  double three = 0;
  for (int i=0; i<tot_telemetries; i++){
      for (int t=0; t<telemetries_sizes[i]; t++){
      global_telemetry = append_to_array(telemetries[i][t], global_telemetry, global_telemetry_current_size, global_telemetry_max_size);
   }
  }
  std::cout<<"printing"<<std::endl;
  for (int j=0; j<global_telemetry_current_size-3; j+=3){
      for (int k=j+3; k<global_telemetry_current_size; k+=3){
          if (global_telemetry[k]<global_telemetry[j]){
              one = global_telemetry[j];
              two = global_telemetry[j+1];
              three = global_telemetry[j+2];
              global_telemetry[j] = global_telemetry[k];
              global_telemetry[j+1] = global_telemetry[k+1];
              global_telemetry[j+2] = global_telemetry[k+2];
              global_telemetry[k] = one;
              global_telemetry[k+1] = two;
              global_telemetry[k+2] = three;
          }
      }

  }
}
