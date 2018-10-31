/*
MazeSolver.cpp
Kiara Barias
October 31, 2018
CSCI 235, Fall 2018, Project 4
Implementation of Mazesolver:
Your maze solver will read the maze from an input file, find a path that leads to the exit and print the solution to the standard output
*/


#include <iostream>
#include <fstream>
#include "MazeSolver.h"
using namespace std;

MazeSolver::MazeSolver(string input_file)
{
	ifstream in_stream; //creates an ifstream object (input file stream) called in_stream
	in_stream.open(input_file); //use the object to read from a file entered
	if(in_stream.fail()) //if the reading of file fails, display error message
	{
		cout<<"Cannot read from input_file_name";
	}

	int roww; 
	in_stream >> roww; //read first item of file (row) into an integer variable roww
	int coll;
	in_stream >> coll; //read first item of file (col) into an integer variable coll
	initializeMaze(roww, coll); //initialize the corresponding data members
	if (mazeIsReady() == true) //if maze was successfully initialized then the mazeisready
	{
		fillMaze(in_stream);
		initializeSolution();
		Position current;
		current.row = 0;
		current.column = 0;
		backtrack_stack_.push(current); //pushes in the current position onto the stack
	}

	in_stream.close(); //stop reading from file
}

MazeSolver::~MazeSolver()
{
	for(int i = 0; i < maze_rows_; i++)
    {
    	delete [] maze_[i];
    }
    delete [] maze_;

    for(int i = 0; i < maze_rows_; i++)
    {
    	delete [] solution_[i];
    }
    delete [] solution_;
}

bool MazeSolver::mazeIsReady() //a method that returns a boolean indicating whether the maze has been initialized
{
	if((maze_rows_ > 0) && (maze_columns_ > 0)) //checks to make sure that valid rows and columns were created so that maze is ready to use
	{
		maze_ready = true;
	}
	else{
		maze_ready = false;
	}
	return maze_ready;
}

void MazeSolver::printSolution() //a method that prints the solution to standard output
{
	cout << "The solution to this maze is:" << endl;
	for(int i = 0; i < maze_rows_; i++)
    {
    	for(int j = 0; j < maze_columns_; j++) //copy items from rows and columns
    	{
    		if(j==maze_columns_-1) //if the item column is the last column of the row jump to new line to create array, else just print out items
    		{
    			cout << solution_[i][j] << " " << endl;
    		}
    		else {cout << solution_[i][j] << " ";}
    	}
    }
    cout << endl;
}

void MazeSolver::initializeMaze(int rows, int columns) //constructor helper function
{
	maze_rows_ = rows;             //set rows and columns that were read from files, to the mze rows and columns 
    maze_columns_ = columns;
    maze_ = new char*[maze_rows_]; //allocate memory for rows
    for(int i = 0; i < maze_rows_; i++)
    {
    	maze_[i] = new char[maze_columns_]; //for each allocated row allocate a column
    }
}

void MazeSolver::fillMaze(ifstream& input_stream) //fills in maze with actual characters
{
	string characters;
	getline(input_stream, characters); //gets characters from file

	int start = 1;
	for(int i = 0; i < maze_rows_; i++)
    {
    	for(int j = 0; j < maze_columns_; j++)
    	{
    		maze_[i][j] = characters[start]; //copy what is in the character string and put it in the maze
    		start+=2; //move to next character on string by jumping 2, to avoid reading spaces
    	}
    }
}

void MazeSolver::initializeSolution()
{
	solution_ = new char*[maze_rows_]; //initializes array for solution_
    for(int i = 0; i < maze_rows_; i++)
    {
    	solution_[i] = new char[maze_columns_]; 
    }
    copyMazetoSolution(); //copy what was in maze to solution
}

void  MazeSolver::copyMazetoSolution()
{
	for(int i = 0; i < maze_rows_; i++) 
    {
    	for(int j = 0; j < maze_columns_; j++)
    	{
    		solution_[i][j] = maze_[i][j]; //copy stuff
    	}
    }
}

bool MazeSolver::solveMaze() //a method that finds a solution to the maze
{
	Position current_;
	current_.row = (backtrack_stack_.top()).row;
	current_.column = (backtrack_stack_.top()).column; //set current position to what was at the top of the stack
	while (!backtrack_stack_.empty()) //while there is something on the stack
	{
		if(maze_[current_.row][current_.column] == '$') //checks to see if current position is an exit
		{
			cout << "Found the exit!!!" << endl;
			return true;
		}
		else if(extendPath(current_) == true) //if the path could be extended from the current postion
		{
			solution_[current_.row][current_.column] = '>'; //mark it as part of the path to solution on solution_
			current_ = backtrack_stack_.top(); //set current position to what is on top of the stack
		}
		else if(extendPath(current_) == false) //if you are stuck and can't move
		{
			maze_[current_.row][current_.column] = 'X'; //set the current position as visited on maze
			solution_[current_.row][current_.column] = '@'; //set current position as backtracked on solution
			backtrack_stack_.pop(); //pop the stack
			if(!backtrack_stack_.empty()) //if the stack is not empty 
			{
				current_ = backtrack_stack_.top(); //set the current position to the one on top of the stack
			}
			else if(backtrack_stack_.empty() == true) //if stack is empty then there is no solution
			{
				cout << "This maze has no solution." << endl;
				return false;
			}
		}
	}
	return true;
}

bool MazeSolver::extendPath(Position current_position)
{
	bool extend = false;
	Position new_position;
	if(isExtensible(current_position, SOUTH) == true)  //if path can move south 
	{
		new_position = getNewPosition(current_position,SOUTH); //get the position of moving one south
		backtrack_stack_.push(new_position); //push it on top of the stack
		extend = true;
	}
	if(isExtensible(current_position, EAST) == true) //if path can move east
	{
		new_position = getNewPosition(current_position,EAST); //get the position of moving one east
		backtrack_stack_.push(new_position); //push it on top of the stack
		extend = true;
	}
	return extend; //return whether it can be extended or not
}

Position MazeSolver::getNewPosition(Position old_position, direction dir)
{
	if(dir == SOUTH)
	{
		old_position.row = (old_position.row)+1; //moves one row down
	}
	if(dir == EAST)
	{
		old_position.column = (old_position.column)+1; //moves one column over to the right
	}
	return old_position;
}

bool MazeSolver::isExtensible(Position current_position, direction dir)
{
	if(dir == SOUTH)
	{
		if(((current_position.row)+1 < maze_rows_) && maze_[(current_position.row)+1][current_position.column]!= '*' && (maze_[(current_position.row)+1][current_position.column]!= 'X')) //checks that moving south is still on the maze and that it is not a wall
		{
			return true; //checks if can move one row down by keeping inside maze rows and it is not a wall or has been visited
		}
	}
	if(dir == EAST)
	{
		if(((current_position.column)+1 < maze_columns_) && maze_[current_position.row][(current_position.column)+1]!= '*' && (maze_[current_position.row][(current_position.column)+1]!= 'X')) //checks that moving south is still on the maze and that it is not a wall
		{
			return true; //checks if can move one column right by keeping inside maze columns and it is not a wall or has been visited
		}
	}
	return false;
}
