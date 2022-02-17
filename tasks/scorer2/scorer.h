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
    void OnCheckFailed(const StudentName& student_name, const TaskName& task_name);
    void OnCheckSuccess(const StudentName& student_name, const TaskName& task_name);
    void OnMergeRequestOpen(const StudentName& student_name, const TaskName& task_name);
    void OnMergeRequestClosed(const StudentName& student_name, const TaskName& task_name);
    void Reset();
    ScoreTable GetScoreTable() const;

private:
    ScoreTable score_table_;
    std::set<TaskName> set_;
    void DelStudents(const StudentName& student_name) {
        if (score_table_[student_name].empty()) {
            score_table_.erase(student_name);
        }
    }
    void DelFromTable(const StudentName& student_name, const TaskName& task_name) {
        score_table_[student_name].erase(task_name);
        DelStudents(student_name);
    }
    void AddToTable(const StudentName& student_name, const TaskName& task_name) {
        if (score_table_.find(student_name) == score_table_.end()) {
            set_.insert(task_name);
            score_table_.insert({student_name, set_});
        } else {
            score_table_[student_name].insert(task_name);
        }
        set_.clear();
    }
};