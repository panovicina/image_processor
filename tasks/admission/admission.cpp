#include <algorithm>
#include <tuple>

bool CompApp(const Applicant& a, const Applicant& b) {
    return std::tie(a.points, b.student.birth_date.year, b.student.birth_date.month, b.student.birth_date.day,
                    b.student.name) > std::tie(b.points, a.student.birth_date.year, a.student.birth_date.month,
                                               a.student.birth_date.day, a.student.name);
}
AdmissionTable FillUniversities(const std::vector<University>& universities, const std::vector<Applicant>& applicants) {
    std::vector<Applicant> copy_applicants = applicants;
    std::sort(copy_applicants.begin(), copy_applicants.end(), CompApp);
    std::unordered_map<std::string, int> applicant_indx;
    for (size_t i = 0; i < applicants.size(); ++i) {
        applicant_indx[applicants[i].student.name] = i;
    }
    AdmissionTable result_list;
    for (size_t i = 0; i < copy_applicants.size(); ++i) {
        bool enter_uni = false;
        for (size_t j = 0; j < copy_applicants[i].wish_list.size(); ++j) {
            if (!enter_uni) {
                for (size_t k = 0; k < universities.size(); ++k) {
                    if (result_list[universities[k].name].size() < universities[k].max_students &&
                        universities[k].name == copy_applicants[i].wish_list[j]) {
                        result_list[copy_applicants[i].wish_list[j]].push_back(&applicants[applicant_indx[copy_applicants[i].student.name]].student);
                        enter_uni = true;
                    }
                }
            }
        }
    }
    return result_list;
}