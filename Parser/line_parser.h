#ifndef LINEPARSER_H
#define LINEPARSER_H

#include <algorithm>

#include "common.h"


std::vector<std::vector<std::pair<int,int>>> SplitByLines(Image image) {
    double present = 0.15;
    std::vector<int> arr(image.h);
    for (int x = 0; x < image.w; x++) {
        for (int y = 0; y < image.h; y++) {
            arr[y] += image[x][y] == BLACK_COLOR;
        }
    }


    const int max_value = *std::max_element(arr.begin(), arr.end());
    const int border = max_value * present;
    const int minimal_line_height = 50;

    std::vector<bool> is_line(image.h, false);
    for (int i = 0; i < image.h; i++) {
        is_line[i] = arr[i] >= border;
    }

    std::vector<int> line_centers;
    int count = 0;
    for (int i = 0; i < image.h; i++) {
        if (is_line[i]) {
            count++;
        } else {
            if (count > minimal_line_height) {
                line_centers.push_back(i - count);
            }
            count = 0;
        }
    }
    std::vector<std::vector<std::pair<int,int>>> result;
    if (line_centers.size() > 1) {
        int summary_dist = line_centers.back() - line_centers.front();
        int avg_height = summary_dist / (line_centers.size() - 1);
        for (int y_mid : line_centers) {
            result.emplace_back();
            for (int y = std::max(0, y_mid - avg_height / 2); y < std::min(y_mid + avg_height / 2, image.h); y++) {
                for (int x = 0; x < image.w; x++) {
                    if (image[x][y] == BLACK_COLOR) {
                        result.back().emplace_back(x, y);
                    }
                }
            }
        }
    } else {
        result.emplace_back();
        for (int x = 0; x < image.w; x++) {
            for (int y = 0; y < image.h; y++) {
                if (image[x][y] == BLACK_COLOR) {
                    result.back().emplace_back(x, y);
                }
            }
        }
    }
    return result;
}

#endif // LINEPARSER_H
