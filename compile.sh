#!/bin/bash
/bin/bash -c "cd /workspace/pointnet2/tf_ops/3d_interpolation && sh tf_interpolate_compile.sh"
/bin/bash -c "cd /workspace/pointnet2/tf_ops/grouping && pwd && sh tf_grouping_compile.sh"
/bin/bash -c "cd /workspace/pointnet2/tf_ops/sampling && sh tf_sampling_compile.sh"