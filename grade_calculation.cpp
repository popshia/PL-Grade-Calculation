// include libraries
#include <cstdio>
#include <cwctype>
#include <exception>
#include <fstream>
#include <iostream>
#include <math.h>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct Record {
    string pid = "\0";
    string weekNo = "\0";
    string correctOrNot = "\0";
}; // struct Record

struct Student {
    string id = "\0";
    int q1Correct = 0;
    int q2Correct = 0;
    int q3Correct = 0;
    int q4Correct = 0;
    float grade;
}; // struct student

void Read_Useless_label( ifstream & file ) {
    string label = "\0";
    for ( int i = 0; i < 25; i++ ) {
        getline( file, label );
    } // loop the labels
} // Read_Useless_label()

void Count_And_Add_Points( Student & student, Record record ) {
    if ( 896201 <= stoi( record.pid ) && stoi( record.pid ) <= 896212 ) {
        student.q1Correct += 1;
    } // if: q1Correct
    else if ( 896221 <= stoi( record.pid ) && stoi( record.pid ) <= 896234 ) {
        student.q2Correct += 1;
    } // else if: q2Correct
    else if ( 896241 <= stoi( record.pid ) && stoi( record.pid ) <= 896256 ) {
        student.q3Correct += 1;
    } // else if: q3Correct
    else if ( 896261 <= stoi( record.pid ) && stoi( record.pid ) <= 896272 ) {
        student.q4Correct += 1;
    } // else if: q4Correct
} // Count_And_Add_Points()

void RoundUpGrades( Student & student ) {
    if ( student.q4Correct == 12 ) {
        student.grade = 120;
    } // if: pass q4

    else if ( student.q3Correct == 16 ) {
        student.grade = 95 + student.q4Correct * 2.083;
    } // if: pass q3

    else if ( student.q2Correct == 14 ) {
        student.grade =
            50 + student.q3Correct * 2.812 + student.q4Correct * 2.083;
    } // if: pass q2

    else if ( student.q1Correct == 12 ) {
        student.grade = 20 + student.q2Correct * 2.142 +
                        student.q3Correct * 2.812 + student.q4Correct * 2.083;
    } // if: pass q2

    else {
        student.grade = student.q1Correct * 1.666 + student.q2Correct * 2.142 +
                        student.q3Correct * 2.812 + student.q4Correct * 2.083;
    } // else: didn't all pass any project

    string::size_type location = student.id.find( "PL110_" );
    student.id.erase( location, 6 ); // erase PL110_ from the id

    student.grade = roundf( student.grade * 10 ) / 10; // round grades
} // RoundUpGrades()

vector<Student> Read_And_Calculate_Grades( ifstream & file ) {
    vector<Student> studentList;
    Student student = Student();
    Record record = Record();
    string singleLine, cutValue;
    getline( file, singleLine );
    string currentID = "\0";

    do {
        istringstream currentStudent( singleLine );
        getline( currentStudent, cutValue, '\t' );

        if ( currentID == "\0" ) {
            currentID = cutValue;
            student.id = currentID;
        } // if: set init currentID

        if ( currentID != cutValue ) {
            RoundUpGrades( student );
            studentList.push_back( student );
            student = Student();
            currentID = cutValue;
            student.id = currentID;
        } // if: different student

        for ( int i = 1; getline( currentStudent, cutValue, '\t' ); i++ ) {
            if ( i == 2 ) {
                record.weekNo = cutValue;
            } // weekNo
            else if ( i == 3 ) {
                record.pid = cutValue;
            } // PID
            else if ( i == 4 ) {
                if ( cutValue == "Y" ) Count_And_Add_Points( student, record );
            } // CorrectOrNot
        } // for: loop each value
    } while ( getline( file, singleLine ) );

    return studentList;
} // Read_Student_Record()

void Output_Student_Grades( vector<Student> allStudents ) {
    ofstream output;
    output.open( "110-3-FinalGrades.txt" );

    for ( int i = 0; i < allStudents.size(); i++ ) {
        output << "id: " << allStudents[i].id;
        if ( allStudents[i].id.size() < 8 ) output << "\t\t\t";
        else output << "\t\t";

        output << "\t\tq1: " << allStudents[i].q1Correct
               << "\t\tq2: " << allStudents[i].q2Correct
               << "\t\tq3: " << allStudents[i].q3Correct
               << "\t\tq4: " << allStudents[i].q4Correct
               << "\t\tgrades: " << allStudents[i].grade << endl;
    } // for: output student grades

    output.close();
} // Output_Student_Grades()

int main() {
    ifstream file;
    vector<string> grades;
    file.open( "PL110-3-AnsweringRecords.txt" );

    if ( file.is_open() ) {
        Read_Useless_label( file );
        Output_Student_Grades( Read_And_Calculate_Grades( file ) );
    } // if: open file

    file.close();
    return 0;
} // main()
