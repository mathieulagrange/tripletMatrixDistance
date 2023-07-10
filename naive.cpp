#include <iostream>
#include<Eigen/Dense>
#include<fstream>
#include <vector>
 
using namespace std;
using namespace Eigen;

void saveData(string fileName, MatrixXd  matrix)
{
    //https://eigen.tuxfamily.org/dox/structEigen_1_1IOFormat.html
    const static IOFormat CSVFormat(FullPrecision, DontAlignCols, ", ", "\n");
 
    ofstream file(fileName);
    if (file.is_open())
    {
        file << matrix.format(CSVFormat);
        file.close();
    }
}

MatrixXd openData(string fileToOpen)
{
 
    // the inspiration for creating this function was drawn from here (I did NOT copy and paste the code)
    // https://stackoverflow.com/questions/34247057/how-to-read-csv-file-and-assign-to-eigen-matrix
     
    // the input is the file: "fileToOpen.csv":
    // a,b,c
    // d,e,f
    // This function converts input file data into the Eigen matrix format
 
 
 
    // the matrix entries are stored in this variable row-wise. For example if we have the matrix:
    // M=[a b c 
    //    d e f]
    // the entries are stored as matrixEntries=[a,b,c,d,e,f], that is the variable "matrixEntries" is a row vector
    // later on, this vector is mapped into the Eigen matrix format
    vector<double> matrixEntries;
 
    // in this object we store the data from the matrix
    ifstream matrixDataFile(fileToOpen);
 
    // this variable is used to store the row of the matrix that contains commas 
    string matrixRowString;
 
    // this variable is used to store the matrix entry;
    string matrixEntry;
 
    // this variable is used to track the number of rows
    int matrixRowNumber = 0;
 
 
    while (getline(matrixDataFile, matrixRowString)) // here we read a row by row of matrixDataFile and store every line into the string variable matrixRowString
    {
        stringstream matrixRowStringStream(matrixRowString); //convert matrixRowString that is a string to a stream variable.
 
        while (getline(matrixRowStringStream, matrixEntry, ',')) // here we read pieces of the stream matrixRowStringStream until every comma, and store the resulting character into the matrixEntry
        {
            matrixEntries.push_back(stod(matrixEntry));   //here we convert the string to double and fill in the row vector storing all the matrix entries
        }
        matrixRowNumber++; //update the column numbers
    }
 
    // here we convet the vector variable into the matrix and return the resulting object, 
    // note that matrixEntries.data() is the pointer to the first memory location at which the entries of the vector matrixEntries are stored;
    return Map<Matrix<double, Dynamic, Dynamic, RowMajor>>(matrixEntries.data(), matrixRowNumber, matrixEntries.size() / matrixRowNumber);
 
}

float get_matrix_distance(MatrixXd rs, MatrixXd ps){
	int n_elts = rs.rows();
	int n_triplets = n_elts*(n_elts-1)*(n_elts-2)/6;
	// cout<<n_elts;
	// cout<<n_triplets;
	float d=0;
	for (int k=0 ; k<n_elts-2; k++){
		for (int l=k+1 ; l<n_elts-1; l++){
			for (int m=l+1 ; m<n_elts; m++){
				if (rs(k, l)>=rs(l, m) && ps(k, l)>=ps(l, m))
                        d+=1;
                if (rs(k, l)<rs(l, m) && ps(k, l)<ps(l, m))
                        d+=1;
			}
		}
	}
	return d/n_triplets;
}


int main()
{
// make sure that the "matrix_outside.csv" file exhists
    MatrixXd reference = openData("reference.csv");
 	MatrixXd predicted = openData("predicted.csv");
 
    // cout <<"\n\n"<< matrix_test3<<"\n\n";

	cout << get_matrix_distance(reference, predicted);

}