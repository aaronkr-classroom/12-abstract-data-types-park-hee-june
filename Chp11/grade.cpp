// grade.cpp

#include <algorithm>
#include <list>

#include <stdexcept>

#include "grade.h"
#include "median.h"
#include "Student_info.h"
#include "Vec.h"

// 학생의 과락 여부를 결정하는 서술 함수
bool fgrade(const Student_info& s) {
    return grade(s) < 60;
}

bool pgrade(const Student_info& s) {
    return !fgrade(s);
}

// 중간시험 점수, 기말시험 점수, 종합 과제 점수에서
// 학생의 종합 점수를 가함
double grade(double mt, double ft, double hw) {
    return 0.2 * mt + 0.4 * ft + 0.4 * hw;
}

// 중간시험 점수, 기말시험 점수, 과제 점수의 백터로
// 학생의 종합 점수를 가함.
// 이 함수는 인수를 복사하기 않고 median 함수가 해당 작업을 실행.
double grade(double mt, double ft, const Vec<double>& hw) {
    if (hw.size() == 0)
        throw domain_error("No homework!");

    return grade(mt, ft, median(hw));
}

double grade(const Student_info& s) {
    return grade(s.midterm, s.final, s.homework);
}

// 세 번째 버젼:
// 인덱스 대신 반복자를 사용하지만 여전히
// 성증 저하가 우려됨
list<Student_info> extract_fails(list<Student_info>& students) {
    list<Student_info> fail;
    list<Student_info>::iterator iter = students.begin();

    while (iter != students.end()) {
        if (fgrade(*iter)) {
            fail.push_back(*iter); // 역참조 = 반복자의 값
            iter = students.erase(iter); // 반복자의 메모리 위치 삭제
        }
        else {
            ++iter;
        }
    }
    return fail;
}
