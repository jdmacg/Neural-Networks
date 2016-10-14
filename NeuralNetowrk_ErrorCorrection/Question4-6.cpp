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

vector <iris> readFileIgnoreType(string fileName, string ignoreType); //file input
pair<double,double> testANN (vector<iris> testData, vector<double> weights, string flowerType);
vector <double> adaline (vector<iris> testData,string, double);

int main()
{
    vector<iris> trainingData = readFileIgnoreType("train.txt", "Iris-setosa");
    vector<iris> testData = readFileIgnoreType("test.txt", "Iris-setosa");
    string flower1 = "Iris-versicolor";
    string flower2 = "Iris-virginica";
    double learningRate = 0.0005;
    vector<double> weights = adaline(trainingData,flower2,learningRate);
    cout << "The weight vector found was {"<< weights[0]<<", "<<weights[1]<<", "<<weights[2]<<", "<<weights[3]<<", "<<weights[4]<<"}"<<endl;
    pair<double,double> result = testANN(trainingData, weights, flower2);
    cout << "Accuracy classifying " <<flower2 << ": %" << result.first*100<<". \nAccuracy classifying non " <<flower2 << ": %" << result.second*100 <<endl;
    return 0;
}


/*
Function for error correction learning
Inputs: Test data and the flower type to test for.
Outputs: Weight vector of the perceptron.
*/
vector <double> adaline(vector<iris> testData, string flowerType, double learningRate){
    int counter = 0;
    bool perfectSeperation = false;
    string correctType;
    double sum;
    double error, meanSqError; // set to very big number
    double errorTotal = 9999999; // some big number
    double oldErrorTotal;
    int expected;
    vector <double> weights = {1,-1,2,-1,1};
    while(counter < 1000000 && errorTotal != oldErrorTotal){ // loop until mean sqare error does not change. limit number of iterations
        oldErrorTotal = errorTotal; // update total mean square error to compare
        errorTotal = 0;
        for(int i = 0; i < testData.size(); i++){
            sum = weights[0]*testData[i].data[0]+weights[1]*testData[i].data[1]+weights[2]*testData[i].data[2]+
            weights[3]*testData[i].data[3]+weights[4];//multiply weights by inputs
            correctType = testData[i].irisType; // what the flower shoudl be classified as
            //cout << "correct type: "<<correctType << " sum: " << sum;
            correctType == flowerType? expected = 1: expected = -1; // determine if sum is expected to be +ve or -ve
            //cout << " expected: "<< expected<<endl;
            error = double(expected) - sum;
            meanSqError = error*error;
            errorTotal+=meanSqError;
            //if((sum > 0 && expected== -1) || (sum < 0 && expected == 1)){ // if flower type is not classified correctly

                    weights[0] += testData[i].data[0]*learningRate*error;
                    weights[1] += testData[i].data[1]*learningRate*error;
                    weights[2] += testData[i].data[2]*learningRate*error;
                    weights[3] += testData[i].data[3]*learningRate*error;
                    weights[4] += learningRate*error; // bias
            //}

        }

        errorTotal/=testData.size(); // calculate total mean square error
        counter++;
    }
    cout << "Iterations: "<<counter<< endl;
    return weights;
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
        sum = weights[0]*testData[i].data[0]+weights[1]*testData[i].data[1]+weights[2]*testData[i].data[2]+
            weights[3]*testData[i].data[3]+weights[4];//multiply weights by inputs
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

//Function to read file data while ignoring supplied plant type
vector<iris> readFileIgnoreType (string fileName, string ignoreType){
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
        if(irisType != ignoreType){//skip ignored plant type
            row[0]=one; row[1]=two; row[2]=three; row[3]=four;
            iris flower{irisType,row};
            arr.push_back(flower);
        }
    }
    return arr;
}
