#include "scorer.h"

using ScoreTable = std::map<StudentName, std::set<TaskName>>;
ScoreTable score_table;
std::map<std::pair<StudentName, TaskName>, Scorer::EventType> merge;
std::map<std::pair<StudentName, TaskName>, Scorer::EventType> check;
void Scorer::DelStudents(const StudentName& student_name) {
    if (score_table[student_name].empty()) {
        score_table.erase(student_name);
    }
}
void Scorer::Checking(const StudentName& student_name, const TaskName& task_name) {
    Scorer::EventType merge_condition = merge[std::make_pair(student_name, task_name)];
    Scorer::EventType check_condition = merge[std::make_pair(student_name, task_name)];
    if (merge_condition == Scorer::EventType::MergeRequestOpen || check_condition == Scorer::EventType::CheckFailed) {
        score_table[student_name].erase(task_name);
        DelStudents(student_name);
    } else {
        score_table[student_name].insert(task_name);
    }
}
void Scorer::OnCheckFailed(const StudentName& student_name, const TaskName& task_name) {
    Scorer::EventType check_condition = check[std::make_pair(student_name, task_name)];
    check_condition = Scorer::EventType::CheckFailed;
    Checking(student_name, task_name);
}
void Scorer::OnCheckSuccess(const StudentName& student_name, const TaskName& task_name) {
    Scorer::EventType check_condition = check[std::make_pair(student_name, task_name)];
    check_condition = Scorer::EventType::CheckSuccess;
    Checking(student_name, task_name);
}
void Scorer::OnMergeRequestOpen(const StudentName& student_name, const TaskName& task_name) {
    Scorer::EventType merge_condition = merge[std::make_pair(student_name, task_name)];
    merge_condition = Scorer::EventType::MergeRequestOpen;
    Checking(student_name, task_name);
}
void Scorer::OnMergeRequestClosed(const StudentName& student_name, const TaskName& task_name) {
    Scorer::EventType merge_condition = merge[std::make_pair(student_name, task_name)];
    merge_condition = Scorer::EventType::MergeRequestClosed;
    Checking(student_name, task_name);
}
void Scorer::Reset() {
    score_table.clear();
}
ScoreTable Scorer::GetScoreTable() const {
    return score_table;
}