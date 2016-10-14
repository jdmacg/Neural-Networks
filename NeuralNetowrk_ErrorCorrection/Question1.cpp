#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <sstream>
#include <fstream>

using namespace std;

struct iris{
    string irisType;
    vector <double> data;
};

vector <iris> readFile(string fileName); //file input
pair<double,double> testANN (vector<iris> testData, vector<double> weights, string);

int main()
{
    vector<iris> trainingData = readFile("train.txt");
    vector<iris> testData = readFile("test.txt");
    vector<double> weights = {0.5,-0.333,-0.125};
    string flower = "Iris-setosa";
    pair<double,double> result = testANN(testData, weights, flower);
    cout << "Accuracy classifying " <<flower << ": %" << result.first*100<<". \nAccuracy classifying non " <<flower << ": %" << result.second*100 <<endl;
    return 0;
}

/*
Function to test if given ANN model classifies irises correctly.
Input: test data with flower calssified, weights, flower type to test for.
Ouputs: what percentage of test data was classified correctly by the model.
*/
pair<double,double> testANN (vector<iris> testData, vector<double> weights, string flowerType){
    double positiveIdsCorrect = 0, positiveIdsIncorrect = 0, negativeIdsCorrect = 0, negativeIdsIncorrect = 0;
    string correctType;
    double sum;
    for (int i = 0; i < testData.size(); i++){
       sum = weights[0]*1+weights[1]*testData[i].data[3]+testData[i].data[2]*weights[2];//formula for sum of weights and inputs with linear separability function
       correctType = testData[i].irisType;
        cout << "Data Point: "<<testData[i].data[0] << ", " << testData[i].data[1] << ", "<< testData[i].data[2] << ", "<< testData[i].data[3];
        if(sum > 0){
            cout<< "\tPredicted class: " << flowerType;
            if(correctType == flowerType){ // flower is actually of tested type
                positiveIdsCorrect++;cout<<". Correct"<<endl;}
            else{
                negativeIdsIncorrect++;cout<<". Incorrect"<<endl;}

        }
        else{ // sum is less than or equal to 0
            cout<< "\tPredicted class: not " << flowerType;
            if(correctType == flowerType){ // flower is actually of tested type

                positiveIdsIncorrect++;cout<<". Incorrect"<<endl;}
            else{
                negativeIdsCorrect++;cout<<". Correct"<<endl;}
        }

    }
    return make_pair<double,double>(positiveIdsCorrect/(positiveIdsCorrect+positiveIdsIncorrect),negativeIdsCorrect/(negativeIdsIncorrect+negativeIdsCorrect)); //calculate accuracy

}


vector<iris> readFile (string fileName){
    vector<iris> arr;
    vector<double> row(4);
    ifstream file(fileName);
    string line;
    double one, two, three, four; //holds the four data points of each Iris line
    string irisType;
    char comma; //skip commas
    while (getline(file, line))//get all lines from the file
    {
        //cout <<line<<endl;
        istringstream iss(line);
        if (!(iss >> one >> comma >> two >> comma >> three >> comma >> four >> comma >> irisType)) { cout<<"error"; break; }
        row[0]=one; row[1]=two; row[2]=three; row[3]=four;
        iris flower{irisType,row};
        arr.push_back(flower);
    }
    return arr;
}
