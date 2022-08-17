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

#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/classification.pb.h"
#include "mediapipe/framework/port/ret_check.h"
#include "mediapipe/framework/port/status.h"
#include "mediapipe/framework/port/statusor.h"
#include "mediapipe/framework/api2/node.h"


namespace mediapipe {
    namespace api2 {

        namespace{

        }

            class ClassificationListCustomCalculator : public Node {
                public:
                static constexpr Input<ClassificationList> kInList{"CLASSIFICATIONS"};
                static constexpr Output<std::vector<std::string>> kOutLabel{"LABELS"};
                MEDIAPIPE_NODE_CONTRACT(kInList, kOutLabel);

                absl::Status Open(CalculatorContext* cc) override;
                absl::Status Process(CalculatorContext* cc) override;
            };

            MEDIAPIPE_REGISTER_NODE(ClassificationListCustomCalculator);

            absl::Status ClassificationListCustomCalculator::Open(CalculatorContext* cc) {
                return absl::OkStatus();
            }

            absl::Status ClassificationListCustomCalculator::Process(CalculatorContext* cc) {

                const auto& classifications = *kInList(cc);
                std::vector<std::string> labels;
                std::vector<float> scores;

                labels.resize(classifications.classification_size());
                scores.resize(classifications.classification_size());
                for (int i = 0; i < classifications.classification_size(); ++i) {
                    labels[i] = classifications.classification(i).label();
                    std::cout << classifications.classification(i).label() << std::endl;
                    scores[i] = classifications.classification(i).score();
                    // std::cout << classifications.classification(i).score() << std::endl;
                } 

                kOutLabel(cc).Send(std::move(labels));
                return absl::OkStatus();
            }



    }


}  // namespace mediapipe
