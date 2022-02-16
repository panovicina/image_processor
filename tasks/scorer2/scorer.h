#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

using StudentName = std::string;
using TaskName = std::string;

using ScoreTable = std::map<StudentName, std::set<TaskName>>;

class Scorer {
public:
    enum class EventType {
        CheckFailed,
        CheckSuccess,
        MergeRequestOpen,
        MergeRequestClosed,
    };
    void DelStudents(const StudentName& student_name);
    void DelFromTable(const StudentName& student_name, const TaskName& task_name);
    void OnCheckFailed(const StudentName& student_name, const TaskName& task_name);
    void OnCheckSuccess(const StudentName& student_name, const TaskName& task_name);
    void OnMergeRequestOpen(const StudentName& student_name, const TaskName& task_name);
    void OnMergeRequestClosed(const StudentName& student_name, const TaskName& task_name);
    void Reset();
    void AddToTable(const StudentName& student_name, const TaskName& task_name);

    ScoreTable GetScoreTable() const;
};
