#include "word2vec.h"
#include <vector>

int VecClose(const std::vector<int>& v1, const std::vector<int>& v2) {
    int res = 0;
    for (size_t i = 0; i < v2.size(); ++i) {
        res += v1[i] * v2[i];
    }
    return res;
}
std::vector<std::string> FindClosestWords(const std::vector<std::string>& words,
                                          const std::vector<std::vector<int>>& vectors) {
    std::vector<std::string> result;
    int vc_max = -99999999;
    int ind = -9999;
    for (size_t i = 1; i < vectors.size(); ++i) {
        if (VecClose(vectors[0], vectors[i]) > vc_max) {
            vc_max = VecClose(vectors[0], vectors[i]);
            ind = i;
        }
    }
    if (abs(ind) < vectors.size()) {
        result.push_back(words[ind]);
    }
    for (size_t i = 1; i < vectors.size(); ++i) {
        if (VecClose(vectors[0], vectors[i]) == vc_max && ind != i) {
            result.push_back(words[i]);
        }
    }
    return result;
}