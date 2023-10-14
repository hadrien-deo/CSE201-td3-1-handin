#include <iostream>
#include "td3.hpp"
#include "support.hpp"
#include <stdlib.h>
#include <math.h>       // sin, cos
#include <assert.h>

using namespace std;

using namespace support;

double* extend_array(double* array, int length, int new_size) {
  // IMPLEMENT YOUR FUNCTION HERE
    double* a;
    a=new double[new_size];
    for (int i=0; i<length; i++) {
        a[i]=array[i];
    }
    for (int i=length; i<new_size; i++) {
        a[i]=0;
    }
    /*for (int i=0; i<length; i++) {
        delete array[i];
    }*/
    delete array;
    return a;
}

double* shrink_array(double* array, int length, int new_size) {
  // IMPLEMENT YOUR FUNCTION HERE
    double* a;
    a=new double[new_size];
    for (int i=0; i<new_size; i++) {
        a[i]=array[i];
    }
    delete array;
    return a;
}

double* append_to_array(double element,
                        double* array,
                        int &current_size,
                        int &max_size) {
  // IMPLEMENT YOUR FUNCTION HERE
    if (current_size==max_size) {
        array=extend_array(array,current_size,current_size+5);
        max_size=max_size+5;
    }
    array[current_size]=element;
    current_size=current_size+1;

    return array;
}

double* remove_from_array(double* array,
                          int &current_size,
                          int &max_size) {
  // IMPLEMENT YOUR FUNCTION HERE
    current_size=current_size-1;
    if (max_size-current_size>=5) {
        array=shrink_array(array,max_size,max_size-5);
        max_size=max_size-5;
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
    telemetry=append_to_array(t, telemetry, telemetry_current_size,telemetry_max_size);
    telemetry=append_to_array(x,telemetry,telemetry_current_size,telemetry_max_size);
    telemetry=append_to_array(y,telemetry,telemetry_current_size,telemetry_max_size);

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


void sort(double *targets, const int num_targets) {
  // IMPLEMENT YOUR CODE HERE
  double x;
  for (int i=3; i<num_targets*3;i=i+3) {

    for (int a=i;a>0;a=a-3) {

      if (*(targets+a)<*(targets+a-3)) {
          x=*(targets+a);
          *(targets+a)=*(targets+a-3);
          *(targets+a-3)=x;
          x=*(targets+a+1);
          *(targets+a+1)=*(targets+a-2);
          *(targets+a-2)=x;
          x=*(targets+a+2);
          *(targets+a+2)=*(targets+a-1);
          *(targets+a-1)=x;

      }
    }
  }
}

void merge_telemetry(double **telemetries,
                     int tot_telemetries,
                     int *telemetries_sizes,
                     double* &global_telemetry,
                     int &global_telemetry_current_size,
                     int &global_telemetry_max_size) {
  // IMPLEMENT YOUR FUNCTION HERE


  for (int i=0; i<tot_telemetries; i++) {
    for (int j=0; j<telemetries_sizes[i];j++) {
      global_telemetry=append_to_array(telemetries[i][j], global_telemetry, global_telemetry_current_size, global_telemetry_max_size);
    }
  }


  int p;
  p=0;
  for (int k=0;k<tot_telemetries;k++) {
    p=p+(telemetries_sizes[k]);
  }
  p=p/3;

  sort(global_telemetry,p);





  /*
  //start 0
  //double a[0];
  //global_telemetry=a;
  global_telemetry_current_size=0;
  global_telemetry_max_size=0;
  double t;
  int minindex;
  bool truth[tot_telemetries];
  for (int i=0; i<tot_telemetries; i++) {
    truth[i]=true;
  }
  int index[tot_telemetries];
  for (int i=0; i<tot_telemetries; i++) {
    index[i]=0;
  }
  int p;
  p=0;
  for (int k=0;k<tot_telemetries;k++) {
    p=p+(telemetries_sizes[k]);
  }
  p=p/3;
  //end 0: initialisation of used variables


  int m;
  m=1;
  for (int j=0; j<m; j++) {
    m=p;
    //start 3
    bool b;
    b=false;

    for (int i=0;i<tot_telemetries;i++) {
      if (truth[i]==true) {
          b=true;
          break;
      }
    }
    if (b==false) {
      return;
    } else {


      //start 1
      t=telemetries[0][index[0]];
      minindex=0;
      for (int i=1; i<tot_telemetries;i++) {
          if (index[i]<*(telemetries_sizes+i)) {
              if (t>telemetries[i][index[i]]) {
                  minindex=i;
                  t=telemetries[i][index[i]];
              }
          } else {
              truth[i]=0;
              //return;
          }
      }
      global_telemetry=append_to_array(t, global_telemetry, global_telemetry_current_size, global_telemetry_max_size);
      global_telemetry=append_to_array(telemetries[minindex][index[minindex]+1], global_telemetry, global_telemetry_current_size, global_telemetry_max_size);
      global_telemetry=append_to_array(telemetries[minindex][index[minindex]+2], global_telemetry, global_telemetry_current_size, global_telemetry_max_size);
      index[minindex]=index[minindex]+3;
      //end 1: appends the triplet with the lower t of all telemetries


    }
    //end 3: verify if the code is finished

  }


*/


}
