#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

vector< vector<double> > data;
vector< vector<double> > features;

int fake_cross_validation(vector< vector<double> > data, vector<int> current_set, int feature_to_add){
    return rand() % 100;
}

bool is_in_set(int j, vector<int> current_set){
    
    for(int i = 0; i < current_set.size(); i++){
        if(current_set[i] == j){
            return true;
        }
    }
    return false;
}


void fwd_feature_search(vector< vector<double> > data){
    
    vector<int> current_set;
    int accuracy = 0;
    
    for(int i = 1; i < data[0].size(); i++){
        cout << "\nOn the " << i << "-th level of the search tree...\n";
        int feature_to_add_at_this_level;
        int best_so_far_accuracy = 0;
        for(int j = 1; j < data[0].size(); j++){
            if(is_in_set(j,current_set)){
                //dont assess it
            }
            else{
                cout << "\tConsidering adding the " << j << "-th feature\n";
                accuracy = fake_cross_validation(data,current_set,j);
                if(accuracy > best_so_far_accuracy){
                    best_so_far_accuracy = accuracy;
                    feature_to_add_at_this_level = j;
                }
            }
        }
        current_set.push_back(feature_to_add_at_this_level);
        cout << "On level " << i << " added the " << feature_to_add_at_this_level << "-th feature to current set!\n";
    }
    /*
    cout << "This is current set: \n";
    for(int i = 0; i < current_set.size(); i++){
        cout << current_set[i] << " ";
    }
    cout << endl;*/
    return;
}

void be_feature_search(vector< vector<double> > data){
    vector<int> current_set;
    int accuracy = 0;
    
    //populate current_set with all our features
    for(int i = 1; i < data[0].size(); i++){
        current_set.push_back(i);
    }

    for(int i = 1; i < data[0].size(); i++){
        cout << "\nOn the " << i << "-th level of the search tree...\n";
        int feature_to_remove_at_this_level;
        int best_so_far_accuracy = 0;
        for(int j = 1; j < data[0].size(); j++){
            if(is_in_set(j,current_set)){
                cout << "\tConsider removing the " << j << "-th feature\n";
                accuracy = fake_cross_validation(data,current_set,j);
                if(accuracy > best_so_far_accuracy){
                    best_so_far_accuracy = accuracy;
                    feature_to_remove_at_this_level = j;
                }
            }
        }
        for(int k = 0; k < current_set.size(); k++){
            if(current_set[k] == feature_to_remove_at_this_level){
                current_set.erase(current_set.begin()+k);
            }
        }
        cout << "On level " << i << " removed the " << feature_to_remove_at_this_level << "-th feature to current set!\n";
    }
    
    return;
}

int process_data(string filename){
    string line;
    int num_instances=0;
    int num_features=0;
    double curr_data;
    
    ifstream myfile (filename);
    if(myfile.is_open()){
        while(getline(myfile,line)){
            num_instances++;
            
            istringstream ss(line);
            string word;
            double num;
            vector<double> instances;
            while( ss >>  word){
                //cout << "pushing into instances: " << word << "\n";
                num = atof(word.c_str());
                instances.push_back(num);
            }
            
            num_features = instances.size()-1;
            //cout << "size of instances: " << instances.size() << "\n";
            data.push_back(instances);
        }
    }
    else{
        cout << "Error opening file!\n";
        return -1;
    }
    
    cout << "This dataset has " << num_features << " features (not including the class attribute), and has " << num_instances << " instances\n";
    return 0;
}

void forward_selection(){
    fwd_feature_search(data);
    return;
}

void backward_elimination(){
    be_feature_search(data);
    return;
}

int main(){
    srand (time(NULL)); //to get random number
    
    string filename;
    string f1 = "CS170_small_special_testdata__95.txt";
    string f2 = "CS170_largetestdata__56.txt";
    int algorithm = 1; //default to forward

    
    //intro
    cout << "===================================================\n";
    cout << "Welcome to the Feature Selection Algorithm Program!\n";
    cout << "===================================================\n";
    
    
    //input file
    cout << "Type in the name of the file to test: ";
    //cin >> filename;
    
    
    //default for testing
    cout << f1;
    filename = f1;

    
    //choose algorithm
    cout << "Type the number of the algoirthm you want to run: ";
    cout << "\n\t (1) Forward selection \n\t (2) Backward Elimination\n";
    cin >> algorithm;
    
    
    //process data
    cout << "\nProcessing data ... \n\t";
    if(process_data(filename) == -1){
        cout << "Exiting\n";
        return -1;
    };
    
    
    //print data for checking
    /*for(int i = 0; i < data.size(); i++){
        for(int j = 0; j < data[i].size(); j++){
            cout << data[i][j] << "   ";
        }
        cout << endl;
    }*/
    
    if(algorithm == 1){
        cout << "\nStarting forward selection...\n";
        forward_selection();
    }
    else if(algorithm == 2){
        cout << "\nStarting backward elimination...\n";
        backward_elimination();
    }
    
    cout << endl;
    return 0;
}
