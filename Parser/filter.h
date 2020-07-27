#include "common.h"
#include <chrono>
#include <random>
#include <iostream>

std::mt19937 rnd(std::chrono::steady_clock::now().time_since_epoch().count());

std::vector<std::vector<double>> Blur(std::vector<std::vector<int>>& alpha) {
    int w = alpha.size();
    int h = w ? alpha[0].size() : 0;
    auto blur = newArray<double>(w, h);
    int delta = 30;
    auto sum = newArray<int>(w + delta * 2, h + delta * 2);
    for (int i = 0; i < w + delta * 2; i++) {
        for (int j = 0; j < h + delta * 2; j++) {
            int x = std::max(0, std::min(w - 1, i - delta));
            int y = std::max(0, std::min(h - 1, j - delta));
            sum[i][j] = alpha[x][y];
            if (j > 0) {
                sum[i][j] += sum[i][j - 1];
            }
        }
        if (i > 0) {
            for (int j = 0; j < h + delta * 2; j++) {
                sum[i][j] += sum[i - 1][j];
            }
        }
    }

    int S = (delta * 2 + 1) * (delta * 2 + 1);

    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            int x2 = i + delta * 2;
            int y2 = j + delta * 2;
            blur[i][j] = sum[x2][y2];
            if (i > 0) blur[i][j] -= sum[i - 1][y2];
            if (j > 0) blur[i][j] -= sum[x2][j - 1];
            if (i > 0 && j > 0) blur[i][j] += sum[i - 1][j - 1];
            blur[i][j] /= S;
        }
    }

    return blur;
}


std::map<int,int> KMeans(std::map<int,int> map, int k) {
    int n = map.size();
    std::vector<int> centers(k);
    std::vector<int> color;
    std::vector<int> quantity;
    for (auto& [key, value] : map) {
        color.push_back(key);
        quantity.push_back(value);
    }
    for (auto& value : centers) {
        value = rnd() % (WHITE_COLOR + 1);
    }

    std::vector<int> classOfColor(n);
    int summaryError = 1;
    int itr = 0;
    std::vector<int> dr(k), dg(k), db(k);
    std::vector<int> q(k);
    std::vector<std::tuple<int,int,int>> def;
    for (int clr : color) {
        def.push_back(GetColor(clr));
    }
    std::vector<std::tuple<int,int,int>> curCent(k);

    while (summaryError) {
        itr++;
        for (int i = 0; i < k; i++) {
            curCent[i] = GetColor(centers[i]);
        }
        for (int i = 0; i < n; i++) {
            int best = 1e9;
            auto& [r1, g1, b1] = def[i];
            for (int j = 0; j < k; j++) {
                auto& [r2, g2, b2] = curCent[j];
                int val = std::abs(r1 - r2) + std::abs(g1 - g2) + std::abs(b1 - b2);
                if (val < best) {
                    best = val;
                    classOfColor[i] = j;
                }
            }
        }
        std::fill(dr.begin(), dr.end(), 0);
        std::fill(dg.begin(), dg.end(), 0);
        std::fill(db.begin(), db.end(), 0);
        std::fill(q.begin(), q.end(), 0);
        summaryError = 0;
        for (int i = 0; i < n; i++) {
            int c = classOfColor[i];
            auto [cr, cg, cb] = GetColor(centers[c]);
            const auto& [r, g, b] = def[i];
            q[c] += quantity[i];
            dr[c] += (r - cr) * quantity[i];
            dg[c] += (g - cg) * quantity[i];
            db[c] += (b - cb) * quantity[i];
        }
        for (int i = 0; i < k; i++) {
            auto [r, g, b] = GetColor(centers[i]);
            int dx = dr[i] / (q[i] + 1);
            int dy = dg[i] / (q[i] + 1);
            int dz = db[i] / (q[i] + 1);
            r += dx;
            g += dy;
            b += dz;
            summaryError += (std::abs(dx) + std::abs(dy) + std::abs(dz));
            centers[i] = (r << 16) ^ (g << 8) ^ b;
        }
    }
    
    for (int i = 0; i < n; i++) {
        map[color[i]] = centers[classOfColor[i]];
    }
    return map;
}

Image ApplyMap(const Image& image, std::map<int,int>& map) {
    int w = image.w;
    int h = image.h;
    Image result(image);
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            result[i][j] = map[result[i][j]];
        }
    }
    return result;
}

int CountBadPixels(Image& image) {
    int counter = 0;
    for (int i = 0; i + 1 < image.w; i++) {
        for (int j = 0; j + 1 < image.h; j++) {
            if (image[i][j] == image[i][j + 1]) {
                if (image[i + 1][j] == image[i + 1][j + 1]) {
                    if (image[i][j] != image[i + 1][j]) {
                        counter++;
                    }
                }
            }
        }
    }
    return counter;
}

Image RemoveErrorPoints(Image image, int borderCount) {
    DSU_2d dsu(image.w, image.h);
    std::map<std::pair<int,int>, int> map;

    for (int i = 0; i + 1 < image.w; i++) {
        for (int j = 0; j + 1 < image.h; j++) {
            if (image[i][j] == BLACK_COLOR) {
                for (auto& [nx, ny] : std::vector<std::pair<int,int>>{{i, j + 1}, {i + 1, j}}) {
                    if (image[nx][ny] == BLACK_COLOR) {
                        dsu.uni({i, j}, {nx, ny});
                    }
                }
            }

        }
    }
    for (int x = 0; x < image.w; x++) {
        for (int y = 0; y < image.h; y++) {
            map[{x, y}] = dsu.Size(dsu.get(x, y));
        }
    }
    std::map<std::pair<int,int>, int> colorMap;

    for (auto& [key, value] : map) {
        auto [x, y] = key;
        if (value < borderCount || value > borderCount * 100) {
            image[x][y] = WHITE_COLOR;
        } else {
            image[x][y] = BLACK_COLOR;
        }
    }
    return image;
}

Image Filter(Image image) {
    int w = image.w;
    int h = image.h;
    auto alpha = newArray<int>(w, h);
    int brightness = 40;
    
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            image.AddColor(x, y, brightness, brightness, brightness);
            auto [r, g, b] = image.GetColor(x, y);
            alpha[x][y] = (r + g + b) / 3;
        }
    }

	
    auto coefficients = Blur(alpha);
    
    std::map<int,int> map;
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            auto [r, g, b] = image.GetColor(x, y);
            r = r * 200 / coefficients[x][y];
            g = g * 200 / coefficients[x][y];
            b = b * 200 / coefficients[x][y];
            image.UpdateColor(x, y, r, g, b);
            map[image[x][y]]++;
        }
    }

	
    int k = 7;
    int bestResult = 1 << 29;
    auto resultImage = image;
    
    
    for (int i = 0; i < 10; i++) {
        auto nextMap = KMeans(map, k);
        auto currentImage = ApplyMap(image, nextMap);
        auto currentResult = CountBadPixels(currentImage);
        if (bestResult > currentResult) {
            bestResult = currentResult;
            resultImage = currentImage;
        }
    }
    
    
    map.clear();
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            map[resultImage[x][y]]++;
        }
    }
    
    
    int totalAmountOfPixels = w * h;
    
    
    double borderPresents = 0.15;
    int borderAmount = totalAmountOfPixels * borderPresents;
			
		

    for (auto& [key, amount] : map) {
        if (amount >= borderAmount) {
            amount = WHITE_COLOR;
        } else {
            amount = BLACK_COLOR;
        }
    }
    resultImage = ApplyMap(resultImage, map);
    
    resultImage = RemoveErrorPoints(resultImage, 30);
    return resultImage;
}

