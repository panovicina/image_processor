#include "scorer.h"

ScoreTable was_failed;
ScoreTable was_merge_open;
using ScoreTable = std::map<StudentName, std::set<TaskName>>;
void Scorer::OnCheckFailed(const StudentName& student_name, const TaskName& task_name) {
    DelFromTable(student_name, task_name);
    was_failed[student_name].insert(task_name);
}
void Scorer::OnCheckSuccess(const StudentName& student_name, const TaskName& task_name) {
    if (was_merge_open[student_name].find(task_name) == was_merge_open[student_name].end()) {
        AddToTable(student_name, task_name);
    }
    if (was_failed[student_name].find(task_name) != was_failed[student_name].end()) {
        was_failed[student_name].erase(task_name);
    }
}
void Scorer::OnMergeRequestOpen(const StudentName& student_name, const TaskName& task_name) {
    DelFromTable(student_name, task_name);
    was_merge_open[student_name].insert(task_name);
}
void Scorer::OnMergeRequestClosed(const StudentName& student_name, const TaskName& task_name) {
    if (was_failed[student_name].find(task_name) == was_failed[student_name].end()) {
        AddToTable(student_name, task_name);
    }
    if (was_merge_open[student_name].find(task_name) != was_merge_open[student_name].end()) {
        was_merge_open[student_name].erase(task_name);
    }
}
void Scorer::Reset() {
    score_table_.clear();
}
ScoreTable Scorer::GetScoreTable() const {
    return score_table_;
}