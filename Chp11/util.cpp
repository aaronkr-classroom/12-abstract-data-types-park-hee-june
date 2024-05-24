// util.cpp

#include <algorithm>
#include <fstream> // 에런 (파일을 사용하기 위해)
#include <iomanip>
#include <iostream>

#include "Vec.h"
#include "analysis.h"
#include "Student_info.h"

using namespace std;

int printGreeting() {
	cout << "Welcome to Student Gradebook!" << endl;
	cout << "==============================" << endl << endl;
	cout << "Which file would you like to load?" << endl;
	cout << "1. Original grades.txt (20 students)" << endl;
	cout << "2. All homework grades_did.txt (24 students)" << endl;
	cout << "3. No homework grades_didnt.txt (18 students)" << endl;
	cout << "0. Quit" << endl;
	cout << "Selection: ";

	int choice;
	while (cin >> choice) {
		if (choice != 0 && choice != 1 && choice != 2 && choice != 3) {
			cout << "Invalid selection. Choose 1, 2, 3, or 0 to quit: ";
		}
		else {
			return choice;
		}
	}

	cout << endl;
}

void printMenu() {
	cout << "------------------------------" << endl
		<< "1. Check homework" << endl
		<< "2. Analyze Grades" << endl
		<< "3. Show Final Scores" << endl
		<< "4. Return to File Selection" << endl
		<< "0. Quit" << endl
		<< "------------------------------" << endl
		<< "Choice: ";
}

void loadGrades(ifstream& student_file, 
	Vec<Student_info>& students,
	Vec<Student_info>& did, 
	Vec<Student_info>& didnt,
	string::size_type& maxlen) {

	Student_info record;

	// 학생 이름과 모든 점수를 읽어 저장하고
	// 가장 긴 이금을 찾음.
	while (read(student_file, record)) { // 에런
		// cin을 사용하면 직접 사용자 입력만 받을 수 있다
		// student_file은 ifstream이어서 파일 입력 받을 수 있다

		maxlen = max(maxlen, record.name.size()); // Aaron: max(0, 5) = 5
		students.push_back(record);

		if (did_all_hw(record)) {
			did.push_back(record);
		}
		else {
			didnt.push_back(record);
		}
	}

	cout << "Read " << students.size() << " records." << endl << endl;
}

void fileSelect(int fileChoice,
	Vec<Student_info>& students,
	Vec<Student_info>& did,
	Vec<Student_info>& didnt,
	string::size_type& maxlen
) {
	// Clear all Vecs to start from scratch
	students.clear();
	did.clear();
	didnt.clear();

	if (fileChoice == 1) {
		ifstream student_file("grades.txt"); // 에런
		loadGrades(student_file, students, did, didnt, maxlen);
	}
	else if (fileChoice == 2) {
		ifstream student_file("grades_did.txt"); // 에런
		loadGrades(student_file, students, did, didnt, maxlen);
	}
	else if (fileChoice == 3) {
		ifstream student_file("grades_didnt.txt"); // 에런
		loadGrades(student_file, students, did, didnt, maxlen);
	}
	else
		cout << "Invalid file choice.";
}

int checkHw(Vec<Student_info>& did, Vec<Student_info>& didnt) {
	cout << endl;

	// 6장: 두 집단에 데이터가 있는지 각각 확인하여 분석할 필요가 있는지 확인
	if (did.empty()) {
		cout << "No student did all the homework!" << endl;
		return 1; // exit early, error code
	}
	if (didnt.empty()) {
		cout << "Every student did all the homework!" << endl;
		return 1; // exit early, error code
	}
	cout << did.size() << " students did all the homework." << endl;
	cout << didnt.size() << " students didn't do all the homework." << endl;
	
	cout << endl;
}

void write_analysis(ostream& out, const string& name,
	double analysis(const Vec<Student_info>&),
	const Vec<Student_info>& students, 
	string vec) {

	out << name << ": median(" << vec << ") = " << analysis(students) << endl;
}

void analyzeGrades(Vec<Student_info>& did, Vec<Student_info>& didnt) {
	cout << endl;

	if (did.empty()) {
		write_analysis(cout, "average", average_analysis, didnt, "didnt");
		write_analysis(cout, "median", median_analysis, didnt, "didnt");
		write_analysis(cout, "median of homework turned in",
			optimistic_median_analysis, didnt, "didnt");
		cout << endl;
		return;
	}

	if (didnt.empty()) {
		write_analysis(cout, "average", average_analysis, did, "did");
		write_analysis(cout, "median", median_analysis, did, "did");
		write_analysis(cout, "median of homework turned in",
			optimistic_median_analysis, did, "did");
		cout << endl;
		return;
	}

	// 6장: 분석 실행
	write_analysis(cout, "average", average_analysis, did, didnt);
	write_analysis(cout, "median", median_analysis, did, didnt);
	write_analysis(cout, "median of homework turned in",
		optimistic_median_analysis, did, didnt);

	cout << endl;
}


void showScores(Vec<Student_info>& students, string::size_type& maxlen) {
	cout << endl;

	// 학생 정보를 알파벳순으로 정렬
	sort(students.begin(), students.end(), compare);

	for (Vec<Student_info>::size_type i = 0;
		i != students.size(); ++i) {
		// 이름과 오른쪽 공백을 포함하여 maxlen + 1개의
		// 문자를 출력
		cout << students[i].name
			<< string(maxlen + 1 - students[i].name.size(), ' ');

		// 종합 점수를 계산해 생성
		try {
			double final_grade = grade(students[i]);
			// 결과를 출력
			streamsize prec = cout.precision(); // 지금 cout precision
			cout << "Final grade: " << setprecision(3)
				<< final_grade << setprecision(prec) << endl;
		}
		catch (domain_error e) {
			cout << e.what();
		}
	} // for 끝

	cout << endl;
}
