#include "scorer.h"

using ScoreTable = std::map<StudentName, std::set<TaskName>>;
ScoreTable score_table;
std::map<std::pair<StudentName, TaskName>, Scorer::EventType> merge;
std::map<std::pair<StudentName, TaskName>, Scorer::EventType> check;
std::set<TaskName> set;
void Scorer::DelStudents(const StudentName& student_name) {
    if (score_table[student_name].empty()) {
        score_table.erase(student_name);
    }
}
void Scorer::DelFromTable(const StudentName& student_name, const TaskName& task_name) {
    score_table[student_name].erase(task_name);
    DelStudents(student_name);
}
void Scorer::AddToTable(const StudentName& student_name, const TaskName& task_name) {
    if (score_table.find(student_name) == score_table.end()) {
        set.insert(task_name);
        score_table.insert({student_name, set});
    } else {
        score_table[student_name].insert(task_name);
    }
    set.clear();
}
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