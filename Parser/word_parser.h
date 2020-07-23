#ifndef WORD_PARSER_H
#define WORD_PARSER_H

#define BORDER 50
#include <vector>
#include <queue>
#include <algorithm>

#include "common.h"

using point = std::pair<int,int>;

const std::vector<point> steps = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

std::pair<std::pair<point, point>, std::vector<point>> GetMaxDistance(std::vector<std::vector<bool>>& g, std::vector<std::vector<bool>>& used, int x, int y) {
    point max(x, y);
    point min(x, y);
    std::queue<point> queue;
    queue.push(min);
    used[x][y] = true;
    std::vector<point> visited;
    int w = g.size();
    int h = w ? g[0].size() : 0;

    while (queue.size()) {
        auto [cx, cy] = queue.front();
        queue.pop();
        max.first = std::max(max.first, cx);
        max.second = std::max(max.second, cy);
        min.first = std::min(min.first, cx);
        min.second = std::min(min.second, cy);
        for (auto& [dx, dy] : steps) {
            int nx = dx + cx;
            int ny = dy + cy;
            if (nx < 0 || ny < 0 || nx >= w || ny >= h || used[nx][ny] || !g[nx][ny]) continue;
            used[nx][ny] = true;
            queue.emplace(nx, ny);
        }
    }
    return {{min, max}, visited};
}


std::vector<std::vector<point>> WordParse(std::vector<point>& line) {
    std::sort(std::begin(line), std::end(line));
    int n = line.size();
    auto dsu = DSU(n);
    std::map<point, int> map;
    for (int i = 0; i < n; i++) {
        map[line[i]] = i;
    }
    for (int i = 0; i < n; i++) {
        auto& [x, y] = line[i];
        for (auto& [dx, dy] : steps) {
            int nx = dx + x;
            int ny = dy + y;
            if (map.count({nx, ny})) {
                int nextIndex = map[{nx, ny}];
                dsu.uni(nextIndex, i);
            }
        }
    }

    std::map<int, std::vector<point>> letterMap;

    for (int i = 0; i < n; i++) {
        int p = dsu.get(i);
        auto& currentPixels = letterMap[p];
        currentPixels.push_back(line[i]);
    }

    std::vector<std::vector<point>> letters;
    for (auto& [p, letter] : letterMap) {
        if (letter.size() >= BORDER) {
            letters.push_back(letter);
        }
    }

    int m = letters.size();
    if (m == 0) {
        return {};
    }

    std::vector<int> distances;
    std::vector<int> distToNext (m - 1);
    for (int i = 1; i < m; i++) {
        int dist = 1e9;
        for (auto& [cx, cy] : letters[i]) {
            for (auto& [px, py] : letters[i - 1]) {
                dist = std::min(dist, (cx - px) * (cx - py) + (cy - py) * (cy - py));
            }
        }
        distToNext[i - 1] = dist;
        distances.push_back(dist);
    }

    std::sort(std::begin(distances), std::end(distances));


    int distancesBetweenWords = 0;
    for (int i = 0; i < int(distances.size()) - 1; i++) {
        if (distances[i] * 3 <= distances[i + 1]) {
            distancesBetweenWords = distances[i] + 1;
            break;
        }
    }

    std::vector<std::vector<point>> answer;

    for (int i = 0; i < m; i++) {
        answer.push_back(letters[i]);
        if (i + 1 < m && distToNext[i] >= distancesBetweenWords) {
            answer.push_back({});
        }
    }

    return answer;
}





#endif // WORD_PARSER_H
