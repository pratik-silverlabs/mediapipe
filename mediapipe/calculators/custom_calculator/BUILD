# Copyright 2019 The MediaPipe Authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

load("//mediapipe/framework/port:build_config.bzl", "mediapipe_proto_library")

licenses(["notice"])

package(default_visibility = ["//visibility:public"])

cc_library(
 name = "gesture_classify_preprocessor_calculator",
 srcs = ["gesture_classify_preprocessor_calculator.cc"],
 visibility = ["//visibility:public"],
 deps=[
     "//mediapipe/calculators/tensor:tensors_to_landmarks_calculator_cc_proto",
     "//mediapipe/framework/api2:node",
     "//mediapipe/framework:calculator_framework",
     "//mediapipe/framework/formats:landmark_cc_proto",
     "//mediapipe/framework/formats:tensor",
 ],
 alwayslink = 1,
)

cc_library(
    name = "pass_through_calculator",
    srcs = ["pass_through_calculator.cc"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/calculators/core:gate_calculator_cc_proto",
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework/port:ret_check",
        "//mediapipe/framework/port:status",
        "//mediapipe/util:header_util",
    ],
    alwayslink = 1,
)