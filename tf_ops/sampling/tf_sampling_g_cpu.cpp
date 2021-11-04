#include <iostream>
#include <cstring>
#include <algorithm>

void gatherPoint(int b, int n, int m, const float * inp, const int * idx, float * out) {
  // std::cout << "tf_sampling_g_cpu gatherPoint\n";
  for (int batchIdx=0; batchIdx < b; batchIdx++) {
    for (int j=0; j<m; j++) {
      int pointIndex = idx[batchIdx * m + j];

      int inputPointIndex = (batchIdx * n + pointIndex) * 3;
      int outputPointIndex = (batchIdx * m + j) * 3;

      out[outputPointIndex + 0] = inp[inputPointIndex + 0];
      out[outputPointIndex + 1] = inp[inputPointIndex + 1];
      out[outputPointIndex + 2] = inp[inputPointIndex + 2];
    }
  }
  // std::cout << "tf_sampling_g_cpu gatherPoint ends\n";
}

float calculateEuclideanDistance(float x1, float y1, float z1, float x2, float y2, float z2) {
  return (x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)+(z2-z1)*(z2-z1);
}

void fpsWithSingleThread(int b, int n, int m, const float * dataset, int * out) {
  // std::cout <<"tf_sampling_g_cpu fpsWithSingleThread\n";
  // std::cout<<b;
  // std::cout<<" ";
  // std::cout<<n;
  // std::cout<<" ";
  // std::cout<<m;
  // std::cout<<"\n";
  
  for (int batchIdx=0; batchIdx < b; batchIdx++) {
    // actually no need to use temp, it's too large, only usable in multiple threads processing

    // Last farthest point
    const float* temp_dataset = dataset + (batchIdx * n) * 3;
    int lfp_i = 0;
    float* temp = new float[n];

    for(int i=0; i<n; i++) {
      temp[i] = 99999999;
    }
    
    for (int j=0; j<m; j++) {
      float x1 = temp_dataset[lfp_i * 3 + 0];
      float y1 = temp_dataset[lfp_i * 3+ 1];
      float z1 = temp_dataset[lfp_i * 3+ 2];

      float farthestDis = -1;   // best
      int indexOfBestPoint = 0; // best_i

      for (int dataIndexInEachSample = 0; dataIndexInEachSample < n; dataIndexInEachSample++) {
        float x2 = temp_dataset[dataIndexInEachSample * 3 + 0];
        float y2 = temp_dataset[dataIndexInEachSample * 3 + 1];
        float z2 = temp_dataset[dataIndexInEachSample * 3 + 2];

        float dis = calculateEuclideanDistance(x1, y1, z1, x2, y2, z2);

        float oldCloestDistance = temp[dataIndexInEachSample];
        float d2 = std::min(dis, oldCloestDistance);

        if (d2 != oldCloestDistance) {
          // update the cloest distance
          temp[dataIndexInEachSample] = d2;
        }

        if (d2 > farthestDis) {
          farthestDis = d2;
          indexOfBestPoint = dataIndexInEachSample;
        }
      }

      // no longer need merge sort
      lfp_i = indexOfBestPoint;
      out[batchIdx * m + j] = lfp_i;
    }
    // for (int j=0; j<m; j++) {
    //   std::cout<<out[batchIdx * m + j];
    //   std::cout<<" ";
    // }
    // std::cout<<"\n";
  }

  // std::cout<< "tf_sampling_g_cpu gatherPoint ends\n";
}

// b: batch size
// n: dataset in each batch
// m: number of points to be sampled in this fps execution
// inp: input data
// temp: temp tensor?
// out: output tensor
void farthest_points_sampling(int b, int n, int m, const float * inp, int * out) {
  if (m<=0) {
    return;
  }
  fpsWithSingleThread(b, n, m, inp, out);
}

// functions to be impletmented
void cumsumLauncher(int b,int n,const float * inp,float * out){
}
//require b*n working space
void probsampleLauncher(int b,int n,int m,const float * inp_p,const float * inp_r,float * temp,int * out){
}

void gatherpointLauncher(int b, int n, int m, const float * inp, const int * idx, float * out){
  gatherPoint(b, n, m, inp, idx, out);
}
void scatteraddpointLauncher(int b,int n,int m,const float * out_g,const int * idx,float * inp_g){
}
//require 32*n working space
void farthestpointsamplingLauncher(int b,int n,int m,const float * inp,int * out){
  farthest_points_sampling(b, n, m, inp, out);
}
