#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <sstream>
#include <fstream>
#include <math.h>
#include<cassert>

using namespace std;

vector<vector<int> > readFile(string fileName);
vector<vector<int> > calculateOutputVectors(vector<vector<int> > numbers);
vector<vector<double> > calculateSums(vector<vector<vector<double> > >weights, vector<int> inputs);
vector<vector<vector<double> > > backPropagation(vector<vector<vector<double> > >weights, vector<vector<int> >inputs);
double sumSquareError (vector<double> calculated, vector<double> desired);

const int NUM_OUTPUT_NODES = 10;
const int NUM_HIDDEN_NODES = 12;
const int NUM_INPUT_NODES = 64;

int main()
{
    vector<vector<int> > numbers = readFile("training.txt");
    vector<vector<int> > d = calculateOutputVectors(numbers);//output vectors
    ofstream output;
    output.open ("output.txt");

    vector<vector<vector<double> > > weights {vector<vector<double> >(12, vector<double>(65,0)),vector<vector<double> >(10, vector<double> (13,0))};
    /*
    for (int i = 0; i < 12; i++){
        weights[0][i]= new vector<double> weights (64,0);
    }
    for (int i = 0; i < 10; i++){
        weights[1][i]= new vector<double> weights (12,0);
    }
    */

    for(int i = 0; i < weights[0].size();i++){
        for(int j = 0; j < 65; j++){
            output<< weights[0][i][j]<<" ";
        }
        output<<endl;
    }


    vector<vector<double> > nodeValues = calculateSums(weights, numbers[0]);

    weights = backPropagation(weights,numbers,d);



    for(int i = 0; i < d.size(); i ++){
        for(int j = 0; j < d[i].size(); j++){
            output<<d[i][j]<<" ";
        }
        output<< " number: " << numbers[i][64]<<endl;
    }

    output.close();
    return 0;
}

vector<vector<vector<double> > > backPropagation(vector<vector<vector<double> > >weights, vector<vector<int> > inputs, vector<vector<int> > outputs){
    double learningRate = 0.1;
    double errorSum = 9999999999999; //set as very for first iteration
    int interationCount = 0;
    vector<vector<double> > y; //output at each node
    while(interationCount<1000){
        for(int inputIterator = 0; inputIterator < inputs.size(); i++){
            y = calculateSums(weights, inputs[i]);
            for(int nodeIteratorJ = 0; nodeIteratorJ < NUM_OUTPUT_NODES; nodeIteratorJ++){
                for(int nodeIteratorI = 0; nodeIteratorI <NUM_HIDDEN_NODES+1; nodeIteratorI++){
                    assert(y[0].size()=13);
                    double yj = y[1][nodeIteratorJ];
                    double dj = outputs[nodeIteratorJ];
                    double xi = y[0][nodeIteratorI]
                    double deltaW = learningRate*xi*(dj-yj)*yj*(1-yj);
                    weights[1][nodeIteratorJ][nodeIteratorI]+=deltaW;
                }
            }
            //*********EDIT HERE ********************************
            for(int nodeIteratorJ = 0; nodeIteratorJ < NUM_HIDDEN_NODES; nodeIteratorJ++){
                for(int nodeIteratorI = 0; nodeIteratorI <NUM_INPUT_NODES+1; nodeIteratorI++){
                    double yj = y[1][nodeIteratorJ];
                    double dj = outputs[nodeIteratorJ];
                    double xi = y[0][nodeIteratorI]
                    double deltaW = learningRate*xi*(dj-yj)*yj*(1-yj);
                    weights[1][nodeIteratorJ][nodeIteratorI]+=deltaW;
                }
            }

        }
    }
}


vector<vector<int> > readFile(string fileName){
    vector<vector<int> > numbers;
    vector<int> row(65);
    ifstream file(fileName);
    string line;
    char comma; //hold comma separator
    while(getline(file, line)){
        istringstream iss(line);
        for(int i = 0; i < 65;i++){
            iss>>row[i];
            iss>>comma;
        }
        numbers.push_back(row);
    }
    return numbers;
}



double sumSquareError(vector<double> calculated, vector<double> desired){
    assert(desired.size()==calculated.size());
    double sum = 0;
    for(int i = 0; i < desired.size(); i++){
        sum+=pow((calculated[i]-desired[i]),2);
    }
    return sum;
}

//12 hidden nodes, 10 output nodes, 64 input nodes
vector<vector<double> > calculateSums(vector<vector<vector<double> > >weights, vector<int> inputs){
    vector<vector<double> > sums;
    int layer = 0; //layer between
    vector<double> layerOneSums (13,0);
    for(int nodeIterOuter = 0; nodeIterOuter < 12; nodeIterOuter++){ //first layer
        for(int nodeIterInner = 0; nodeIterInner < 64; nodeIterInner++){
            layerOneSums[nodeIterOuter] += inputs[nodeIterInner]*weights[layer][nodeIterOuter][nodeIterInner];
        }
        layerOneSums[nodeIterOuter] += weights[layer][nodeIterOuter][64];//add bias
        layerOneSums[nodeIterOuter] = 1/(1+exp(-layerOneSums[nodeIterOuter]));//apply sigmoid function
    }
    sums[12]=1;//bias
    sums.push_back(layerOneSums);

    cout << endl;

    layer = 1; //layer between hidden layer and output
    vector<double> layerTwoSums (10,0);
    for(int nodeIterOuter = 0; nodeIterOuter < 10; nodeIterOuter++){ //first layer
        for(int nodeIterInner = 0; nodeIterInner < 12; nodeIterInner++){
            layerTwoSums[nodeIterOuter] += layerOneSums[nodeIterInner]*weights[layer][nodeIterOuter][nodeIterInner];
        }
        layerTwoSums[nodeIterOuter] += weights[layer][nodeIterOuter][12];//add bias
        layerTwoSums[nodeIterOuter] = 1/(1+exp(-layerTwoSums[nodeIterOuter]));//apply sigmoid function
    }
    sums[10]
    sums.push_back(layerTwoSums);

    return sums;
}

vector<vector<int> > calculateOutputVectors(vector<vector<int> > numbers){
    vector<vector<int> > outputVector;
    vector<int> zero = {1,0,0,0,0,0,0,0,0,0};
    vector<int> one = {0,1,0,0,0,0,0,0,0,0};
    vector<int> two = {0,0,1,0,0,0,0,0,0,0};
    vector<int> three = {0,0,0,1,0,0,0,0,0,0};
    vector<int> four = {0,0,0,0,1,0,0,0,0,0};
    vector<int> five = {0,0,0,0,0,1,0,0,0,0};
    vector<int> six = {0,0,0,0,0,0,1,0,0,0};
    vector<int> seven = {0,0,0,0,0,0,0,1,0,0};
    vector<int> eight = {0,0,0,0,0,0,0,0,1,0};
    vector<int> nine = {0,0,0,0,0,0,0,0,0,1};
    for(int i = 0; i < numbers.size(); i++){
        switch(numbers[i][64]){
            case 0:
                outputVector.push_back(zero);
                break;
            case 1:
                outputVector.push_back(one);
                break;
            case 2:
                outputVector.push_back(two);
                break;
            case 3:
                outputVector.push_back(three);
                break;
            case 4:
                outputVector.push_back(four);
                break;
            case 5:
                outputVector.push_back(five);
                break;
            case 6:
                outputVector.push_back(six);
                break;
            case 7:
                outputVector.push_back(seven);
                break;
            case 8:
                outputVector.push_back(eight);
                break;
            case 9:
                outputVector.push_back(nine);
                break;
        }
    }
    return outputVector;
}
