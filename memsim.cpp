//This code is a simulator
//This code implements FIFO, LRU and VMS page replacements

#include <unistd.h>
#include <stack>
#include <iostream>
#include <cstdio>//to be able to use functions from C to C++
#include <cstring>//for string functions
#include <queue>//for implementing a queue in FIFO
#include <vector>//for implementing a vector in LRU
#include <fstream>//for file operations
#include <string>//helps work with strings of characters
#include <sstream>//This will help convert values to int
	
	typedef struct Page
	{
	std::string addr;//stores the memory address
	char rw;//store the read-write bit
	bool dirty_bit;
	}Page;
	Page mem;
	
	//Initializing global variables
	int page_size = 0; //page frames will be the size
	int write_count = 0;
	int file_size = 0;
	int read_count = 0;
	int percent = 0;//This will help store the percent argument for VMS
	int queueSize = 0;   
	FILE *file; //file will be used to input the trace files into the file
	std::vector<Page> Input;//This vector will store the string which will have the address and the read-write character
	std::string myString;		

	void fifo(int p_size)
	{
 	//The section below will implement the queue to implement FIFO page replacement
	std::vector<Page> fifo(p_size);//Empty vector for implementing FIFO
	int hit = 0; // This integer is used to confirm whether we've found a page within the page table
	int counter = 0;
	Page temp;
	int max = 0;
	
	for(int i = 0; i < Input.size(); i++)
	{ 		
			//For the first 63 inputs, it will either be a hit or a miss (buffer not full)
			// Save current input in temporary page
			temp = Input.at(i);

			if(temp.addr.size() != 5) {break;}

			if(fifo.size() <= queueSize - 1) {max = 1;}

			else {std::cout << "Not full\n"; max = 0;}

			/*
			std::cout << "input count: " << i << "\n";
			std::cout << "queueSize: " << queueSize << "\n";
			std::cout << "Maximum Capacity Reached: " << max << "\n";*/

			//Search queue for copies
			for(int j = 0; j < fifo.size(); j++)
			{	
			
				// If you find a copy, confirm as hit
				if(fifo.at(j).addr == temp.addr)
				{
				
				hit++;

		
				
				if(temp.rw == 'R') {break;}//do nothing
				else if(temp.rw == 'W') 
				{
				fifo.at(j).dirty_bit = 1; break;			
				}
				}
			}
		 	if( (hit == 0) && (max == 0))
			{
			fifo.push_back(temp);
			queueSize++;
			if(temp.rw == 'R') { read_count++; }
			else if(temp.rw == 'W') {fifo.back().dirty_bit = 1; read_count++;} 
			}
			else if ((hit == 0) &&  (max == 1))
			{		
				if(temp.rw == 'R')
				{
				if(fifo.at(0).dirty_bit == 1) {write_count++;}
				fifo.erase(fifo.begin());
				fifo.push_back(temp);
 				read_count++;
				}
				else if(temp.rw == 'W')
				{
				if(fifo.at(0).dirty_bit == 1) {write_count++;}
				fifo.erase(fifo.begin());
				fifo.push_back(temp);
				fifo.back().dirty_bit = 1;
				read_count++;
				}	
			}
			counter++;
			hit = 0;	
	}
	std::cout<<"total disk reads:"<<read_count<<std::endl;
	std::cout<<"total disk writes:"<<write_count<<std::endl;
	}

	void lru(int p_size)
	{
	//The section below will implement the queue to implement FIFO page replacement
	std::vector<Page> lru(p_size);//Empty vector for implementing FIFO
	int hit = 0; // This integer is used to confirm whether we've found a page within the page table
	int counter = 0;
	Page temp;
	int max = 0;

	for(int i = 0; i < Input.size(); i++)
	{ 	
			//For the first 63 inputs, it will either be a hit or a miss (buffer not full)
			// Save current input in temporary page
			temp = Input.at(i);

			if(temp.addr.size() != 5) {break;}

			if(lru.size() <= queueSize - 1) {max = 1;}

			else {std::cout << "Not full\n"; max = 0;}

			//Search queue for copies
			for(int j = 0; j < lru.size(); j++)
			{	
				// If you find a copy, confirm as hit
				if(lru.at(j).addr == temp.addr)
				{
				hit++;
				if(temp.rw == 'R') {temp = lru.at(j); lru.erase(lru.begin()+j); lru.push_back(temp); break;}//do nothing
				else if(temp.rw == 'W') 
				{
				temp = lru.at(j); lru.erase(lru.begin()+j); lru.push_back(temp);
				lru.back().dirty_bit = 1; break;
				}
				}
			}

		 	if( (hit == 0) && (max == 0))
			{
			lru.push_back(temp);
			queueSize++;
			if(temp.rw == 'R') { read_count++;}
			else if(temp.rw == 'W') {lru.back().dirty_bit = 1; read_count++;} 
			}
			else if ((hit == 0) &&  (max == 1))
			{	
				if(temp.rw == 'R')
				{
				if(lru.at(0).dirty_bit == 1) {write_count++; }
				lru.erase(lru.begin());
				lru.push_back(temp);
 				read_count++;
				}
				else if(temp.rw == 'W')
				{
				if(lru.at(0).dirty_bit == 1) {write_count++;}
				lru.erase(lru.begin());
				lru.push_back(temp);
				lru.back().dirty_bit = 1;
				read_count++;
				}	
			}
			counter++;
			hit = 0;
	}
	std::cout<<"total disk reads:"<<read_count<<std::endl;
	std::cout<<"total disk writes:"<<write_count<<std::endl;
	}
	
	void vms(int size, int percent)
	{
	
	int prim_size = 0;//primary size for FIFO
	int sec_size = 0;//secondary size for LRU
	
	sec_size = (size * percent) / 100;//formula from previous TA's slides
	prim_size = size - sec_size;
	
	std::vector<Page> fifo_vms(prim_size);//creating a vector of type Page for VMS usingFIFO
	std::vector<Page> lru_vms(sec_size);//creating a vector of type Page for VMS using LRU
	Page temp;//temporary struct variable of type Page
	bool hit;
		
		if(sec_size == 0) fifo(prim_size);
		else if(prim_size == 0) lru(sec_size);
		
		else
		{
		for(int j = 0; j < Input.size(); j++)
		{
			temp = Input.at(j);
			for(int i = 0; i < fifo_vms.size(); i++)
			{
				if(fifo_vms.at(i).addr == temp.addr)
				{ 
				hit = 1;
				if(temp.rw == 'W'){fifo_vms.at(i).dirty_bit = 1;}
				break;
				}
			}

			if(hit == 0)
			{
				for(int i = 0; i < lru_vms.size(); i++)
				{
				if(lru_vms.at(i).addr == temp.addr)
				{
				hit = 1;
				Page current = lru_vms.at(i);
				Page initial = fifo_vms.at(0);

				lru_vms.erase(lru_vms.begin()+i);
				fifo_vms.erase(fifo_vms.begin());
				
				lru_vms.insert(lru_vms.end(),initial);
				fifo_vms.insert(fifo_vms.end(),current);
			
				if(temp.rw == 'W')fifo_vms.at(fifo_vms.size()-1).dirty_bit = 1;
				break;
				}
				}
			}
			
			if(temp.addr.size() !=5) break;
			
			if(hit == 0)
			{
				if(fifo_vms.size() == prim_size)
				{
					if(lru_vms.size() == sec_size)
					{
					if(lru_vms.front().dirty_bit == 1){write_count++;}
					lru_vms.erase(lru_vms.begin());

					lru_vms.push_back(fifo_vms.at(0));//stores the page from the front of fifo vector to the back of lru
					fifo_vms.erase(fifo_vms.begin());//pops the first page of the fifo vector
					
				
					if(temp.rw == 'W'){temp.dirty_bit = 1;}
					
					fifo_vms.push_back(temp);
					read_count++;
					}
				
					else
					{
					lru_vms.push_back(fifo_vms.at(0));
					fifo_vms.erase(fifo_vms.begin());
					if(temp.rw == 'W'){temp.dirty_bit = 1;}
					fifo_vms.push_back(temp);
					read_count++;
					}
				}
			
				else
				{
				if(temp.rw = 'W') {temp.dirty_bit = 1;}
				fifo_vms.push_back(temp);
				read_count++;
				}
			}
		hit = 0;
		}
		std::cout<<"total disk reads:"<<read_count<<std::endl;
		std::cout<<"total disk writes:"<<write_count<<std::endl;
		}	
	}
	

int main(int argc, char *argv[])
{
	char *cmd = NULL;
	int file_size = 0;
	std::string trace;
	std::string type;
	std::string test;//for quiet or debug command
	
	if (argc < 5 or argc > 6){return 0;}

	for(int i = 0; i< argc; i++)
	{cmd = argv[i];}
	
	//The next if-else if statements help to store the values of the page frames and the precentage for VMS
	if(argc == 5)
	{
	std::stringstream(argv[2]) >> page_size;//This will be useful for implementing the page methods
	type = argv[3];
	test = argv[4];
	}
	else if(argc == 6)
	{
	std::stringstream(argv[2]) >> page_size;
	std::stringstream(argv[4]) >> percent;//Storing percent as an integer
	type = argv[3];
	test = argv[5];
	}
	trace = argv[1];
	if(trace == "bzip.trace")
		{
		std::ifstream file("bzip.trace"); //file will be used to input the trace files into the file
		if(file.is_open())
		{
		while (!file.eof())
		{
		file >> myString;
		
		mem.addr = myString.substr(0,myString.length()-3);//splitting the string to store address
		file >> mem.rw;
		if(mem.rw == 'R') mem.dirty_bit = 0;
		else if(mem.rw == 'W') mem.dirty_bit = 1;
		getline(file, myString);	
		Input.push_back(mem);		
		file_size = file_size + 1;
		if(test == "debug")
		{
		std::cout<<"String:"<<mem.rw<<std::endl;
		std::cout<<"Address:"<<mem.addr<<std::endl;
		}
		}
		}
		file_size = file_size - 1;
 		file.close();
		}

	else if(trace == "sixpack.trace")
		{
 		std::ifstream file("sixpack.trace"); //file will be used to input the trace files into the file
		if(file.is_open()) 
		{
		while (!file.eof())
		{
		std::string Hold;
		file >> myString;
		mem.addr = myString.substr(0,myString.length()-3);//splitting the string to store address
		file >> mem.rw;
		if(mem.rw == 'R') mem.dirty_bit = 0;
		else if(mem.rw == 'W') mem.dirty_bit = 1;	
		getline(file, myString);
		Input.push_back(mem);
		file_size = file_size + 1;
		if(test == "debug")
		{
		std::cout<<"String:"<<mem.rw<<std::endl;
		std::cout<<"Address:"<<mem.addr<<std::endl;
		}
		
		}
		}
		file_size = file_size - 1;
 		file.close();
		}
	
	//The next two lines print the first two statements regarding memory frames and number of traces
	std::cout<<"total memory frames: "<<page_size<<std::endl;
	std::cout<<"events in trace: "<<file_size<<std::endl;
	
	//These statement will call the page replacement method
	if(type == "fifo") 
		{
		fifo(page_size);
		if(test == "quiet") execlp(cmd, cmd, cmd, cmd, cmd, NULL);
		else if(test == "debug")std::cout<<"In debug mode"<<std::endl;
		}
	
	else if(type == "lru") 
		{
		lru(page_size);
		if(test == "quiet") execlp(cmd, cmd, cmd, cmd, cmd, NULL);
		else if(test == "debug")std::cout<<"In debug mode"<<std::endl;
		}
	
	
	else if(type == "vms") 
		{
		vms(page_size, percent);
		if(test == "quiet") execlp(cmd, cmd, cmd, cmd, cmd, cmd, NULL);
		else if(test == "debug")std::cout<<"In debug mode"<<std::endl;
		}
	return 0;
}