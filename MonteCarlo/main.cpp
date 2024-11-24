#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <algorithm>
#include <iomanip>

struct Circle {
    double centerX;
    double centerY;
    double radius;
};

bool isPointInsideAllCircles(double x, double y, const Circle& circle1, const Circle& circle2, const Circle& circle3) {
    auto isInside = [&](const Circle& circle) -> bool {
        double dx = x - circle.centerX;
        double dy = y - circle.centerY;
        return (dx * dx + dy * dy) <= (circle.radius * circle.radius);
    };
    return isInside(circle1) && isInside(circle2) && isInside(circle3);
}

double generateRandomDouble(double min, double max) {
    return min + (max - min) * static_cast<double>(rand()) / RAND_MAX;
}

int countPointsInside(int numSamples, double xMin, double xMax, double yMin, double yMax,
                      const Circle& circle1, const Circle& circle2, const Circle& circle3) {
    int countInside = 0;
    for (int i = 0; i < numSamples; ++i) {
        double x = generateRandomDouble(xMin, xMax);
        double y = generateRandomDouble(yMin, yMax);
        if (isPointInsideAllCircles(x, y, circle1, circle2, circle3)) {
            ++countInside;
        }
    }
    return countInside;
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    Circle circle1 = {1.0, 1.0, 1.0};
    Circle circle2 = {1.5, 2.0, std::sqrt(5) / 2.0};
    Circle circle3 = {2.0, 1.5, std::sqrt(5) / 2.0};

    double wideXMin = circle1.centerX - circle1.radius;
    double wideXMax = circle3.centerX + circle3.radius;
    double wideYMin = circle1.centerY - circle1.radius;
    double wideYMax = circle2.centerY + circle2.radius;
    double wideArea = (wideXMax - wideXMin) * (wideYMax - wideYMin);

    double narrowXMin = 1.0;
    double narrowXMax = 2.0;
    double narrowYMin = 1.0;
    double narrowYMax = 2.0;
    double narrowArea = (narrowXMax - narrowXMin) * (narrowYMax - narrowYMin);

    double exactArea = 0.25 * M_PI + 1.25 * std::asin(0.8) - 1.0;

    std::ofstream outputFile("data.csv");
    outputFile << "N,Area_Wide,Error_Wide,Area_Narrow,Error_Narrow\n";

    for (int sampleSize = 100; sampleSize <= 100000; sampleSize += 500) {
        int countWide = countPointsInside(sampleSize, wideXMin, wideXMax, wideYMin, wideYMax,
                                          circle1, circle2, circle3);
        double estimatedAreaWide = static_cast<double>(countWide) / sampleSize * wideArea;
        double errorWide = std::fabs((estimatedAreaWide - exactArea) / exactArea) * 100.0;

        int countNarrow = countPointsInside(sampleSize, narrowXMin, narrowXMax, narrowYMin, narrowYMax,
                                            circle1, circle2, circle3);
        double estimatedAreaNarrow = static_cast<double>(countNarrow) / sampleSize * narrowArea;
        double errorNarrow = std::fabs((estimatedAreaNarrow - exactArea) / exactArea) * 100.0;

        outputFile << sampleSize << ","
                   << estimatedAreaWide << ","
                   << errorWide << ","
                   << estimatedAreaNarrow << ","
                   << errorNarrow << "\n";

        if (sampleSize % 5000 == 0) {
            std::cout << "Processed N = " << sampleSize << std::endl;
        }
    }

    outputFile.close();
    std::cout << "Data saved to file: data.csv" << std::endl;
}
