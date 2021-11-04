#/bin/bash
echo $(date)
rm -f ./tf_sampling_so.so
echo "rming"

# TF1.2
TF_INC=$(python -c 'import tensorflow as tf; print(tf.sysconfig.get_include())')
TF_LIB=$(python -c 'import tensorflow as tf; print(tf.sysconfig.get_lib())')
g++ -std=c++11 tf_sampling_cpu.cpp tf_sampling_g_cpu.cpp -o tf_sampling_so.so -shared -fPIC -I $TF_INC -I /usr/local/lib/python2.7/dist-packages/tensorflow/include -L$TF_LIB -I$TF_INC/external/nsync/public -ltensorflow_framework -O2 -D_GLIBCXX_USE_CXX11_ABI=0

# TF1.4
#g++ -std=c++11 tf_sampling.cpp tf_sampling_g.cu.o -o tf_sampling_so.so -shared -fPIC -I /usr/local/lib/python2.7/dist-packages/tensorflow/include -I /usr/local/cuda-8.0/include -I /usr/local/lib/python2.7/dist-packages/tensorflow/include/external/nsync/public -lcudart -L /usr/local/cuda-8.0/lib64/ -L/usr/local/lib/python2.7/dist-packages/tensorflow -ltensorflow_framework -O2 -D_GLIBCXX_USE_CXX11_ABI=0
