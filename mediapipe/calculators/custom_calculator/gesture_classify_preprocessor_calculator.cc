// Copyright 2019 The MediaPipe Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include <iostream>
#include "mediapipe/calculators/tensor/tensors_to_landmarks_calculator.pb.h"
#include "mediapipe/framework/api2/node.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/landmark.pb.h"
#include "mediapipe/framework/formats/tensor.h"
#include "mediapipe/framework/port/ret_check.h"

namespace mediapipe {
namespace api2 {

namespace {

// inline float Sigmoid(float value) { return 1.0f / (1.0f + std::exp(-value)); }

// float ApplyActivation(
//     ::mediapipe::GestureClassifyPreprocessorCalculatorOptions::Activation activation,
//     float value) {
//   switch (activation) {
//     case ::mediapipe::GestureClassifyPreprocessorCalculatorOptions::SIGMOID:
//       return Sigmoid(value);
//       break;
//     default:
//       return value;
//   }
// }

}  // namespace

// A calculator for converting Tensors from regression models into landmarks.
// Note that if the landmarks in the tensor has more than 5 dimensions, only the
// first 5 dimensions will be converted to [x,y,z, visibility, presence]. The
// latter two fields may also stay unset if such attributes are not supported in
// the model.
//
// Input:
//  TENSORS - Vector of Tensors of type kFloat32. Only the first tensor will be
//  used. The size of the values must be (num_dimension x num_landmarks).
//
// Output:
//  LANDMARKS(optional) - Result MediaPipe landmarks.
//  NORM_LANDMARKS(optional) - Result MediaPipe normalized landmarks.
//
// Notes:
//   To output normalized landmarks, user must provide the original input image
//   size to the model using calculator option input_image_width and
//   input_image_height.
// Usage example:
// node {
//   calculator: "GestureClassifyPreprocessorCalculator"
//   input_stream: "TENSORS:landmark_tensors"
//   output_stream: "TENSORS_NORM:normalized_flattend_tensors"
// }
class GestureClassifyPreprocessorCalculator : public Node {
 public:
  static constexpr Input<std::vector<Tensor>> kInTensors{"TENSORS"};
  static constexpr Output<std::vector<Tensor>> kOutTensors{"TENSORS_NORM"};
  MEDIAPIPE_NODE_CONTRACT(kInTensors, kOutTensors);

  absl::Status Open(CalculatorContext* cc) override;
  absl::Status Process(CalculatorContext* cc) override;

 private:
  absl::Status LoadOptions(CalculatorContext* cc);
  int num_landmarks_ = 0;
  ::mediapipe::TensorsToLandmarksCalculatorOptions options_;
};
MEDIAPIPE_REGISTER_NODE(GestureClassifyPreprocessorCalculator);

absl::Status GestureClassifyPreprocessorCalculator::Open(CalculatorContext* cc) {
  MP_RETURN_IF_ERROR(LoadOptions(cc));

  return absl::OkStatus();
}

absl::Status GestureClassifyPreprocessorCalculator::Process(CalculatorContext* cc) {
  if (kInTensors(cc).IsEmpty()) {
    return absl::OkStatus();
  }

  const auto& input_tensors = *kInTensors(cc);
  // std::cout << input_tensors.size() << std::endl; // size of input vector
  // std::cout << input_tensors[0].shape().dims.size() << std::endl; // num dimesions in tensor
  std::cout << input_tensors[0].shape().dims.data()[0] << std::endl; // data in 0th dimension of tensor
  std::cout << input_tensors[0].shape().dims.data()[1] << std::endl; // data in 1st dimension of tensor

  int num_values = input_tensors[0].shape().num_elements();
  const int num_dimensions = num_values / num_landmarks_;

  // CHECK_GT(num_dimensions, 0);

  auto view = input_tensors[0].GetCpuReadView();
  auto raw_landmarks = view.buffer<float>();

  LandmarkList output_landmarks;
  float maxval = 0;
  float base_x = 0;
  float base_y = 0;
  float _x = 0;
  float _y = 0;
  float landmark_array[num_landmarks_*2] = {};
  // Tensor tensor(Tensor::ElementType::kFloat32, tensor_shape);
  Tensor tensor(Tensor::ElementType::kFloat32, {1, num_landmarks_*2});
  auto* buffer = tensor.GetCpuWriteView().buffer<float>();
  // std::cout << tensor.shape().dims.data()[0]<< "," << tensor.shape().dims.data()[1] << std::endl; // get shape of tensor at each dimension
  // std::cout << tensor.shape().dims.size() << std::endl; // get num dims of tensor

  for (int ld = 0; ld < num_landmarks_; ++ld) {
    const int offset = ld * num_dimensions;
    Landmark* landmark = output_landmarks.add_landmark();
    if (ld==0)
    {
      base_x = raw_landmarks[offset];
      base_y = raw_landmarks[offset+1];
    }
    _x = raw_landmarks[offset]-base_x;
    _y = raw_landmarks[offset + 1]-base_y;
    // std::cout << _x << std::endl;
    // std::cout << _y << std::endl;

    maxval = std::max(maxval, std::abs(_x));
    maxval = std::max(maxval, std::abs(_y));
    

    landmark_array[2*ld] = _x;
    landmark_array[(2*ld)+1] = _y;

  }

  for (int i = 0; i < num_landmarks_*2 ; ++i){
    buffer[i] = landmark_array[i]/maxval;
    // std::cout << buffer[i] << std::endl;

  }
  // std::cout << buffer.get_size() << std::endl;


  // std::cout << tensor.element_size()<< std::endl;
  // std::cout << num_landmarks_*2<< std::endl;

  // Return vector with a single tensor.
  auto result = std::vector<Tensor>();
  result.push_back(std::move(tensor));
  kOutTensors(cc).Send(std::move(result));
  

  return absl::OkStatus();
}

absl::Status GestureClassifyPreprocessorCalculator::LoadOptions(CalculatorContext* cc) {
  // Get calculator options specified in the graph.
  options_ = cc->Options<::mediapipe::TensorsToLandmarksCalculatorOptions>();
  RET_CHECK(options_.has_num_landmarks());
  num_landmarks_ = options_.num_landmarks();

  return absl::OkStatus();
}
}  // namespace api2
}  // namespace mediapipe
