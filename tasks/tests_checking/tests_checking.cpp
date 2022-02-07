#include "tests_checking.h"

std::vector<std::string> StudentsOrder(const std::vector<StudentAction>& student_actions,
                                       const std::vector<size_t>& queries) {
    std::vector<std::string> result;
    std::vector<std::string> students;
    for (size_t i = 0; i < student_actions.size(); ++i) {
        if (student_actions[i].side == Side::Bottom) {
            students[i] = student_actions[i].name;
        }
        if (student_actions[i].side == Side::Top) {
            students.resize(students.size() + 1);
            for (size_t i = students.size() - 1; i > 0; --i) {
                students[i] = students[i - 1];
            }
            students[0] = student_actions[i].name;
        }
    }
    for (size_t i = 0; i < queries.size(); ++i) {
        result[i] = students[queries[i] - 1];
    }
    return result;
}