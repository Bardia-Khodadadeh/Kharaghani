#ifndef DETECTOR_TORCHSCRIPT_YOLO_H
#define DETECTOR_TORCHSCRIPT_YOLO_H

#include "detector.h"
#include <torch/torch.h>
#include <torch/cuda.h>
#include <torch/script.h>

enum Det { tl_x = 0, tl_y = 1, br_x = 2, br_y = 3, score = 4, class_idx = 5 };
struct PadIno
{
    int top;
    int down;
    int left;
    int right;
    float scale;
};
class Detector_TorchScript_Yolo : public Detector
{
public:
    explicit Detector_TorchScript_Yolo(QObject *parent = nullptr);

    void loadModel(const QString modelPath) override;
    void setOutputSize(const cv::Size &newOutputSize) override;
    std::vector<std::vector<Detection>> run(std::vector<cv::Mat> &images, int cup) override;

private:

    cv::Size outputSize;
    torch::DeviceType device_type;
    torch::jit::script::Module module;
    std::vector<PadIno> LetterboxImage(std::vector<cv::cuda::GpuMat> &images, std::vector<cv::Size> &orgImagesSize);
    at::Tensor convertToTensor(std::vector<cv::cuda::GpuMat> &images);
    std::vector<std::vector<Detection>> PostProcessing(const at::Tensor &detections, std::vector<PadIno> pads_info, const std::vector<cv::Size> &orgImagesSize);
    float conf_threshold = 0.10;
    float iou_threshold = 0.5;
    at::Tensor xywh2xyxy(const at::Tensor &x);
    void Tensor2Detection(const at::TensorAccessor<float, 2>& offset_boxes, const at::TensorAccessor<float, 2>& det, std::vector<cv::Rect>& offset_box_vec, std::vector<float>& score_vec);
    void ScaleCoordinates(std::vector<Detection> &data, float pad_w, float pad_h, float scale, const cv::Size &img_shape);
    float calcIOU(Detection &trackedBox, Detection &detectedBox);

};

#endif // DETECTOR_TORCHSCRIPT_YOLO_H
