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
pair<double,double> testANN (vector<iris> testData, vector<double> weights, string flowerType);
vector <double> feedbackPerceptron (vector<iris> testData,string, double);

int main()
{
    vector<iris> trainingData = readFile("train.txt");
    vector<iris> testData = readFile("test.txt");
    string flower = "Iris-setosa";
    double learningRate = 0.1;
    vector<double> weights = feedbackPerceptron(trainingData,flower,learningRate);
    pair<double,double> result = testANN(trainingData, weights, flower);
    cout << "The weight vector found was {"<< weights[0]<<", "<<weights[1]<<", "<<weights[2]<<", "<<weights[3]<<", "<<weights[4]<<"}"<<endl;
    cout << "Accuracy classifying " <<flower << ": %" << result.first*100<<". \nAccuracy classifying non " <<flower << ": %" << result.second*100 <<endl;
    return 0;
}


/*
Function to learn to correct linear seperation of plants.
Inputs: Test data and the flower type to test for.
Outputs: Weight vector of the perceptron.
*/
vector <double> feedbackPerceptron (vector<iris> testData, string flowerType, double learningRate){
    int counter = 0;
    bool perfectSeperation = false;
    string correctType;
    double sum;
    vector <double> weights = {1,1,1,1,1};
    while(!perfectSeperation && counter < 1000){ // loop until testData is perfectly separated. limit number of iterations
        perfectSeperation = true; // testing for perfect seperation
        for(int i = 0; i < testData.size(); i++){
            sum = weights[0]*testData[i].data[0]+weights[1]*testData[i].data[1]+weights[2]*testData[i].data[2]+
            weights[3]*testData[i].data[3]+weights[4];//multiply weights by inputs
            correctType = testData[i].irisType; // what the flower shoudl be classified as
            if(sum > 0){ // weights currently classify data point as the flowerType
                if(correctType != flowerType){ // data point is not actually the flower type
                    weights[0] -= testData[i].data[0]*learningRate;
                    weights[1] -= testData[i].data[1]*learningRate;
                    weights[2] -= testData[i].data[2]*learningRate;
                    weights[3] -= testData[i].data[3]*learningRate;
                    weights[4] -= learningRate; // bias
                    perfectSeperation = false; //perfect seperation not found
                }
            }
            else{ // weights currently classify data point as not the flowerType
                if(correctType == flowerType){ // data point is actually the flower type
                    weights[0] += testData[i].data[0]*learningRate;
                    weights[1] += testData[i].data[1]*learningRate;
                    weights[2] += testData[i].data[2]*learningRate;
                    weights[3] += testData[i].data[3]*learningRate;
                    weights[4] += learningRate; // bias
                    perfectSeperation = false; //perfect seperation not found
                }
            }

        }
        counter++;
    }
    cout << "counter value: "<<counter << endl;
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
