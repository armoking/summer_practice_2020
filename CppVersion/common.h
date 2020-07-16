#ifndef COMMON_H
#define COMMON_H

#include <QImage>
#include <QDebug>

constexpr int WHITE_COLOR = 0xffffff;
constexpr int BLACK_COLOR = 0x000000;
constexpr int RED_COLOR = 0xff0000;
constexpr int GREEN_COLOR = 0x00ff00;
constexpr int BLUE_COLOR = 0x0000ff;

std::tuple<int, int, int> GetColor(int color) {
    color &= WHITE_COLOR;
    const int red = (color & RED_COLOR) >> 16;
    const int green = (color & GREEN_COLOR) >> 8;
    const int blue = (color & BLUE_COLOR) >> 0;
    return {red, green, blue};
}

class Image {
public:
    int w, h;
    std::vector<std::vector<int>> image;

    Image(const QImage& qImage)
        : w(qImage.width())
        , h(qImage.height())
        , image(w, std::vector<int>(h, 0))
    {
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                image[x][y] = qImage.pixel(x, y) & WHITE_COLOR;
            }
        }
    }

    Image(const std::vector<std::vector<int>>& img)
        : w(img.size())
        , h(img.size() ? img[0].size() : 0)
        , image(img)
    {}

    std::vector<int>& operator [] (int i) {
        return image[i];
    }

    std::tuple<int, int, int> GetColor(int x, int y) const {
        const int color = image[x][y] & WHITE_COLOR;
        const int red = (color & RED_COLOR) >> 16;
        const int green = (color & GREEN_COLOR) >> 8;
        const int blue = (color & BLUE_COLOR) >> 0;
        return {red, green, blue};
    }

    void SaveImage(QString path) const {
        QImage res(w, h, QImage::Format::Format_RGB32);
        for (int i = 0; i < w; i++) {
            for (int j = 0; j < h; j++) {
                res.setPixelColor(i, j, image[i][j]);
            }
        }
        res.save(path);
    }

    void UpdateR(int x, int y, int val) {
        val = std::min(val, 255);
        val = std::max(0, val);
        int& color = image[x][y];
        color &= (WHITE_COLOR ^ RED_COLOR);
        color ^= val << 16;
    }

    void UpdateG(int x, int y, int val) {
        val = std::min(val, 255);
        val = std::max(0, val);
        int& color = image[x][y];
        color &= (WHITE_COLOR ^ GREEN_COLOR);
        color ^= val << 8;
    }

    void UpdateB(int x, int y, int val) {
        val = std::min(val, 255);
        val = std::max(0, val);
        int& color = image[x][y];
        color &= (WHITE_COLOR ^ BLUE_COLOR);
        color ^= val << 0;
    }

    void UpdateColor(int x, int y, int val) {
        val = std::min(val, WHITE_COLOR);
        val = std::max(0, val);
        image[x][y] = val;
    }

    void UpdateColor(int x, int y, int r, int g, int b) {
        r = std::min(r, 255);
        g = std::min(g, 255);
        b = std::min(b, 255);
        r = std::max(r, 0);
        g = std::max(g, 0);
        b = std::max(b, 0);
        int val = (r << 16) ^ (g << 8) ^ (b << 0);
        image[x][y] = val;
    }

    void SetGrey(int x, int y) {
        auto [r, g, b] = GetColor(x, y);
        int val = (r + g + b) / 3;
        val = std::min(val, 255);
        val = std::max(val, 0);
        image[x][y] = val * ((1 << 16) | (1 << 8) | 1);
    }

    void AddColor(int x, int y, int dr, int dg, int db) {
        auto [r, g, b] = GetColor(x, y);
        UpdateColor(x, y, r + dr, g + dg, b + db);
    }
};

template<typename T>
std::vector<std::vector<T>> newArray(int w, int h) {
    return std::vector<std::vector<T>>(w, std::vector<T>(h));
}

class DSU_2d {
private:
    std::vector<std::vector<std::pair<int,int>>> parent;
    std::vector<std::vector<int>> size;
public:
    DSU_2d(int w, int h)
        : parent(w, std::vector<std::pair<int,int>>(h))
        , size(w, std::vector<int>(h, 1))
    {
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                parent[x][y] = {x, y};
            }
        }
    }

    std::pair<int,int> get(std::pair<int,int> p) {
        return get(p.first, p.second);
    }

    std::pair<int,int> get(int x, int y) {
        if (parent[x][y] == std::pair<int,int>(x, y)) return {x, y};
        return parent[x][y] = get(parent[x][y]);
    }

    int& Size(std::pair<int,int> a) {
        return size[a.first][a.second];
    }

    void uni(std::pair<int,int> a, std::pair<int,int> b) {
        a = get(a.first, a.second);
        b = get(b.first, b.second);
        if (a == b) {
            return;
        }
        if (Size(a) < Size(b)) {
            std::swap(a, b);
        }
        parent[b.first][b.second] = a;
        Size(a) += Size(b);
    }
};

#endif // COMMON_H
