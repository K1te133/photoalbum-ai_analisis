#ifndef IMAGEANALYZER_H
#define IMAGEANALYZER_H

#include <opencv2/dnn.hpp>
#include <QString>
#include <QImage>

class ImageAnalyzer {
public:
    ImageAnalyzer();
    ~ImageAnalyzer();
    bool loadModel(const QString& modelPath);
    bool isImageAIDetected(const QImage& image);
    float getAIProbability() const;
    bool isModelLoaded() const;

private:
    cv::dnn::Net net;
    float aiProbability;
};

#endif
