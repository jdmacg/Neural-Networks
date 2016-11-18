%{
CMPE 452 Assignment 3
PCA Network
Jordan Mcgergor - 10052770
The purpose of this program is to isolate one sound source from a wav audio
file with two sound sources and two microphones. This is accomlished by
applying Principal Component Analysis to a network to find the
approximation of the first principal component. This is used to generate a
CSV with the isolated sound which is then converted to a wav file for
playback.
Input: wav file converted into a csv file, range of data values between 1 and -1
Output: wav file and csv file with one source isolated
%}

clear;
soundCSV = csvread('sound.csv');
sum = [0,0];
for i = 1:size(soundCSV) %add up all the data from the imported csv
    sum = sum + soundCSV(i,:);
end
mean = sum/size(soundCSV);
error = soundCSV; %start to calculate the error. first copy the original values
for i = 1:size(soundCSV)
    error(i,1:2) = error(i,:) - mean;
end 
learningRate = 0.1; %set learning rate to 0.1 as suggested
weight = [1,0]; %holds the calculated weights while being being updated during the iteration
currentIteration = 1;
maxIterations = 1; %set max iterations to 1 as suggested
while currentIteration <= maxIterations
    for i = 1:size(soundCSV)
        y = dot(error(i,:),weight);
        deltaW = learningRate*(y*error(i,:)-y^2*weight);  %deltaW =  n*y*x-K*W
        weight = weight + deltaW; %adjust weight vector
    end 
    currentIteration = currentIteration + 1;
end 
for i = 1:size(soundCSV)
    y(i) = dot(weight,error(i,:)); %the output is the sum of weights plus the inputs (or the dot product of weights with inputs)
end
fileID = fopen('weights.txt','w');
fprintf(fileID,'%8.4f',weight);
fclose(fileID);
csvwrite('output.csv',y);
audiowrite('output.wav',y,8192);