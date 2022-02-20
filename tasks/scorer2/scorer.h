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
    void DelStudents(ScoreTable& table) {
        for (auto it = table.begin(); it != table.end();) {
            if (it->second.empty()) {
                it = table.erase(it);

            } else {
                it++;
            }
        }
    }
    void DelFromTable(ScoreTable& table, const StudentName& student_name, const TaskName& task_name) {
        if (table.find(student_name) != table.end()) {
            std::set<TaskName>& tasks = table.find(student_name)->second;
            Tasks.erase(task_name);
        }
        DelStudents(table);
    }
    void AddToTable(ScoreTable& table, const StudentName& student_name, const TaskName& task_name) {
        if (table.find(student_name) == table.end()) {
            table[student_name] = {task_name};
        } else {
            table[student_name].insert(task_name);
        }
    }
};
