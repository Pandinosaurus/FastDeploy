// Copyright (c) 2022 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "fastdeploy/vision/common/processors/transform.h"
#include "fastdeploy/fastdeploy_model.h"
#include "fastdeploy/vision/common/result.h"
#include "fastdeploy/vision/tracking/pptracking/tracker.h"
//#include "fastdeploy/vision/tracking/pptracking/letter_box.h"

namespace fastdeploy {
namespace vision {
namespace tracking {

class FASTDEPLOY_DECL PPTracking: public FastDeployModel {

public:
  /** \brief Set path of model file and configuration file, and the configuration of runtime
   *
   * \param[in] model_file Path of model file, e.g pptracking/model.pdmodel
   * \param[in] params_file Path of parameter file, e.g pptracking/model.pdiparams, if the model format is ONNX, this parameter will be ignored
   * \param[in] config_file Path of configuration file for deployment, e.g pptracking/infer_cfg.yml
   * \param[in] custom_option RuntimeOption for inference, the default will use cpu, and choose the backend defined in `valid_cpu_backends`
   * \param[in] model_format Model format of the loaded model, default is Paddle format
   */
  PPTracking(const std::string& model_file,
             const std::string& params_file,
             const std::string& config_file,
             const RuntimeOption& custom_option = RuntimeOption(),
             const ModelFormat& model_format = ModelFormat::PADDLE);

  /// Get model's name
  std::string ModelName() const override { return "pptracking"; }

  /** \brief Predict the detection result for an input image(consecutive)
   *
   * \param[in] im The input image data which is consecutive frame, comes from imread() or videoCapture.read()
   * \param[in] result The output tracking result will be writen to this structure
   * \return true if the prediction successed, otherwise false
   */
  virtual bool Predict(cv::Mat* img, MOTResult* result);


private:

  bool BuildPreprocessPipelineFromConfig();

  bool Initialize();

  bool Preprocess(Mat* img, std::vector<FDTensor>* outputs);

  bool Postprocess(std::vector<FDTensor>& infer_result, MOTResult *result);

  std::vector<std::shared_ptr<Processor>> processors_;
  std::string config_file_;
  float draw_threshold_;
  float conf_thresh_;
  float tracked_thresh_;
  float min_box_area_;
  std::unique_ptr<JDETracker> jdeTracker_;

};

} // namespace tracking
} // namespace vision
} // namespace fastdeploy
