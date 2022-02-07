#include "tests_checking.h"
#include <deque>

std::vector<std::string> StudentsOrder(const std::vector<StudentAction>& student_actions,
                                       const std::vector<size_t>& queries) {
    std::vector<std::string> result;
    std::deque<std::string> students;
    for (size_t i = 0; i < student_actions.size(); ++i) {
        switch (student_actions[i].side) {
            case Side::Top:
                students.push_front(student_actions[i].name);
            case Side::Bottom:
                students.push_back(student_actions[i].name);
            default:
                break;
        }
    }
    for (size_t i = 0; i < queries.size(); ++i) {
        result.push_back(students[queries[i] - 1]);
    }
    return result;
}