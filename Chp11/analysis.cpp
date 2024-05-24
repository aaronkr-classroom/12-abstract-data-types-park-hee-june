// analysis.cpp

#include <algorithm>
#include <numeric> // accumulate() 함수

#include "grade.h"
#include "median.h"
#include "Student_info.h"
#include "Vec.h" //Vec 대신 Vec

bool did_all_hw(const Student_info& s) {
    return ((find(s.homework.begin(), s.homework.end(), 0)) == s.homework.end());
}

double grade_aux(const Student_info& s) {
    try {
        return grade(s);
    }
    catch (domain_error) {
        return grade(s.midterm, s.final, 0);
    }
}

// 이 함수는 제대로 동작하지 않음:
double median_analysis(const Vec<Student_info>& students) {
    Vec<double> grades;

   transform(students.begin(), students.end(), 
        back_inserter(grades), grade_aux); // grades대신 grade_aux 함수 사용

    return median(grades);
}

// s.homework 벡터에서 0이 아닌 요소들의 중앙값을 구합니다.
// 0이 아닌 요소가 없다면 종합 과제 점수를 0점으로 처리
double optimistic_median(const Student_info& s) {
    Vec<double> nonzero;
    remove_copy(s.homework.begin(), s.homework.end(), 
        back_inserter(nonzero), 0);

    if (nonzero.empty())
        return grade(s.midterm, s.final, 0);
    else
        return grade(s.midterm, s.final, median(nonzero));
}

double optimistic_median_analysis(const Vec<Student_info>& students) {
    Vec<double> grades;

    transform(students.begin(), students.end(),
        back_inserter(grades), optimistic_median);

    return median(grades);
}

double average(const Vec<double>& v) {
    return accumulate(v.begin(), v.end(), 0.0) / v.size();
}

double average_grade(const Student_info& s) {
    return grade(s.midterm, s.final, average(s.homework));
}

double average_analysis(const Vec<Student_info>& students) {
    Vec<double> grades;

    transform(students.begin(), students.end(), 
        back_inserter(grades), average_grade);
    
    return median(grades);
}

void write_analysis(ostream& out, const string& name,
    double analysis(const Vec<Student_info>&),
    const Vec<Student_info>& did,
    const Vec<Student_info>& didnt) {

    out << name << ": median(did) = " << analysis(did)
        << ", median(didnt) = " << analysis(didnt) << endl;
}
