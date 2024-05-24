// main.cpp

// 중간고사, 기말고사, 그리고 과제 점수 몇개를 받고
// 마지막 평균 결과 점수를 계산하기.
#include <algorithm>
#include <fstream> // 에런 (파일을 사용하기 위해)
#include <ios>
#include <iomanip>
#include <iostream> // cin/cout을 사용하기 위해
#include <stdexcept>
#include <string>


#include "analysis.h"
#include "grade.h"
#include "util.h"
#include "Student_info.h"
#include "Vec.h"

using namespace std;

int main() {
    /*
    // 파일스트림 만들고 "txt" 파일을 읽기
    ifstream student_file("grades.txt"); // 에런
    ifstream student_file_did("grades_did.txt"); // 에런
    ifstream student_file_didnt("grades_didnt.txt"); // 에런

    // 6장: 모든 과제를 제출한 학생과 그렇지 않은 학생
    Vec<Student_info> students, did, didnt;
    Student_info record;
    string::size_type maxlen = 0;

    // 학생 이름과 모든 점수를 읽어 저장하고
    // 가장 긴 이금을 찾음.
    while (read(student_file, record)) { // 에런
        // cin을 사용하면 직접 사용자 입력만 받을 수 있다
        // student_file은 ifstream이어서 파일 입력 받을 수 있다

        maxlen = max(maxlen, record.name.size());
        // Aaron: max(0, 5) = 5
        students.push_back(record);

        if (did_all_hw(record)) {
            did.push_back(record);
        }
        else {
            didnt.push_back(record);
        }
    }
    */

    int fileChoice = printGreeting();
    // cout << fileChoice << typeid(fileChoice).name();

    Vec<Student_info> students, did, didnt;
    string::size_type maxlen = 0;

    fileSelect(fileChoice, students, did, didnt, maxlen);

    // loadGrades(student_file, students, did, didnt, maxlen);

    // cout << "Welcome to Student Gradebook!" << endl;
    // cout << "Read " << students.size() << " records." << endl << endl;

    bool quit = false;
    int choice;

    while (!quit) {
        printMenu();

        // Read menu choice
        cin >> choice;

        // 과제 점수를 일음
        switch (choice) {
        case 1: // Check Homework
            checkHw(did, didnt);
            break;
        case 2: // Analyze Grades
            analyzeGrades(did, didnt);
            break;
        case 3: // Show Final Scores
            showScores(students, maxlen);
            break;
        case 4:
            fileChoice = printGreeting();
            fileSelect(fileChoice, students, did, didnt, maxlen);
            break;
        case 0: // Quit
            quit = true;
            break;
        default:
            cout << "Incorrect input. Enter a choice between 1, 2, 3, or 0: " << endl;
        }
    }

    return 0;
} // main 끝