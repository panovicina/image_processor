#include "scorer.h"

ScoreTable check_condition;
ScoreTable merge_condition;
using ScoreTable = std::map<StudentName, std::set<TaskName>>;
void Scorer::OnCheckFailed(const StudentName& student_name, const TaskName& task_name) {
    DelFromTable(check_condition, student_name, task_name);
    DelFromTable(score_table_, student_name, task_name);
}
void Scorer::OnCheckSuccess(const StudentName& student_name, const TaskName& task_name) {
    AddToTable(check_condition, student_name, task_name);
    if (merge_condition[student_name].find(task_name) == merge_condition[student_name].end()) {
        AddToTable(score_table_, student_name, task_name);
    }
    DelStudents(score_table_);
}
void Scorer::OnMergeRequestOpen(const StudentName& student_name, const TaskName& task_name) {
    AddToTable(merge_condition, student_name, task_name);
    DelFromTable(score_table_, student_name, task_name);
}
void Scorer::OnMergeRequestClosed(const StudentName& student_name, const TaskName& task_name) {
    DelFromTable(merge_condition, student_name, task_name);
    if (check_condition[student_name].find(task_name) != check_condition[student_name].end()) {
        AddToTable(score_table_, student_name, task_name);
    }
    DelStudents(score_table_);
}
void Scorer::Reset() {
    score_table_.clear();
    check_condition.clear();
    merge_condition.clear();
}
ScoreTable Scorer::GetScoreTable() const {
    return score_table_;
}