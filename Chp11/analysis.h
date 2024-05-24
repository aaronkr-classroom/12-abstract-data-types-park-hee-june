// analysis.h

#ifndef GUARD_analysis_h
#define GUARD_analysis_h

#include "grade.h"
#include "median.h"
#include "Student_info.h"

bool did_all_hw(const Student_info&);
double grade_aux(const Student_info&);

double median_analysis(const Vec<Student_info>&);
double optimistic_median(const Student_info&);
double optimistic_median_analysis(const Vec<Student_info>&);

double average(const Vec<double>&);
double average_grade(const Student_info&);
double average_analysis(const Vec<Student_info>&);

void write_analysis(ostream&, const string&,
    double analysis(const Vec<Student_info>&),
    const Vec<Student_info>&,
    const Vec<Student_info>&);

#endif
