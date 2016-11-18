#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <sstream>
#include <fstream>
#include <math.h>
#include <cassert>
#include <cstdlib>
#include <ctime>


using namespace std;

vector<vector<int> > readFile(string fileName);
vector<vector<int> > calculateOutputVectors(vector<vector<int> > numbers);
vector<vector<double> > calculateSums(vector<vector<vector<double> > >weights, vector<int> inputs);
vector<vector<vector<double> > > backPropagation(vector<vector<vector<double> > >weights, vector<vector<int> >inputs,vector<vector<int> > outputs);
double sumSquareError (vector<double> calculated, vector<int>desired);
vector<vector<int> > addBias (vector<vector<int> > input);
vector<vector<vector<double> > > randomizeWeights();
double calculateAccuracy(vector<vector<vector<double> > >weights, vector<vector<int> > inputs, vector<vector<int> > outputs);

const int NUM_OUTPUT_NODES = 10;
const int NUM_HIDDEN_NODES = 12;
const int NUM_INPUT_NODES = 64;
const double GLOBAL_ERROR_ADJUSTMENT = 0.1;

int main()
{
    vector<vector<int> > train = readFile("training.txt");
    vector<vector<int> > d = calculateOutputVectors(train);//output vectors

    train = addBias(train);
    ofstream output;
    output.open ("output.txt");

    vector<vector<int> > test = readFile("testing.txt");
    vector<vector<int> > dTest = calculateOutputVectors(test);
    test = addBias(test);

    vector<vector<vector<double> > > weights = randomizeWeights();

    weights = backPropagation(weights,train,d);
    output << "Accuracy of model on test data: "<<calculateAccuracy(weights, test, dTest);

    output.close();

    return 0;
}

/*
Performs backpropagation to adjust weights of a neural network
Inputs: 3d vector of weights - randomly initialized, input into the neural network, initilized vector of outputs from the network
Outputs: returns the updated weights vector
*/
vector<vector<vector<double> > > backPropagation(vector<vector<vector<double> > >weights, vector<vector<int> > inputs, vector<vector<int> > outputs){
    double learningRate = 0.05;
    double momentum = 0.2;
    double errorSum = 9999999999999; //set as very for first iteration
    int iterationCount = 0;
    vector<vector<double> > y; //output at each node
    ofstream output;
    output.open ("functionOutput.txt");
    double currentErrorSum;
    vector<vector<vector<double> > > deltaWHistory {vector<vector<double> >(NUM_HIDDEN_NODES, vector<double>(NUM_INPUT_NODES+1,0)),vector<vector<double> >(NUM_OUTPUT_NODES, vector<double> (NUM_HIDDEN_NODES+1,0))};
    while(iterationCount<300&&errorSum>100){
        currentErrorSum = 0;
        for(int inputIterator = 0; inputIterator < inputs.size(); inputIterator++){
            y = calculateSums(weights, inputs[inputIterator]);
            for(int nodeIteratorJ = 0; nodeIteratorJ < NUM_OUTPUT_NODES; nodeIteratorJ++){
                for(int nodeIteratorI = 0; nodeIteratorI <NUM_HIDDEN_NODES+1; nodeIteratorI++){
                    double yj = y.at(1).at(nodeIteratorJ);
                    double dj = outputs.at(inputIterator).at(nodeIteratorJ);
                    double xi = y.at(0).at(nodeIteratorI);
                    double e = 0;
                    if ((dj == 1&&yj<dj-GLOBAL_ERROR_ADJUSTMENT)||(dj==0&&yj>0+GLOBAL_ERROR_ADJUSTMENT)){
                        e = dj-yj;
                    }
                    double deltaW = learningRate*xi*(e)*yj*(1-yj)+momentum*deltaWHistory.at(1).at(nodeIteratorJ).at(nodeIteratorI);
                    weights.at(1).at(nodeIteratorJ).at(nodeIteratorI)+=deltaW;
                    deltaWHistory.at(1).at(nodeIteratorJ).at(nodeIteratorI) = deltaW;
                }
            }
            y = calculateSums(weights, inputs[inputIterator]);
            for(int nodeIteratorHidden = 0; nodeIteratorHidden < NUM_HIDDEN_NODES; nodeIteratorHidden++){
                for(int nodeIteratorInput = 0; nodeIteratorInput <NUM_INPUT_NODES+1; nodeIteratorInput++){
                    double y1i = y.at(0).at(nodeIteratorHidden);
                    double fpa = y1i*(1-y1i);
                    double sumDeltaH = 0; // holds the value of the summation in the delta w formula for layers 1-0;
                    for(int nodeIteratorOutput = 0; nodeIteratorOutput < NUM_OUTPUT_NODES; nodeIteratorOutput++){
                        double dj = outputs.at(inputIterator).at(nodeIteratorOutput);
                        double y2j = y.at(1).at(nodeIteratorOutput);
                        double e = 0;
                        if ((dj == 1 && y2j < dj-GLOBAL_ERROR_ADJUSTMENT)||(dj==0 && y2j > 0 + GLOBAL_ERROR_ADJUSTMENT)){
                            e = dj-y2j;
                        }
                        sumDeltaH += (e)*y2j*(1-y2j)*weights.at(0).at(nodeIteratorOutput).at(nodeIteratorHidden);
                    }
                    double xi = inputs.at(inputIterator).at(nodeIteratorInput);
                    double deltaW = learningRate*xi*sumDeltaH*fpa+momentum*deltaWHistory.at(0).at(nodeIteratorHidden).at(nodeIteratorInput);
                    weights.at(0).at(nodeIteratorHidden).at(nodeIteratorInput)+=deltaW;
                    deltaWHistory.at(0).at(nodeIteratorHidden).at(nodeIteratorInput) = deltaW;
                }
            }

            y=calculateSums(weights, inputs[inputIterator]);
            currentErrorSum += sumSquareError(y.at(1),outputs.at(inputIterator));
        }
        output<<currentErrorSum<<endl;
        errorSum=currentErrorSum;
        iterationCount++;
        output<<iterationCount<<endl;
        cout <<iterationCount<<" "<<errorSum<<endl;
    }
    return weights;
}

/*
Uses weights determined by backpropagation to test the accuracy of the model on the test data
Inputs: 3d weight vector, input data vector, expected output vector.
Ouput: accuracy
*/
double calculateAccuracy(vector<vector<vector<double> > >weights, vector<vector<int> > inputs, vector<vector<int> > outputs){
    double counter=0;
    double correctConuter = 0;
    vector<vector<double> > y;
    for(int inputIterator = 0; inputIterator < inputs.size(); inputIterator++){
        y = calculateSums(weights, inputs[inputIterator]);
        int guess;
        double maxNum = 0;
        for(int yIterator = 0; yIterator<y[1].size(); yIterator++){
            if(y.at(1).at(yIterator)>maxNum){
                guess = yIterator;
                maxNum = y.at(1).at(yIterator);
            }
        }
        if(outputs.at(inputIterator).at(guess)==1){
            correctConuter++;
        }
        counter++;
    }
    return correctConuter/counter;
}


/*
Removes correct label/number from input vector of pixels and adds bias input of value 1
Input: 2d vector of pixels input and label of size 65
Ouput: 2d vector of pixels with bias input added
*/
vector<vector<int> > addBias (vector<vector<int> > input){
    for(int i = 0; i < input.size(); i++){
        input.at(i).at(64) = 1;
    }
    return input;
}


/*
Initlizes weight vector to random values
Input: none
Output: 3d weight vector
*/
vector<vector<vector<double> > > randomizeWeights(){
    vector<vector<vector<double> > > weights {vector<vector<double> >(12, vector<double>(65,0)),vector<vector<double> >(10, vector<double> (13,0))};
    int layer = 0;
    srand(time(0));
    for(int layer = 0; layer <= 1; layer ++){
        for(int i = 0; i < weights.at(layer).size(); i++){
            for(int j = 0; j < weights.at(layer).at(i).size(); j++){
                double randomVariable = double(rand());
                randomVariable=((randomVariable/RAND_MAX)-(0.5))/10;
                weights.at(layer).at(i).at(j) = randomVariable;
            }
        }
    }
    return weights;
}

/*
file I/O
*/
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

/*
Calculate the sum square error on one data point
*/
double sumSquareError(vector<double> calculated, vector<int> desired){
    assert(desired.size()==calculated.size());
    double sum = 0;
    for(int i = 0; i < desired.size(); i++){
        if(desired.at(i)==1){
            if(calculated.at(i)<1-GLOBAL_ERROR_ADJUSTMENT)
                sum+=pow((calculated.at(i)-desired.at(i)),2);
        }
        else{// desired == 0
            if(calculated.at(i)>0+GLOBAL_ERROR_ADJUSTMENT)
                sum+=pow((calculated.at(i)-desired.at(i)),2);
        }

    }
    return sum;
}

//12 hidden nodes, 10 output nodes, 64 input nodes
vector<vector<double> > calculateSums(vector<vector<vector<double> > >weights, vector<int> inputs){
    vector<vector<double> > sums;
    int layer = 0; //layer between
    vector<double> layerOneSums (NUM_HIDDEN_NODES+1,0);
    for(int nodeIterOuter = 0; nodeIterOuter < NUM_HIDDEN_NODES; nodeIterOuter++){ //first layer
        for(int nodeIterInner = 0; nodeIterInner < NUM_INPUT_NODES+1; nodeIterInner++){
            layerOneSums[nodeIterOuter] += inputs[nodeIterInner]*weights[layer][nodeIterOuter][nodeIterInner];
        }
        layerOneSums[nodeIterOuter] = 1/(1+exp(-layerOneSums[nodeIterOuter]));//apply sigmoid function
    }
    layerOneSums.at(NUM_HIDDEN_NODES)=1;
    sums.push_back(layerOneSums);

    layer = 1; //layer between hidden layer and output
    vector<double> layerTwoSums (NUM_OUTPUT_NODES,0);
    for(int nodeIterOuter = 0; nodeIterOuter < NUM_OUTPUT_NODES; nodeIterOuter++){ //first layer
        for(int nodeIterInner = 0; nodeIterInner < NUM_HIDDEN_NODES+1; nodeIterInner++){
            layerTwoSums[nodeIterOuter] += layerOneSums[nodeIterInner]*weights[layer][nodeIterOuter][nodeIterInner];
        }
        layerTwoSums[nodeIterOuter] = 1/(1+exp(-layerTwoSums[nodeIterOuter]));//apply sigmoid function
    }
    sums.push_back(layerTwoSums);

    return sums;
}


/*Given a vactor of the numbers read in from the file, this function outputs the desired neural network outputs
Input: 2d vector of data read in from the input file
Ouput: 2d vector of integers. each vector is of length 10
*/

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
