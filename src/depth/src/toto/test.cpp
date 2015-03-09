#include<iostream>
#include<stdlib.h>
#include<fstream>
#include<string.h>
#include<sstream>
#include<time.h>

using namespace std;


void letswait(int);

int main(int argc, char **argv){
	
	char filename[100];
	float temp;
	int row=0;
	stringstream rosrun;
	
	//find out number of rows
	strcpy(filename,argv[1]);
	ifstream myfile (filename);
	if(myfile.is_open()){
	while(!myfile.eof()){
		myfile>>temp;
		row++;		
	}
	}	
	row--;
	row=row/6;
	
	//refresh the file read
	myfile.clear();
	myfile.seekg(0,myfile.beg);
	
	//create dynamic table
	float **table; 
	table = new float* [row];
	for(int i=0;i<row;i++)
	table[i] = new float [6];
	
	//read the file into table
	if(myfile.is_open()){
	while(!myfile.eof()){
		//cout<<myfile;
		for(int i=0;i<row;i++){
		for(int j=0;j<6;j++){
		myfile>>table[i][j];
		}}
	
	}
	myfile.close();
	}

	for(int i=0;i<row;i++){
	
	//create string stream for rosrun
	rosrun<<"rosrun jaco_demo joint_angle_workout.py jaco "<<table[i][0]<<" "<<table[i][1]<<" "<<table[i][2]<<" "<<table[i][3]<<" "<<table[i][4]<<" "<<table[i][5];
	
	//call rosrun jaco_demo with one set of angles
	system(rosrun.str().c_str());
	
	//clear stream
	rosrun.str("");
	
	//wait for stability
	letswait(1);
	
	rosrun<<"rosrun depth depth_node c_jac"<<i<<".png 1";
	system(rosrun.str().c_str());
	rosrun.str("");
	
	rosrun<<"rosrun depth depth_node d_jac"<<i<<".png 0";
	system(rosrun.str().c_str());
	rosrun.str("");
		
	//wait a little longer
	//letswait(1);
	}
	
	
		
	//debug output of the table
	/*
	for(int i=0;i<row;i++){cout<<endl;
	for(int j=0;j<6;j++){
	cout<<table[i][j]<<" ";
	}
	}*/
	
	//free memory
	delete[] table; 
	
	
	return 0; 
}


void letswait(int secs){
clock_t end;
end=clock()+secs*CLOCKS_PER_SEC;
while(clock()<end);
}


