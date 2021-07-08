// 3-memory allocation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;
#include <vector>
#include"string"
#include"fstream"
#include "sstream"
#include <filesystem>

vector <int> numofunallocated; // global vector to hold number of unallocated proccesses from each algorith respectively 
vector <double> ratios;  //gloab vector to hold average precentage of internal fragmentation from from each algorith respectively 

void print(vector<int> process_block, vector<int> processes, vector<int> leftover, vector<int> partitions, vector<int> partitions_copy)
{
	double freesum = 0; // variable to hold sum of free spaces in memory
	bool extfrag = 0; // boolean to indicate existance of external fragmentation 
	double sumfragmentsratios=0; // variable to hold sum of internal fragmentation ratios 
	vector<int> unallocated; // vector to hold unallocated processes 

	
	
	for (unsigned i = 0; i < process_block.size(); i++)
	{
		if (process_block[i] == -1) // check if process was not allocated
		{
			cout << " Process " << i + 1 << " is not allocated " << endl;
			unallocated.push_back(processes[i]); // add this process to unallocated processes vector 

		}
		else
			cout << " Process " << i + 1 << " is allocated in block " << process_block[i] + 1 << " with internal fragmentation " << leftover[i] << " Kb" << endl;

	}
	cout << endl;
	

	cout << " Size of Free holes after the algorithm exceution in order :  ";
	for (unsigned i = 0; i < partitions_copy.size(); i++)
	{

		cout << partitions_copy[i] << " KB    "; //
		
			freesum += partitions_copy[i]; // calculating sum of remaining free memory spaces



		
	}



	cout << endl;
	cout << endl;
	for (unsigned i = 0; i < unallocated.size(); i++)
	{
		if (unallocated.at(i) <= freesum) // comparing unallocated processes sizes with free memory spaces to check external frag.
		{
			extfrag = 1;

		}

	}

	if (extfrag)
	{
		cout << " External Fragmentation occurs , enough amount of space is found to satisify request but it is not contiguous, a possible solution is compaction " << endl;

	}


	else if (unallocated.size() == 0)
		cout << " All processes were allocated , no external fragmentation." << endl;
	else
		cout << " Free Space is not enough to satisify request , no external fragmentation." << endl;
	cout << endl;

	for (unsigned i=0; i < partitions.size(); i++) 
	{

		if (partitions[i] != partitions_copy[i])  // this indicates that the block size was changed 
		{
			double x, y;
			x = partitions_copy[i], y = partitions[i];
			sumfragmentsratios += (x/y); // we divide the remaining size in block by its original size to find ratios of fragmentation


		}
			
		 
	}
	
	
	numofunallocated.push_back(unallocated.size()); // adding number of unallocated processes of current algorithm (funtion caller )

	double size = partitions.size();
	ratios.push_back( (sumfragmentsratios / partitions.size()) * 100); // adding percentage of current algorithm ( function caller)

	
}



void firstfit(vector <int> processes, vector <int> partitions)
{
	cout << "                                               ####  First Fit #####  " << endl;
	cout << endl;
	vector <int> process_block;  // vector to hold the index of block in which each processe is allocated respectively .
	vector <int> leftover; // vector to hold internal fragmentation values 
	
	vector <int> partitions_copy; // this vector is a copy of partitions vector , because we need to keep values of original one

	for (unsigned i = 0; i < partitions.size(); i++) // copying content of partitions to partitions_copy
	{
		partitions_copy.push_back(partitions[i]);

	}
	
	for (unsigned i = 0; i < processes.size(); i++) 
	{
		process_block.push_back(-1); // -1 means not allocated yet
		leftover.push_back(-1);  

	}



	for (unsigned i = 0; i < processes.size(); i++)
	{
		for (unsigned j = 0; j < partitions_copy.size(); j++)
		{
			if (processes[i] <= partitions_copy[j]) // checking if the partition has enough space to allocate current process 
			{
				process_block[i] = j; // alloctation of process 
				partitions_copy[j] = partitions_copy[j] - processes[i]; // calculating new size of partition 
				leftover[i] = partitions_copy[j]; // saving the value of internal fragmentation 
				break;
			}
		}
	}

	 print(process_block, processes, leftover, partitions, partitions_copy);
	

}

void bestfit(vector <int> processes, vector <int> partitions)
{
	cout << "                                               ####  Best Fit #####  " << endl;
	vector <int> process_block;
	vector <int> leftover;
	
	vector <int> partitions_copy;

	for (unsigned i = 0; i < partitions.size(); i++)
	{
		partitions_copy.push_back(partitions[i]);

	}


	for (unsigned i = 0; i < processes.size(); i++)
	{
		process_block.push_back(-1); // -1 means not allocated yet
		leftover.push_back(-1);

	}



	for (unsigned i = 0; i < processes.size(); i++)
	{
		for (unsigned j = 0; j < partitions.size(); j++)
		{
			if (processes[i] <= partitions_copy[j]) // check if partition space is enough to allocate process 
			{
				if (process_block[i] == -1) // check if the process was not allocated before 
				{
					process_block[i] = j; // allocate the process 


				}

				else if (partitions_copy[process_block[i]] > partitions_copy[j]) // check if there is a smaller partition that fits the process 
				{
					process_block[i] = j; // allocate the process 


				}



			}
		}
		if (process_block[i] > -1) // check if the current process was allocated 
		{
			partitions_copy[process_block[i]] = partitions_copy[process_block[i]] - processes[i]; // calculate new memory partition(block size )
			leftover[i] = (partitions_copy[process_block[i]]); // saving the value of internal fragmentation 
		}
	}

	 print(process_block, processes, leftover, partitions, partitions_copy);

}


void worstfit(vector <int> processes, vector <int> partitions)
{
	cout << "                                               ####  Worst Fit #####  " << endl;
	vector <int> process_block;
	vector <int> leftover;
	vector <int> partitions_copy;

	for (unsigned i = 0; i < partitions.size(); i++)
	{
		partitions_copy.push_back(partitions[i]);

	}


	for (unsigned i = 0; i < processes.size(); i++)
	{
		process_block.push_back(-1); // -1 means not allocated yet
		leftover.push_back(-1);

	}



	for (unsigned i = 0; i < processes.size(); i++)
	{
		for (unsigned j = 0; j < partitions.size(); j++)
		{
			if (processes[i] <= partitions_copy[j]) // checking if partition size is enough to allocate process 
			{
				if (process_block[i] == -1) // check if process is not already allocated 
				{
					process_block[i] = j;


				}

				else if (partitions_copy[process_block[i]] < partitions_copy[j]) // check if there is a larger partition that fits the process 
				{
					process_block[i] = j;


				}



			}
		}
		if (process_block[i] > -1)
		{
			partitions_copy[process_block[i]] = partitions_copy[process_block[i]] - processes[i]; //calculate new memory partition(block size)
			leftover[i] = (partitions_copy[process_block[i]]); // saving value of internal fragmentation 
		}
	}

	 print(process_block, processes, leftover, partitions, partitions_copy);


}


void compare()
{
	cout << "                                                          ####  Compare  #####  " << endl;
	vector <int> index; // vector to store index of algorithms which could allocate all processes 
	bool equalratio = 1; // when this boolean is equal 1 it means the ratios receieved from the 3 algorithms are equal
	// a struct to hold the algorithms data to do the comparsion processes .
	struct algorithm { 
		string name;
		int unallocatednum; // number of unallocated processes of the algorithm 
		double ratio; //

	};


	algorithm arr[3]; // array of type struct of size 3 , indicating the 3 algorithms respectively 
	arr[0].name = "First fit";
	arr[1].name = "Best fit";
	arr[2].name = "Worst fit";



	for (unsigned i = 0; i < numofunallocated.size(); i++)
	{
		arr[i].unallocatednum = numofunallocated[i]; 
		arr[i].ratio = ratios[i];
		cout << " Number of unallocated processes using " << arr[i].name << " Algorithm :" << arr[i].unallocatednum << endl;

	}

	cout << endl;
	// variable to hold the minimum percentages and their index 
	double minratio = 1000;  

	int indexminratio = 0, indexminratio2 = -1;



	for (int i = 0; i < 3; i++) 
	{
		if (arr[i].unallocatednum == 0) // keeping track of algorithms that was able to allocate all processes 
		{
			index.push_back(i);

		}
	}

	if (index.size() == 1) // we have only one algorithm that could allocate all proccesses 
	{

		cout << " Therefore most efficent algorithm is " << arr[index[0]].name << " as it has no waiting process " << endl;

	}

	else if (index.size() > 1) // we have more than one algorithm that could allocate all proccesses
	{
		cout << " Since more than one algorithm could allocate all processes we will examine the fragmentation of equal algorithms to decide the most efficient ,one with least percentage is the best" << endl << endl;
		for (int i = 0; i < index.size(); i++)
		{

			cout << "Average percentage of internal fragmentation " << arr[i].name << " algorithm = " << arr[i].ratio <<"%"<< endl;
			if (i < index.size() - 1)
			{
				if (arr[i].ratio != arr[i + 1].ratio) // check if the 3 algorithms have equal percentages
					equalratio = 0;
			}

		}
		cout << endl;
		if (equalratio == 1)
		{
			cout << " Percentages of internal fragmentation are also equal , therefore the algorithms have equal efficiency " << endl;

		}

		else
		{
			for (int i = 0; i < index.size(); i++)
			{
				
					if (arr[i].ratio < minratio) // getting the algorithm with least internal fragmentation percentage ( most efficient)
					{
						minratio = arr[i].ratio;
						indexminratio = i;


					}

					else if (arr[i].ratio == minratio) // checking if 2 algorithms have equal percentage 
					{
						indexminratio2 = i;
					}
				
				
				
				
			}
		
			if (indexminratio2 != -1) // means 2 algorithms have equal percentages
			cout << " Therefore most efficient algorithms are  " << arr[indexminratio].name << " and " << arr[indexminratio2].name;
			else
				cout << " Therefore most efficient algorithm is " << arr[indexminratio].name;
		

		}

	}

	else if (index.size() == 0) 
	{
		cout << " The 3 algorithms were not enough efficient ,none of them allocated all processes " << endl;


	}

}
	





int main()
{
	int datachoice, algochoice,  repeat = 1;
	int processnum, partnum;
	vector <int> processes;
	vector <int> partitions;

	do
	{
		

		cout << " Hello , would you like to enter your inputs or load them from a file  ?" << endl << endl;
		cout << " 1-Enter inputs " << endl << " 2-Load inputs from a file" << endl;
		cin >> datachoice;
		if (datachoice == 1)
		{

			cout << " Please enter number of processes " << endl;
			cin >> processnum;
			cout << " Please Enter the size of each process in KB (in order)" << endl;
			for (int i = 0; i < processnum; i++)
			{
				int input;
				cin >> input;
				processes.push_back(input);

			}
			cout << " Please Enter The number of free memory partitions " << endl;
			cin >> partnum;
			cout << " Please Enter sizes of free memory partitions in KB (in order) " << endl;
			for (int i = 0; i < partnum; i++)
			{
				int input;
				cin >> input;
				partitions.push_back(input);
			}
			cout << endl;

		}

		if (datachoice == 2)
		{
			
			char path[32];
			

			cout << " Could you please enter the path of your file containing the data ?   Example : D:\\filename.txt \n Note : please avoid spaces in your file name "<< endl;
			cin >> path;
			cout << endl;
			
			
			

			fstream myfile(path, ios_base::in);
			int input;
			int linecount=0;
			string readstr;
			while (getline(myfile, readstr))
			{
				stringstream sstr(readstr);

				while (sstr >> input)
				{
					if(linecount==0)
				   processes.push_back(input);
					else
				  partitions.push_back(input);


				}

				linecount++;

			}

			cout << " Processes loaded from file and their sizes in order are :";
			for (int i = 0; i < processes.size(); i++)
			{
				cout << processes[i] << " KB  ";

			}
			cout << endl;
			cout << " free memory partitions loaded from file and their sizes in order are :";
			for (int i = 0; i < partitions.size(); i++)
			{
				cout << partitions[i] << " KB  ";

			}
			cout << endl;
			cout << endl;

		}

		

		cout << " Which algorithm would you like to apply ?" << endl;
		cout << " 1-First Fit " << endl << " 2-Best Fit " << endl << " 3-Worst fit" << endl << " 4-Compare between the 3 algorithms in terms of how efficiently they use the memory" << endl;
		cin >> algochoice;


		switch (algochoice)
		{
		case 1: firstfit(processes, partitions);
			break;
		case 2: bestfit(processes, partitions);
			break;
		case 3:	worstfit (processes, partitions);
			break;
		case 4:
		
			firstfit(processes, partitions);
			bestfit(processes, partitions);
			worstfit(processes, partitions);
			compare();
			break;
		
		
		}

		cout << " Thanks for choosing our service ! , do you need anymore services ?" << endl;
		cout << " 1-Yes " << " 2- No ( exit program ) " << endl;

        cin >> repeat;
		
		numofunallocated.clear();
		ratios.clear();
		processes.clear();
		partitions.clear();
		
		system("cls");

			
		



	} while (repeat!=2);




}

