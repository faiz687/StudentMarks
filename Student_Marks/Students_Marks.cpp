# include <iostream>
# include <string>
# include <array>
# include <stdexcept>
# include <vector>
# include <iterator>
# include <algorithm>
# include <iomanip>
# include "libsqlite.hpp"

using namespace std;
string students_database = "Student_Marks.sqlite3";
sqlite::sqlite db(students_database);

template<typename T>
T input()
/* Taken from week 6 lab.adaptive script . This function takes an input and 
checks if the deisred input is of the correct type or not entered by the user */
{
    string buffer;
    getline(cin, buffer);

    stringstream ss(buffer);
    T input;

    if( ss >> input /*&& ss.eof() */ )
        return input;

    throw runtime_error("Input is of invalid type");
}
int Over_all_marks_calc(int phase_test1 , int phase_test2 , int course_work)
/*This function calculates the overall marks of a student it takes three (int) parameters
and returns the overall marks (int). */
{ 
  int total_1 = (phase_test1 + phase_test2)*0.20;
  int total_2 = (course_work * 0.60);
  float overall_marks = total_2 + total_1; 
  return overall_marks;
}
void View_records()
  /* This function helps us in viewing the database records */
{ 
  try
  {
  int option ; 
  
      cout << " " << endl;
      cout << "1.View All Students Marks  " << endl ;
      cout << "2.View Individual Students Marks " << endl;
      cout << "" << endl;
      cout << "Select An Option : " ;
      cin >> option; 
      cout << " " << endl;
      if (option == 1)
        
        {
          auto view_all  = db.get_statement();
     
          view_all -> set_sql ("SELECT * FROM  student_Marks "
                             "ORDER BY  Overall_marks DESC");
        
          view_all -> prepare();
     
          cout << "Student ID  |   Student Name   |   PT1     PT2     CW   |   Overall Marks " <<  endl ;
          cout << "" << endl;
          while ( view_all -> step() )
              { 
                  int ID = view_all -> get_int(0);
    
                  string name = view_all -> get_text(1);
    
                  int phase_test1 = view_all -> get_int(2);
    
                  int phase_test2 = view_all -> get_int(3);
    
                  int course_work = view_all -> get_int(4);
    
                  int overall_marks = view_all -> get_int(5);
    
                  cout << setw(6)  << right <<ID <<  setw(22) << right  << name << setw(10) <<  phase_test1 << setw(8) <<   phase_test2 << setw(8) << course_work << setw(10) << overall_marks << endl;
              }
        }  
      if (option == 2)   
        
        {
          cout << "Enter Student ID : " ; 
          int student_id ;
        
          cin >> student_id ;
      
          auto view_individual = db.get_statement();
     
          view_individual -> set_sql ("SELECT Student_ID , student_name , PT1 , PT2 , CW , Overall_marks "
                                 "FROM student_Marks WHERE Student_ID = ?");
     
          view_individual -> prepare();
     
          view_individual -> bind(1,student_id);
       
          view_individual -> step(); 
     
          int ID = view_individual -> get_int(0);
          
          string name = view_individual -> get_text(1);
       
          int phase_test1 = view_individual -> get_int(2);
       
          int phase_test2 = view_individual -> get_int(3);
       
          int course_work = view_individual -> get_int(4);
       
          int overall_marks = view_individual -> get_int(5);
          cout << "Student ID  |   Student Name   |   PT1     PT2     CW   |   Overall Marks " <<  endl ;
          cout << setw(5)  << right <<ID <<  setw(26) << right  << name << setw(6) <<  phase_test1 << setw(8) <<   phase_test2 << setw(8) << course_work << setw(10) << overall_marks << endl;
        } 
  
  if (option != 1 || option != 2)
  {
    cout << setw(45) << "**INVALID OPTIOIN**" << endl;
  }
  }
  catch (logic_error)
  {
    cerr << setw(45) << "**STUDENT ID DOES NOT EXIST**" << endl; 
  }
}
void update_overall(int pt1,int pt2,int  cw , int student_id)
/*This function updates the overall marks on the databse , this function is 
is called from the Edit records function , it takes the 4 (int) parameters and 
updates the overall marks for the particular student */
{
   
  int new_overall_marks; 
    
  new_overall_marks = Over_all_marks_calc(pt1,pt2,cw);
    
  cout << "these are your new over_all marks: " << new_overall_marks << endl;  
                      
  auto editing_overall_marks = db.get_statement();
            
  editing_overall_marks -> set_sql ("UPDATE student_Marks "
                                    "SET Overall_marks = ? "
                                    "WHERE student_ID = ? ;" );
            
  editing_overall_marks -> prepare();
            
  editing_overall_marks -> bind(1,new_overall_marks);
            
  editing_overall_marks -> bind(2,student_id);
            
  editing_overall_marks -> step();
  
}
int Edit_records()
/*This function Edits the records on the database Name , Pt1 , Pt2 , CW , 
and overall_marks */
{

  
  cout << "MODIFYING STUDENTS DETAILS..."  << endl;
  cout << "" << endl;
  
  auto cur = db.get_statement();
  int student_id ,  option , phase_test1 , phase_test2 , course_work , overall_marks,
  new_phase_test_1,new_phase_test_2,new_course_work;
  bool output ;
  string new_name , record_updated  , name ;
  
  record_updated = "**Record Updated**" ; 
    
  cout << "Enter Student ID : " ;
  cin >> student_id ; 
   
  cur -> set_sql ("SELECT Student_ID,Student_name,PT1,PT2,CW,Overall_marks FROM student_Marks "
                  "WHERE Student_ID = ?" );
  cur -> prepare();
  
  cur -> bind(1,student_id);
  
  cur -> step() ; 
  
     output = cur -> get_int(0);
   
     if (output == 1)
      { 
           name = cur -> get_text(1);
           phase_test1 = cur -> get_int(2);
           phase_test2 = cur -> get_int(3);
           course_work = cur -> get_int(4);
           overall_marks = cur -> get_int(5);
        
        cout << "" << endl;
        cout << "Modifying Student no : "  << student_id << endl;
        cout << "" << endl;
        cout << "1.Student Name"      << endl;
        cout << "2.Phasetest-1 Marks" << endl;
        cout << "3.Phasetest-2 Marks" << endl;
        cout << "4.Course Work Marks" << endl;
        cout << "" << endl;
        cout << "Select An Option : " ;
        cin >> option ;
        array <int,5> options = {1,2,3,4};
        
        bool selected_options = find(begin(options),end(options),option) !=end (options);
        if (selected_options == 1)
        {
          if (option == 1)
              
              {
                 cout << "Old Name : " << name << endl;
                 cout << "Enter New Name : " ;
                 cin.ignore();
                 getline(cin,new_name);
           
                 auto editing_name = db.get_statement(); 
              
                 editing_name -> set_sql ("UPDATE student_Marks "
                                    "SET Student_name = ? "
                                    "WHERE Student_ID = ? ;" );
           
                 editing_name -> prepare();
            
                 editing_name -> bind(1,new_name);
           
                 editing_name -> bind(2,student_id);
             
                 editing_name -> step();
                 
           
                 cout << record_updated << endl;
             }
          if (option == 2)
              {  cout << "Overall marks will be updated Automatically" << endl ;         
                 cout << "Old Phase test 1 Marks : " << phase_test1 << endl ;
                  
                 cout << "Enter new phase test 1 marks : " ;
               
                 cin >> new_phase_test_1;
                     
                 update_overall(new_phase_test_1,phase_test2,course_work,student_id);
            
                 auto editing_pt1 = db.get_statement();
            
            
                 editing_pt1 -> set_sql ("UPDATE student_Marks "
                                    "SET PT1 = ? "
                                    "WHERE student_ID = ? ;" );
            
                 editing_pt1 -> prepare();
            
                 editing_pt1 -> bind(1,new_phase_test_1);
            
                 editing_pt1 -> bind(2,student_id);
            
                 editing_pt1 -> step();
            
                 cout << record_updated << endl;
             }
          if (option == 3)
              {
                 cout << "Overall marks will be updated Automatically" << endl ;
            
                 cout << "Old Phase test 2 Marks : " << phase_test2 << endl ;
            
                 cout << "Enter new phase test 2 marks : " ;
            
                 cin >> new_phase_test_2 ; 
            
                 update_overall(phase_test1,new_phase_test_2,course_work,student_id);
            
                 auto editing_pt2 = db.get_statement();
            
                 editing_pt2 -> set_sql ("UPDATE student_Marks "
                                    "SET PT2 = ? "
                                    "WHERE student_ID = ? ;" );
            
                 editing_pt2 -> prepare();
            
                 editing_pt2 -> bind(1,new_phase_test_2);
            
                 editing_pt2 -> bind(2,student_id);
            
                 editing_pt2 -> step();
            
                 cout << record_updated << endl;
             }
          if (option == 4)
              {
                 cout << "Overall marks will be updated Automatically" << endl ;
            
                 cout << "Old Course work Marks : " << course_work << endl ;
            
                 cout << "Enter new Coursework Marks : " ;
            
                 cin >> new_course_work ;
            
                 update_overall(phase_test1,phase_test2,new_course_work,student_id);
            
                 auto editing_CW = db.get_statement();
            
                 editing_CW -> set_sql ("UPDATE student_Marks "
                                    "SET CW = ? "
                                    "WHERE student_ID = ? ;" );
            
                 editing_CW -> prepare();
            
                 editing_CW -> bind(1,new_course_work);
            
                 editing_CW -> bind(2,student_id);
            
                 editing_CW -> step();
            
                 cout << record_updated << endl;
             }
        
        }
        else
        {
          cout << setw(47) << "**INVALID OPTION**" << endl;
        }
      }
  if (output == 0)
  {
    cout << setw(47) << "**STUDENT ID DOES NOT EXIST" << endl ; 
  }

  }    
void Add_records()
/*This function Adds Data to the  database */
{
    auto s   = db.get_statement();
  
      int student_ID , phase_test1 ,id, phase_test2 , course_work , overall_marks;
  
     string student_name;
  do
    {
      cout << "ADDING NEW STUDENT..." << endl;
      cout << "" << endl;
 
      auto cur = db.get_statement();
     
      cout << "(1/6) - Enter student ID : "  ;
      cin >> student_ID ;
  
      cur -> set_sql( "SELECT Student_ID FROM student_Marks "
                      "WHERE Student_ID = ?;" );
 
      cur -> prepare();
 
      cur -> bind( 1,student_ID );

      cur -> step(); 
        id = cur -> get_int(0);
        if (id != 0)
        {
          cout << "                         **Student ID " << id << " already Exists** " << endl;
        }
   }while (id != 0);
 
  cout << "(2/6) - Enter Student Name : "  ;
  cin.ignore() ;
  getline(cin,student_name) ;
  cout << "(3/6) - Enter Phasetest 1 Marks : "  ;
  cin >> phase_test1;
  cout << "(4/6) - Enter Phasetest 2 Marks : "  ;
  cin >> phase_test2;
  cout << "(5/6) - Enter Course Work Marks : "  ;
  cin >> course_work;
  overall_marks = Over_all_marks_calc(phase_test1,phase_test2,course_work);
  cout << "(6/6) - Overall Marks : "  << overall_marks << endl; 

  s -> set_sql( "INSERT INTO STUDENT_MARKS VALUES (?,?,?,?,?,?);" );
  
  s -> prepare();
  
  s -> bind(1,student_ID);
  
  s -> bind(2,student_name);
  
  s -> bind(3,phase_test1);
  
  s -> bind(4,phase_test2);
  
  s -> bind(5,course_work);
  
  s -> bind(6,overall_marks);
  
  s -> step();
    
  cout  << "Student Added" << endl;
}
void Delete_records()
/* This function Deletes the records on the database */
{
  
  int  option;
  cout << "" << endl ;
  cout << "1.Delete student Record" << endl;
  cout << "2.Delete  All Records" << endl;
  cout << "" << endl;
  cout << "Select an Option : " ;
  array <int,2> options = {1,2};  
  cin >> option ;
  bool selected_options ;
  selected_options = find(begin(options),end(options),option) !=end (options);
  if (selected_options == 1)
  {  
      cout << "" << endl; 
      if (option == 1 )
        {
          cout << "Enter Student ID : " ;
          int student_ID ;
          cin >> student_ID ;
    
          auto delete_record  = db.get_statement();
    
          delete_record -> set_sql("DELETE FROM student_Marks " 
                               "WHERE student_ID = ?");
  
          delete_record -> prepare();
  
          delete_record -> bind(1,student_ID);
  
          delete_record  -> step();
        
          cout << "RECORD DELETED" << endl;
        }
      if (option == 2)
        {
          cout <<"ARE YOU SURE YOU WANT TO DELETE ALL RECORDS - (Y/N) : " ;
    char selection;
    cin >> selection ;
    if (selection == 'y' || selection == 'Y')
    {
      auto delete_all_records  = db.get_statement();
  
      delete_all_records -> set_sql("DELETE FROM student_Marks" );
  
      delete_all_records -> prepare();
  
      delete_all_records  -> step();
      
      cout << "RECORDS DELETED" << endl ;
    }
  }
  }
  else 
  {
    cout << setw(45) << "**INVALID OPTION**" << endl ;
  }
}
int main_menu() 
/*This function displays the main menu of the databse , all the above functions rerturn 
 to this function once they have completed their tasks */
{
  char main_menu_restart ;
  main_menu_restart = 'y' ; 
  do
  {
  
   cout << "                       ----------Main Menu---------"   << endl;
   cout << "                       | 1.View -  Students Marks  |"  << endl;
   cout << "                       | 2.Edit -  Students Marks  |"  << endl;
   cout << "                       | 3.Add  -  Student  Marks  |"  << endl;
   cout << "                       | 4.Delete -Student Record  |"  << endl;
   cout << "                       | 5.Exit                    |"  << endl;
   cout << "                       -----------------------------"   << endl;     
   cout << "Select an Option : " ;
 
   int user_slection ;
   

  cin >> user_slection ;
  array <int,5> options = {1,2,3,4,5};
  
  bool selected_options = find(begin(options),end(options),user_slection) !=end (options);
     if (selected_options == 1)
     main_menu_restart = 'n' ;
  {
       
      if ( user_slection ==  1)
      {
      View_records();
      cout << "" << endl ;
      cout << "Back to main menu  - (Y/N) : " ;
      cin >> main_menu_restart ; 
      if ( main_menu_restart == 'n' || main_menu_restart == 'N' )
      {
       main_menu_restart = 'n'; 
      }
      } 
      else if (user_slection == 2)
      {
          char edit_record_restart;
          do
          { 
            Edit_records();
            cout << "Start Again  - (Y/N) : " ; 
            cin >> edit_record_restart ;
          } while (edit_record_restart == 'y' || edit_record_restart == 'Y');
      }
      else if (user_slection == 3)
        {
          char add_record_restart;
          add_record_restart = 'y' ; 
          do
            {
              Add_records();
              cout << "Start again - (Y/N) : " ;
              cin >> add_record_restart;
              if (add_record_restart == 'n' || add_record_restart == 'N')
              {
                  add_record_restart = 'n';
              }
            } while ( add_record_restart == 'y' || add_record_restart == 'Y');
        }
      else if (user_slection == 4)
       {
          char delete_record_restart;
          do
          {
            Delete_records();
            cout <<"Start again - (Y/N) : " ;
            cin >> delete_record_restart; 
          } while (delete_record_restart == 'y' || delete_record_restart == 'Y');
       }
    else if (user_slection == 5)
      {
         return 0 ;
      }
  }
       
 if ( selected_options != 1 )
  {
    cout << setw(45) << "**INVALID OPTION**" << endl;
  }
 
  }while(main_menu_restart == 'y'|| main_menu_restart == 'Y');
}
int main()
{  
     main_menu();
     return 0 ;
}  
    
    
    
    
    
 