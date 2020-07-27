#include "common.h"
#include "filter.h"
#include "line_parser.h"
#include "word_parser.h"

#include <iostream>


const std::string PATH_TO_SOURCE = "C:\\Users\\User\\Desktop\\github\\summer_practice_2020\\source.txt";
const std::string PATH_TO_RESULT = "C:\\Users\\User\\Desktop\\github\\summer_practice_2020\\result.txt";


int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
	
	
	auto input = std::freopen(PATH_TO_SOURCE.c_str(), "r", stdin);
	int w, h;
	std::cin >> w >> h;
	std::vector<std::vector<int>> img(w, std::vector<int>(h));
	for (int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {
			std::cin >> img[x][y];
		}
	}
	std::fclose(input);
	
    Image image(img);
    image = Filter(image);
	
    auto lines = SplitByLines(image);
    std::cout << lines.size() << std::endl;
    std::vector<std::vector<std::pair<int,int>>> letters;
    for (auto& line : lines) {
        auto current = WordParse(line);
        
		std::cout << current.size() << std::endl;
        for (auto& letter : current) {
            letters.push_back(letter);

        }
    }

    
    auto output = std::freopen(PATH_TO_RESULT.c_str(), "w", stdout);
    std::cout << letters.size() << '\n';
    for (auto& letter : letters) {
        Image image(letter);
        std::cout << image.w << ' ' << image.h << '\n';
        for (int x = 0; x < image.w; x++) {
            for (int y = 0; y < image.h; y++) {
                std::cout << (image[x][y] == 0);
            }
            std::cout << '\n';
        }
    }
    
    
    std::cout << std::endl;
    std::fclose(output);
}
