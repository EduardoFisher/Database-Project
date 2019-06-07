# Database-Project
Database Project

This Project involves me creating a datebase, this project involves me creating my version of SQL (NOTE: That this is not a full version of SQL and thus oes not have all the functionality of true SQL).
This comes with a already created file named Students that has the binary file with all of its information preloaded into the file, along with the Filenames name .txt file needed to load the table Students into the program.

------------------------------------------------------------------------------------------------------------------------------------------
OPERATIONS SUPPORTED: 
                      Create: Creates a table with the specified columns1(name), ...., columnN(name).
                      Load: Loads a previously created table for use in the program(NOTE: Tha all table must be loaded before they can be                             used) all tables are automatically loaded into the program after they are created.
                      Print: Prints all the contents of the table on the screen.
                      End: Closes the program.
                      Insert: Inserts an item into the table.
                      Select: Prints items to the screen based off the query that you just did.
------------------------------------------------------------------------------------------------------------------------------------------

------------------------------------------------------------------------------------------------------------------------------------------
EXAMPLE (Type these commands into the program to see what they):
        Create: Create Names Fname, Lname
        Load: Load Students
        Print: Print Students
        Insert: Insert into Students Fname, Lname, 3.0
        Select: Select * from Students 
                Select * from Students where GPA < "4.0"
                Select GPA from Students where GPA < "4.0"
                Select GPA from Students where GPA < "4.0" and GPA > "3.0"
                Select GPA from Students where GPA < "4.0" or GPA > "3.0"
                Select * from Students where GPA < "4.0" and GPA > "3.0" and Fname > "F"
                Select * from Students where GPA < "4.0" and GPA > "3.0" or Fname > "F"
                Select * from Students where GPA < "4.0" and GPA > "3.0" and Fname > "F" and Fname < "W"
        End: End
        Error Message given to you: Not a valid string please enter another one.
------------------------------------------------------------------------------------------------------------------------------------------
