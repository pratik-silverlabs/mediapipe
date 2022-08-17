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

#include <math.h>

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include "absl/strings/str_cat.h"
#include "mediapipe/calculators/util/labels_to_render_data_calculator.pb.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/classification.pb.h"
#include "mediapipe/framework/formats/video_stream_header.h"
#include "mediapipe/framework/port/ret_check.h"
#include "mediapipe/framework/port/status.h"
#include "mediapipe/framework/port/statusor.h"
#include "mediapipe/util/color.pb.h"
#include "mediapipe/util/render_data.pb.h"

namespace mediapipe {

constexpr char kClassificationsTag[] = "CLASSIFICATIONS";


// A calculator takes in pairs of labels and scores or classifications, outputs
// generates render data. Either both "LABELS" and "SCORES" or "CLASSIFICATIONS"
// must be present.
//
// Usage example:
// node {
//   calculator: "ClassificationListCustomCalculator"
//   input_stream: "LABELS:labels"
//   input_stream: "SCORES:scores"
//   output_stream: "VIDEO_PRESTREAM:video_header"
//   options {
//     [ClassificationListCustomCalculatorOptions.ext] {
//       color { r: 255 g: 0 b: 0 }
//       color { r: 0 g: 255 b: 0 }
//       color { r: 0 g: 0 b: 255 }
//       thickness: 2.0
//       font_height_px: 20
//       max_num_labels: 3
//       font_face: 1
//       location: TOP_LEFT
//     }
//   }
// }
class ClassificationListCustomCalculator : public CalculatorBase {
 public:
  static absl::Status GetContract(CalculatorContract* cc);
  absl::Status Open(CalculatorContext* cc) override;
  absl::Status Process(CalculatorContext* cc) override;
};
REGISTER_CALCULATOR(ClassificationListCustomCalculator);

absl::Status ClassificationListCustomCalculator::GetContract(CalculatorContract* cc) {
//   if (cc->Inputs().HasTag(kClassificationsTag)) {
//     cc->Inputs().Tag(kClassificationsTag).Set<ClassificationList>();
//   } else {
//     RET_CHECK(cc->Inputs().HasTag(kLabelsTag))
//         << "Must provide input stream \"LABELS\"";
//     cc->Inputs().Tag(kLabelsTag).Set<std::vector<std::string>>();
//     if (cc->Inputs().HasTag(kScoresTag)) {
//       cc->Inputs().Tag(kScoresTag).Set<std::vector<float>>();
//     }
//   }
//   if (cc->Inputs().HasTag(kVideoPrestreamTag)) {
//     cc->Inputs().Tag(kVideoPrestreamTag).Set<VideoHeader>();
//   }
//   cc->Outputs().Tag(kRenderDataTag).Set<RenderData>();

  RET_CHECK(cc->Inputs().HasTag(kClassificationsTag)) << "Must provide input stream \"CLASSIFICATIONS\"";
  cc->Inputs().Tag(kClassificationsTag).Set<ClassificationList>();

  return absl::OkStatus();
}

absl::Status ClassificationListCustomCalculator::Open(CalculatorContext* cc) {
//   cc->SetOffset(TimestampDiff(0));
//   options_ = cc->Options<ClassificationListCustomCalculatorOptions>();
//   num_colors_ = options_.color_size();
//   label_height_px_ = std::ceil(options_.font_height_px() * kFontHeightScale);
  return absl::OkStatus();
}

absl::Status ClassificationListCustomCalculator::Process(CalculatorContext* cc) {

  std::vector<std::string> labels;
  std::vector<float> scores;
    const ClassificationList& classifications = cc->Inputs().Tag(kClassificationsTag).Get<ClassificationList>();
    labels.resize(classifications.classification_size());
    scores.resize(classifications.classification_size());
    for (int i = 0; i < classifications.classification_size(); ++i) {
        labels[i] = classifications.classification(i).label();
        std::cout << classifications.classification(i).label() << std::endl;
        scores[i] = classifications.classification(i).score();
        // std::cout << classifications.classification(i).score() << std::endl;

  } 

//   cc->Outputs()
//       .Tag(kRenderDataTag)
//       .AddPacket(MakePacket<RenderData>(render_data).At(cc->InputTimestamp()));

  return absl::OkStatus();
}
}  // namespace mediapipe
