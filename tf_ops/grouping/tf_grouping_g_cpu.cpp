#include <iostream>
#include <cstring>
#include <algorithm>


void queryBallPoint(int b, int n, int m, float radius, int nsample, const float *xyz1, const float *xyz2, int *idx, int *pts_cnt) {
    // std::cout <<"tf_grouping_g_cpu queryBallPoint\n";

    for (int batchIdx=0; batchIdx < b; batchIdx++) {
        // move to the correspond data part in raw data points array
        const float* temp_xyz1 = xyz1 + n * 3 * batchIdx;
        const float* temp_xyz2 = xyz2 + m * 3 * batchIdx;
        int* temp_idx = idx + m * nsample * batchIdx;
        int* temp_pts_cnt = pts_cnt + m * batchIdx;

        for (int i=0; i<m; i++) { // for each centroid, query the ball
            int pointNum = 0;
            for (int j=0; j<n; j++) {
                if (pointNum == nsample) {
                    break;
                }
                // centroid
                float x1=temp_xyz2[i * 3 + 0];
                float y1=temp_xyz2[i * 3 + 1];
                float z1=temp_xyz2[i * 3 + 2];
                
                // point to be checked
                float x2=temp_xyz1[j * 3 + 0];
                float y2=temp_xyz1[j * 3 + 1];
                float z2=temp_xyz1[j * 3 + 2];

                float distance = std::max(sqrtf((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)+(z2-z1)*(z2-z1)),1e-20f);

                if (distance < radius) {
                    if (pointNum == 0) {
                        // initialize the idx with first j
                        for (int k=0; k<nsample; k++) {
                            temp_idx[i * nsample + k] = j;
                        }
                    }
                    temp_idx[i * nsample + pointNum] = j;
                    pointNum += 1;
                }
            }
            temp_pts_cnt[i] = pointNum;
        }
    }
    // int biggest = 0;
    // for (int i =0;i<b * m * nsample; i++) {
    //     if (biggest < idx[i]) {
    //         biggest =  idx[i];
    //     }
    // }
    // std::cout<<"biggest id:";
    // std::cout<<biggest;
    // std::cout<<"\n";
}

// input: points (b,n,c), idx (b,m,nsample)
// output: out (b,m,nsample,c)
void groupPoint(int b, int n, int c, int m, int nsample, const float *points, const int *idx, float *out) {
    // std::cout <<"tf_grouping_g_cpu groupPoint\n";
    // std::cout<<b;
    // std::cout<<" ";
    // std::cout<<n;
    // std::cout<<" ";
    // std::cout<<c;
    // std::cout<<" ";
    // std::cout<<m;
    // std::cout<<" ";
    // std::cout<<nsample;
    // std::cout<<"\n";
    
    
    for (int batchIdx=0; batchIdx < b; batchIdx++) {
        const float* temp_points = points + batchIdx * n * c;
        const int* temp_idx = idx + batchIdx * m * nsample;
        float* temp_out = out + batchIdx * m * nsample * c;

        // int biggest = 0;
        // for (int i=0; i<m; i++) {
        //     for (int j=0; j < nsample; j++) {
        //         // std::cout<<temp_idx[i * nsample + j];
        //         // std::cout<<" ";
        //         if (temp_idx[i * nsample + j] > biggest) {
        //             biggest = temp_idx[i * nsample + j];
        //         }
        //     }
        // }
        // std::cout<<biggest;
        // std::cout<<"\n";

        // std::cout<<"start to group\n";
        for (int i=0; i<m; ++i) { // for each centroid, do the grouping
            for (int j=0; j < nsample; ++j) {
                int ii = temp_idx[i * nsample + j];
                if (ii > n) {
                    std::cout<<"Find illegal ii";
                    std::cout<<ii;
                    std::cout<<"\n";
                    ii = n - 1;
                }
                for (int l=0; l<c; ++l) {
                    temp_out[i * nsample * c + j * c + l] = temp_points[ii * c + l];
                }
            }
        }
    }
}

// functions to be impletmented
// pts_cnt -> in each centroid point, how many points is included in its ball
// xyz1 -> row data point (batch_size, number_of_data, 3)
// xyz2 -> sampled points in each data batch
// nsample -> how many points in each local region
// idx -> for each local region, the index of the points in the ball
void queryBallPointLauncher(int b, int n, int m, float radius, int nsample, const float *xyz1, const float *xyz2, int *idx, int *pts_cnt) {
    queryBallPoint(b,n,m,radius,nsample,xyz1,xyz2,idx,pts_cnt);
    //cudaDeviceSynchronize();
}
void selectionSortLauncher(int b, int n, int m, int k, const float *dist, int *outi, float *out) {
}
// b -> batch size
// n -> points number in each data sample
// c -> channels: 3 dimensions + feature vection dimensions
// m -> numbers of centroid
// nsample -> numbers of points in each local region
// points -> faltted raw data (b, n, c)
// idx -> index of the points in each local region in each data sample (b, m, nsample)
// output: out (b,m,nsample,c)
void groupPointLauncher(int b, int n, int c, int m, int nsample, const float *points, const int *idx, float *out){
    // std::cout<<"start";
    groupPoint(b,n,c,m,nsample,points,idx,out);
}
void groupPointGradLauncher(int b, int n, int c, int m, int nsample, const float *grad_out, const int *idx, float *grad_points){
}