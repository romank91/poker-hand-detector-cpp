#include "HandDetector.hpp"
#include <fstream>
#include <iomanip>
#include <thread>
#include <random>
#include <cstdint>
#include <opencv2/opencv.hpp>

void HandDetector::loadClassesFromFile()
{
    const std::filesystem::path inputPath = config_->rootPath / std::filesystem::path("models/Classes.txt");
    std::ifstream inputFile(inputPath);

    if (inputFile.is_open())
    {
        std::string classLine;

        while (std::getline(inputFile, classLine))
        {
            classes_.push_back(classLine);
        }
            
        inputFile.close();
    }
    else
    {
        // TODO: handle invalid file 
        std::cout << "Could not open file\n";
    }
}

void HandDetector::loadOnnxNetwork()
{
    const std::filesystem::path modelPath = config_->rootPath / std::filesystem::path("app/playingCards.onnx");
    neuralNet_ = cv::dnn::readNetFromONNX(modelPath);

    if (config_->cuda_enabled)
    {
        std::cout << "Running on CUDA\n";
        neuralNet_.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
        neuralNet_.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA);
    }
    else
    {
        std::cout << "Running on CPU\n";
        neuralNet_.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
        neuralNet_.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
    }
}

cv::Mat HandDetector::resizeFrame(const cv::Mat &source) {
    int32_t col = source.cols;
    int32_t row = source.rows;
    int32_t _max = MAX(col, row);

    cv::Mat result = cv::Mat::zeros(_max, _max, CV_8UC3);
    source.copyTo(result(cv::Rect(0, 0, col, row)));

    return result;
}

void HandDetector::init()
{
    loadClassesFromFile();
    loadOnnxNetwork();     
}

void HandDetector::handDetectorWorker(FrameQueue &frameQueue)
{
    cv::Mat frame;

    while (true)
    {
        // Process oldest frame in queue
        if (!frameQueue.raw.empty())
        {
            frame = frameQueue.raw.front();
            std::vector<Detection> output;
            runInference(frame, output);

            // Alter frame by bounding boxes and text
            for (const Detection &detection : output)
            {
                cv::rectangle(frame, detection.box, detection.color, 3);

                cv::rectangle(frame, 
                              cv::Point(detection.box.x, detection.box.y - 20), 
                              cv::Point(detection.box.x + detection.box.width, detection.box.y), 
                              detection.color, 
                              cv::FILLED);

                cv::putText(frame, 
                            detection.className.c_str(), 
                            cv::Point(detection.box.x, detection.box.y - 5), 
                            cv::FONT_HERSHEY_SIMPLEX, 
                            0.5, 
                            cv::Scalar(0, 0, 0));

                std::cout << detection.className << ": " << std::setprecision(2) << detection.confidence << std::endl;
            }
            
            frameQueue.processed.push(frame);
            frameQueue.raw.pop();
        }
    }
}

void HandDetector::runInference(const cv::Mat &image, std::vector<Detection> &detections)
{
    cv::Mat blob;
    auto modelInput = resizeFrame(image);
    
    cv::dnn::blobFromImage(modelInput, blob, 1.0/255.0, cv::Size(config_->inputWidth, config_->inputHeight), cv::Scalar(), true, false);
    neuralNet_.setInput(blob);

    std::vector<cv::Mat> outputs;
    neuralNet_.forward(outputs, neuralNet_.getUnconnectedOutLayersNames());

    int32_t rows = outputs[0].size[1];
    int32_t dimensions = outputs[0].size[2];

    bool yolov8 = false;
    // yolov5 has an output of shape (batchSize, 25200, 85) (Num classes + box[x,y,w,h] + confidence[c])
    // yolov8 has an output of shape (batchSize, 84,  8400) (Num classes + box[x,y,w,h])
    
    if (dimensions > rows) // Check if the shape[2] is more than shape[1] (yolov8)
    {
        yolov8 = true;
        rows = outputs[0].size[2];
        dimensions = outputs[0].size[1];

        outputs[0] = outputs[0].reshape(1, dimensions);
        cv::transpose(outputs[0], outputs[0]);
    }
    float *data = (float *)outputs[0].data;

    float xFactor = modelInput.cols / config_->inputWidth;
    float yFactor = modelInput.rows / config_->inputHeight;

    std::vector<int32_t> class_ids;
    std::vector<float> confidences;
    std::vector<cv::Rect> boxes;

    for (uint32_t i = 0; i < rows; ++i)
    {
        if (yolov8)
        {
            float *classes_scores = data + 4;

            cv::Mat scores(1, classes_.size(), CV_32FC1, classes_scores);
            cv::Point class_id;
            double maxClassScore;

            minMaxLoc(scores, 0, &maxClassScore, 0, &class_id);

            if (maxClassScore > config_->scoreThreshold)
            {
                confidences.push_back(maxClassScore);
                class_ids.push_back(class_id.x);

                float x = data[0];
                float y = data[1];
                float w = data[2];
                float h = data[3];

                auto left = static_cast<int32_t>((x - 0.5 * w) * xFactor);
                auto top = static_cast<int32_t>((y - 0.5 * h) * yFactor);

                auto width = static_cast<int32_t>(w * xFactor);
                auto height = static_cast<int32_t>(h * yFactor);

                boxes.push_back(cv::Rect(left, top, width, height));
            }
        }
        else // yolov5
        {
            float confidence = data[4];

            if (confidence >= config_->confidenceThreshold)
            {
                float *classes_scores = data + 5;

                cv::Mat scores(1, classes_.size(), CV_32FC1, classes_scores);
                cv::Point class_id;
                double max_class_score;

                minMaxLoc(scores, 0, &max_class_score, 0, &class_id);

                if (max_class_score > config_->scoreThreshold)
                {
                    confidences.push_back(confidence);
                    class_ids.push_back(class_id.x);

                    float x = data[0];
                    float y = data[1];
                    float w = data[2];
                    float h = data[3];

                    auto left = static_cast<int32_t>((x - 0.5 * w) * xFactor);
                    auto top = static_cast<int32_t>((y - 0.5 * h) * yFactor);

                    auto width = static_cast<int32_t>(w * xFactor);
                    auto height = static_cast<int32_t>(h * yFactor);

                    boxes.push_back(cv::Rect(left, top, width, height));
                }
            }
        }

        data += dimensions;
    }

    std::vector<int> nms_result;
    cv::dnn::NMSBoxes(boxes, confidences, config_->scoreThreshold, config_->nmsThreshold, nms_result);

    for (unsigned long i = 0; i < nms_result.size(); ++i)
    {
        int32_t idx = nms_result[i];

        Detection result;
        result.classID = class_ids[idx];
        result.confidence = confidences[idx];
        result.className = classes_[result.classID];

        static std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(100, 255);
        result.color = cv::Scalar(dis(gen),
                                  dis(gen),
                                  dis(gen));

        result.box = boxes[idx];

        detections.push_back(result);
    }
}
