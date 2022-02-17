#include "scorer.h"

using ScoreTable = std::map<StudentName, std::set<TaskName>>;
ScoreTable score_table;
void Scorer::OnCheckFailed(const StudentName& student_name, const TaskName& task_name) {
    DelFromTable(student_name, task_name);
}
void Scorer::OnCheckSuccess(const StudentName& student_name, const TaskName& task_name) {
    AddToTable(student_name, task_name);
}
void Scorer::OnMergeRequestOpen(const StudentName& student_name, const TaskName& task_name) {
    DelFromTable(student_name, task_name);
}
void Scorer::OnMergeRequestClosed(const StudentName& student_name, const TaskName& task_name) {
    AddToTable(student_name, task_name);
}
void Scorer::Reset() {
    score_table.clear();
}
ScoreTable Scorer::GetScoreTable() const {
    return score_table;
}