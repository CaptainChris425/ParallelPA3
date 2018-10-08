/*
/ Function:
/ Parameters:
/ Useage:
/ Return:
*/

/*
#include <iostream>
#include <vector>
#include <ctime>
std::vector< std::vector<int> > generatesquare(int n, int m);
std::vector<int> nrands(int n, int s);
std::vector< std::vector<int> > generateRsquare(int n,int m, int s);
*/
#include "generation.h"

int useageexamples(){
	std::srand(std::time(nullptr));
	std::vector< std::vector<int> > vect;
	std::vector<int> vect2;
	int n = 10;
	vect = generatesquare(5,n);	
	vect2 = nrands(n, std::time(nullptr));
	for (int i= 0; i<n; i++){
		std::cout << i << " ";
		std::cout << vect2[i] << "\n";
	}
	std::cout << "size -> " << vect.size() << "\n";	
	for (int i = 0; i<5; i++){
		std::cout << i << " ";
		std::cout << "[";
		for (int j = 0; j<n; j++){
			std::cout << vect[i][j] << ",";
		}
		std::cout << "]";
		std::cout << "\n";

	}
	std::vector< std::vector<int> > vect3;
	vect3 = generateRsquare(n,n,time(0));
	for (int i = 0; i<n; i++){
		std::cout << i << " ";
		std::cout << "[";
		for (int j = 0; j<n; j++){
			std::cout << vect3[i][j] << ",";
		}
		std::cout << "]";
		std::cout << "\n";

	}
}

/*
/ Function: generatesquare
/ Parameters: 
/	int n <- dimension of vector
/	int m <- dimension of vector
/ Useage:
/		std::vector< std::vector<int> > vect = 
/						generatesquare(10, 15);
/ Return: n x m empty vector
*/ 

std::vector< std::vector<int> > generatesquare(int n, int m){
	std::vector< std::vector<int> > vect;
	for (int i = 0; i<n; i++){
		std::vector<int> row;
		for (int j = 0; j<m;j++){
			row.push_back(std::rand());
		}
		vect.push_back(row);
	}
	return vect;
}

/*
/ Function: nrands
/ Parameters:
/	int n <- ammount of rand ints to return
/	int s <- seed for rand
/ Useage:
/ 	std::vector<int> randv = nrand(10,time(0));
/ Return: n length std::vector of rand ints
*/ 

std::vector<int> nrands(int n, int s){
	std::vector<int> rd;
	std::srand(s);
	for(int i=0;i<n;i++)
		rd.push_back(std::rand());
	return rd;
}

/*
/ Function: generateRsquare
/ Parameters:
/	int n <- dimension of vector 
/	int m <- dimension of vector
/	int s <- seed for rand
/ Useage:
/	std::vector<int> rvect = generateRsquare(10,15,time(0))
/ Return: n x m vector of random ints
*/

std::vector< std::vector<int> > generateRsquare(int n,int m, int s){
	std::vector< std::vector<int> > vect;
	std::srand(s);
	for (int i = 0; i<n; i++){
		vect.push_back(nrands(m,s));
		s =	std::rand(); 	
	}
	return vect;
}
