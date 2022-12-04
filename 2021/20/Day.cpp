#include "Day.h"

#include "FactoryManager.h"
#include "helpers.h"

#include <iostream>
#include <optional>

namespace aoc {

namespace {

constexpr char LIGHT{'#'};
constexpr char DARK{'.'};
constexpr size_t MARGIN{2};

void print(const Image& image, const std::string& name) {
    std::cout << std::endl << name << std::endl;
    for (int y = 0; y < image.height; y++) {
        std::cout << image.pixels.substr(y * image.width, image.width) << std::endl;
    }
    std::cout << "[ " << image.width << ", " << image.height << " ]" << std::endl << std::endl;
}

std::optional<char> getPixel(const Image& input, int x, int y) {
    if (x > 0 && x < input.width && y > 0 && y < input.height) {
        return input.pixels.at(y * input.width + x);
    }
    return std::nullopt;
}

void setPixel(Image& input, int x, int y, char value) {
    input.pixels.at(y * input.width + x) = value;
}

std::string getKernel(const Image& input, int x, int y) {
    std::string kernel(9, input.outside);
    for (int j = -1; j < 2; j++) {
        for (int i = -1; i < 2; i++) {
            if (auto pixel = getPixel(input, x + i, y + j)) {
                kernel.at((i + 1) + (j + 1) * 3) = pixel.value();
            }
        }
    }
    return kernel;
}

int32_t kernelToIndex(const std::string& kernel) {
    int32_t index{0};
    for (int i = 0; i < kernel.size(); i++) {
        if (kernel.at(i) == LIGHT) {
            index |= (1 << (8 - i));
        }
    }
    return index;
}

Image expand(const Image& input, size_t margin) {
    Image output;
    output.width = input.width + 2 * margin;
    output.height = input.height + 2 * margin;
    output.outside = input.outside;
    output.pixels = std::string(output.width * output.height, input.outside);
    auto destination = output.pixels.begin() + margin * output.width;
    for (int y = 0; y < input.height; y++) {
        auto start = input.pixels.begin() + y * input.width;
        auto end = start + input.width;
        std::copy(start, end, destination + margin);
        destination += output.width;
    }
    return output;
}

Image enhance(const Image& input, const Algorithm& algorithm) {
    // Expand the image to accomodate for the size of the kernel
    Image expanded = expand(input, MARGIN);
    Image output(expanded);
    // Process pixels inside the image
    for (int y = 0; y < output.height; y++) {
        for (int x = 0; x < output.width; x++) {
            std::string kernel = getKernel(expanded, x, y);
            auto idx = kernelToIndex(kernel);
            setPixel(output, x, y, algorithm.at(idx));
        }
    }
    // Process infinite pixels outside the image
    std::string kernel(9, input.outside);
    auto idx = kernelToIndex(kernel);
    output.outside = algorithm.at(idx);
    return output;
}

Result countLightPixels(const Image& image) {
    return std::count_if(image.pixels.begin(), image.pixels.end(),
                         [](const auto& pixel) { return pixel == LIGHT; });
}

}  // namespace

Input Day::read(std::ifstream& line_stream) const {
    std::string line;
    Algorithm algorithm;
    while (std::getline(line_stream, line)) {
        if (line.empty()) {
            break;
        }
        algorithm.append(line);
    }

    Image image;
    image.outside = DARK;
    while (std::getline(line_stream, line)) {
        if (image.width == 0) {
            image.width = line.size();
        }
        image.pixels.append(line);
        image.height++;
    }
    return {algorithm, image};
}

Result Day::partOne(const Input& input) const {
    // print(input.second, "Input");

    Image first_pass = enhance(input.second, input.first);
    // print(first_pass, "First");

    Image second_pass = enhance(first_pass, input.first);
    // print(second_pass, "Second");

    return countLightPixels(second_pass);
}

Result Day::partTwo(const Input& input) const {
    Image image(input.second);
    for (int i = 0; i < 50; i++) {
        image = enhance(image, input.first);
    }

    return countLightPixels(image);
}

extern "C" void registerFactories(FactoryManager* manager) {
    manager->registerSolution<Day>(YEAR + "-" + DAY);
}

}  // namespace aoc