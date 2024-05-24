#pragma once
// grade.h

#ifndef GUARD_grade_h
#define GUARD_grade_h

#include <list>

#include "Student_info.h"

bool fgrade(const Student_info&);
bool pgrade(const Student_info&);

double grade(double, double, double);
double grade(double, double, Vec<double>&);
double grade(const Student_info&);

list<Student_info> extract_fails(list<Student_info>&);

#endif