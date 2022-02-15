#include "scorer.h"

using ScoreTable = std::map<StudentName, std::set<TaskName>>;
ScoreTable score_table;
std::map<std::pair<StudentName, TaskName>, EventType> merge;
std::map<std::pair<StudentName, TaskName>, EventType> check;
void DelStudents(const StudentName& student_name) {
    if (score_table[student_name].empty()) {
        score_table.erase(student_name);
    }
}
void Checking(const StudentName& student_name, const TaskName& task_name) {
    EventType merge_condition = merge[std::make_pair(student_name, task_name)];
    EventType check_condition = merge[std::make_pair(student_name, task_name)];
    if (merge_condition == EventType::MergeRequestOpen || check_condition == EventType::CheckFailed) {
        score_table[student_name].erase(task_name);
        DelStudents(student_name);
    } else {
        score_table[student_name].insert(task_name);
    }
}
void Scorer::OnCheckFailed(const StudentName& student_name, const TaskName& task_name) {
    EventType check_condition = merge[std::make_pair(student_name, task_name)];
    check_condition = EventType::CheckFailed;
    Checking(student_name, task_name);
}
void Scorer::OnCheckSuccess(const StudentName& student_name, const TaskName& task_name) {
    EventType check_condition = merge[std::make_pair(student_name, task_name)];
    check_condition = EventType::CheckSuccess;
    Checking(student_name, task_name);
}
void Scorer::OnMergeRequestOpen(const StudentName& student_name, const TaskName& task_name) {
    EventType merge_condition = merge[std::make_pair(student_name, task_name)];
    merge_condition = EventType::MergeRequestOpen;
    Checking(student_name, task_name);
}
void Scorer::OnMergeRequestClosed(const StudentName& student_name, const TaskName& task_name) {
    EventType merge_condition = merge[std::make_pair(student_name, task_name)];
    merge_condition = EventType::MergeRequestClosed;
    Checking(student_name, task_name);
}
void Scorer::Reset() {
    score_table.clear();
}
ScoreTable Scorer::GetScoreTable() const {
    return score_table;
}