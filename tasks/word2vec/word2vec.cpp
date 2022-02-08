#include "word2vec.h"
#include <vector>

int VecClose(std::vector<int> v1, std::vector<int> v2) {
    int res = 0;
    for (int i = 0; i < v2.size(); ++i) {
        res += v1[i] * v2[i];
    }
    return res;
}
std::vector<std::string> FindClosestWords(const std::vector<std::string>& words,
                                          const std::vector<std::vector<int>>& vectors) {
    std::vector<std::string> result;
    int vc_max = VecClose(vectors[0], vectors[1]);
    for (int i = 1; i < vectors.size(); ++i) {
        if (VecClose(vectors[0], vectors[i]) > vc_max) {
            vc_max = VecClose(vectors[0], vectors[i]);
        }
    }
    for (int i = 1; i < vectors.size(); ++i) {
        if (VecClose(vectors[0], vectors[i]) == vc_max) {
            result.push_back(words[i]);
        }
    }
    return result;
}
