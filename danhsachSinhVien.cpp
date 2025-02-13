#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cmath>
using namespace std;


// test



struct DayofBirth
{
	int day;
	int month;
	int year;
};

struct Student
{
	string id;
	string name;
	DayofBirth dayofbirth;
	bool gender;
	double grade;
};

void addStudent(vector<Student>& student)
{
	Student newstudent;
	cout << "ID:";
	cin >> newstudent.id;
	cin.ignore();

	cout << "Name:";
	getline(cin,newstudent.name);

	cout << "Select gender (male->0, female->1):";
	cin >> newstudent.gender;

	cout << "Select day of birth" << endl;
	cout << "Day:";
	cin >> newstudent.dayofbirth.day;
	cout << "Month:";
	cin >> newstudent.dayofbirth.month;
	cout << "Year:";
	cin >> newstudent.dayofbirth.year;

	cout << "Grade:";
	cin >> newstudent.grade;

	student.push_back(newstudent);
	cout << endl;
}

void loadFromFile(vector<Student>& student, string filename)
{
	ifstream extractFromFile(filename);
	if(!extractFromFile)
	{
		cout << "File does not exist" << endl;
		return;
	}
	string line;
	Student newstudent;
	int pos;
	while(getline(extractFromFile,line))
	{
		if(line.empty())
			continue;
		pos=(int)line.find(',');
		newstudent.id=line.substr(0,pos);
		line.erase(0,pos+1);

		pos=(int)line.find(',');
		newstudent.name=line.substr(0,pos);
		line.erase(0,pos+1);

		pos=(int)line.find(',');
		newstudent.gender=(bool)(stoi(line.substr(0,pos)));
		line.erase(0,pos+1);

		pos=(int)line.find('/');
		newstudent.dayofbirth.day=stoi(line.substr(0,pos));
		line.erase(0,pos+1);

		pos=(int)line.find('/');
		newstudent.dayofbirth.month=stoi(line.substr(0,pos));
		line.erase(0,pos+1);

		pos=(int)line.find(',');
		newstudent.dayofbirth.year=stoi(line.substr(0,pos));
		line.erase(0,pos+1);

		pos=(int)line.find('#');
		newstudent.grade=stod(line.substr(0,pos));
		line.erase(0,pos+1);

		student.push_back(newstudent);
	}
	extractFromFile.close();
}

void turnToString(vector<vector<string>>& student_str, vector<Student> student)
{
	int numberOfStudents=student.size();
	for(int i=0; i<numberOfStudents; i++)
	{
		student_str[i].push_back(student[i].name);
		student_str[i].push_back(student[i].id);
		if(student[i].gender)
			student_str[i].push_back("Female");
		else
			student_str[i].push_back("Male");
		string dmy_str=to_string(student[i].dayofbirth.day)+"/"+to_string(student[i].dayofbirth.month)+"/"+to_string(student[i].dayofbirth.year);
		student_str[i].push_back(dmy_str);
		double grade_prev=student[i].grade;
		stringstream ss;
		ss << fixed << setprecision(2) << grade_prev;
		string grade_rounded;
		ss >> grade_rounded;
		student_str[i].push_back(grade_rounded);
	}
}

void centerAligned(string text, int width)
{
    int text_size=text.size();
    if(text_size>width)
    {
        string text_left=text.substr(0,width);
        centerAligned(text_left,width);
        return;
    }
    int pad1=(width-text_size)/2;
    int pad2=width-pad1-text_size;
    cout << "|" << string(pad1,' ') << text << string(pad2,' ') << "|";
}

void printList(string filename)
{
	vector<Student> student;
	loadFromFile(student,filename);
	vector<vector<string>> student_str(student.size());
	if(student.size()==0)
	{
		cout << "No data is in the file" << endl << endl;;
		return;
	}
	turnToString(student_str,student);
	cout << string(97,'=') << endl;
	cout << "|";
	centerAligned("Name",25);
	centerAligned("ID",15);
	centerAligned("Gender",15);
	centerAligned("Day of birth",20);
	centerAligned("Grade",10);
	cout << "|" << endl;
	cout << string(97,'-') << endl;

	for(int i=0; i<student_str.size(); i++)
	{
		cout << "|";
		centerAligned(student_str[i][0],25);
		centerAligned(student_str[i][1],15);
		centerAligned(student_str[i][2],15);
		centerAligned(student_str[i][3],20);
		centerAligned(student_str[i][4],10);
		cout << "|" << endl;
		if(i<student_str.size()-1)
			cout << string(97,'-') << endl;
	}
	cout << string(97,'=') << endl << endl;
}

void saveData(vector<Student>& student, string filename)
{
	ios::openmode mode=(filename=="search.txt") ? ios::out : ios::app;
	ofstream savetoFile(filename,mode);
	int numberOfStudents=student.size();
	if(numberOfStudents==0)
	{
		cout << "No data is being saved" << endl;
		return;
	}
	for(int i=0; i<numberOfStudents; i++)
	{
		savetoFile << student[i].id << ",";
		savetoFile << student[i].name << ",";
		savetoFile << student[i].gender << ",";
		savetoFile << student[i].dayofbirth.day << "/";
		savetoFile << student[i].dayofbirth.month << "/";
		savetoFile << student[i].dayofbirth.year << ",";
		savetoFile << student[i].grade << "#" << '\n';
	}
	savetoFile.close();
	student.clear();
	cout << "Datas have been saved" << endl << endl;
}

bool find_substring(string wanted_name, string name)
{
	if(wanted_name.size()>name.size())
		return false;
	for(int i=0; i<name.size()-wanted_name.size(); i++)
	{
		if(wanted_name==name.substr(i,wanted_name.size()))
			return true;
	}
	return false;
}

void searchStudent(string filename)
{
	vector<Student> found_student; 
	vector<Student> search_student;
	loadFromFile(search_student,filename);
	cout << "1:ID" << endl;
	cout << "2:Name" << endl;
	cout << "3:Gender" << endl;
	cout << "4:Grade" << endl;
	int choice;
	cout << "Enter:";
	cin >> choice;
	if(choice==1)
	{
		string wanted_id;
		cout << "Enter ID:";
		cin >> wanted_id;
		for(int i=0; i<search_student.size(); i++)
		{
			if(search_student[i].id==wanted_id)
				found_student.push_back(search_student[i]);
		}
	}
	else if(choice==2)
	{
		string wanted_name;
		cout << "Enter name:";
		cin >> wanted_name;
		for(int i=0; i<search_student.size(); i++)
		{
			if(find_substring(wanted_name,search_student[i].name))
				found_student.push_back(search_student[i]);
		}
	}
	else if(choice==3)
	{
		bool wanted_gender;
		cout << "Select gender (male->0, female->1):";
		cin >> wanted_gender;
		for(int i=0; i<search_student.size(); i++)
		{
			if(search_student[i].gender==wanted_gender)
				found_student.push_back(search_student[i]);
		}
	}
	else if(choice==4)
	{
		int choice_grade;
		double wanted_grade;
		double wanted_grade_rounded;
		cout << "1:Exact" << endl << "2:Lower" << endl << "3:Higher" << endl;
		cout << "Enter:";
		cin >> choice_grade;
		cout << "Enter grade:";
		cin >> wanted_grade;

		stringstream ss;
		ss << fixed << setprecision(2) << wanted_grade;
		ss >> wanted_grade_rounded;

		if(choice_grade==1)
		{
			for(int i=0; i<search_student.size(); i++)
			{
				if(wanted_grade_rounded==search_student[i].grade)
					found_student.push_back(search_student[i]);
			}
		}
		else if(choice_grade==2)
		{
			for(int i=0; i<search_student.size(); i++)
			{
				if(wanted_grade_rounded>search_student[i].grade)
					found_student.push_back(search_student[i]);
			}
		}
		else if(choice_grade==3)
		{
			for(int i=0; i<search_student.size(); i++)
			{
				if(wanted_grade_rounded<search_student[i].grade)
					found_student.push_back(search_student[i]);
			}
		}
	}
	if(found_student.size()==0)
	{
		cout << "That student does not exist in the file" << endl;
		return;
	}
	saveData(found_student,"search.txt");
	printList("search.txt");
}

int main()
{
	vector<Student> students;
	string filename="danhsachSinhVien.txt";
	do
	{
		system("cls");
		cout << "1:Print" << endl;
		cout << "2:Add" << endl;
		cout << "3:Fix" << endl;
		cout << "4:Delete" << endl;
		cout << "5:Search" << endl;
		cout << "6:Sorting" << endl;
		cout << "7:Analyze" << endl;
		cout << "8:Save" << endl;
		cout << "9:Change saving file" << endl;
		cout << "10:Exit" << endl;
		cout << endl;
		int choice;
		cout << "Enter:";
		cin >> choice;

		switch(choice)
		{
			case 1:
				printList(filename);
				break;
			case 2:
				addStudent(students);
				break;
			case 5:
				searchStudent(filename);
				break;
			case 8:
				saveData(students,filename);
				break;	
			case 9:
				cout << "Enter new txt file name:";
				cin >> filename;
				break;
			case 10:
				return 0;
			default:
				cout << "Invalid choice" << endl << endl;
		}
		cout << "Press any key to continue";
		cin.ignore();
		cin.get();
	}while(true);
}
