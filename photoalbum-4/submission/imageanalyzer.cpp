#include "ImageAnalyzer.h"
#include <QDebug>
#include <opencv2/imgproc.hpp>

ImageAnalyzer::ImageAnalyzer() : aiProbability(-1.0f) {}

ImageAnalyzer::~ImageAnalyzer() {}

bool ImageAnalyzer::loadModel(const QString& modelPath) {
    try {
        net = cv::dnn::readNetFromONNX(modelPath.toStdString());
        if (net.empty()) return false;
        aiProbability = -1.0f;
        return true;
    } catch (const cv::Exception& e) {
        qDebug() << "Ошибка загрузки модели:" << e.what();
        return false;
    }
}

bool ImageAnalyzer::isImageAIDetected(const QImage& image) {
    if (net.empty()) {
        qDebug() << "Модель не загружена.";
        aiProbability = -1.0f;
        return false;
    }

    // Конвертируем QImage в cv::Mat
    QImage converted = image.convertToFormat(QImage::Format_RGB888);
    cv::Mat mat(converted.height(), converted.width(), CV_8UC3,
                const_cast<uchar*>(converted.bits()),
                converted.bytesPerLine());

    cv::Mat inputBlob = cv::dnn::blobFromImage(mat, 1.0, cv::Size(224, 224), cv::Scalar(), true, false);
    net.setInput(inputBlob);
    cv::Mat output = net.forward();

    if (output.empty() || output.total() != 1) {
        qDebug() << "Неожиданный формат выходного тензора:" << output.size;
        aiProbability = -1.0f;
        return false;
    }

    aiProbability = output.at<float>(0);
    qDebug() << "Вероятность сгенерированного фото:" << aiProbability;
    return aiProbability > 0.5f;
}

float ImageAnalyzer::getAIProbability() const {
    return aiProbability;
}

bool ImageAnalyzer::isModelLoaded() const {
    return !net.empty();
}
