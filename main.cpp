#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>

using namespace std;

vector< vector<double> > data;
vector< vector<double> > features;

vector< vector<double > > adjusted_data;

bool exists_in(int value, vector<int> v){
    for(int i = 0; i < v.size(); i++){
        if(value == v[i]){
            return true;
        }
    }
    return false;
}

void clean_data(vector< vector< double > > data, vector<int> current_set, int feature_to_add){
    current_set.push_back(feature_to_add);
    adjusted_data = data;
    
    //cout << "\nTHIS IS OUR CURRENT SET: \n";
    for(int i = 0; i < current_set.size(); i++){
        cout << current_set[i] << " " ;
    }
    
//    for(int i = 0; i < current_set.size(); i++){
//        //everything not in the current_set, set to 0
//        for(int j = 0; j < data.size(); j++){
//            for(int k = 0; k < data[i].size(); k++){
//                if(current_set[i] != k){
//                    adjusted_data[j][k] = 0;
//                }
//            }
//        }
//    }
    
    for(int i = 0; i < data.size(); i++){
        for(int j = 0; j < data[i].size(); j++){
            //adjusted_data[i][j] = 0;
            if(exists_in(j, current_set)){
                //leave it alone! we want it
            }
               else{
                adjusted_data[i][j] = 0;
            }
        }
    }
    
    
//    cout << "\nNew data array: \n";
//    for(int i = 0; i < adjusted_data.size(); i++){
//        for(int j = 0; j < adjusted_data[i].size(); j++){
//            cout << adjusted_data[i][j] << "    ";
//        }
//        cout << "\n";
//    }

    return;
}

double calculate_distance(vector<double> object_to_classify, vector<double> neighbor_k){
    double distance = 0;
    double x;
    
    //DONT incluse the class column
    for(int i = 1; i < object_to_classify.size(); i++){
        //cout << "doing: (" << object_to_classify[i] << " - " << neighbor_k[i] << ")^2\n";
        x = pow(object_to_classify[i] - neighbor_k[i],2);
        //cout << "Adding: " << x << "\n";
        distance += x;
    }
//    cout << "Distance: " << distance << "\n";
//    cout << "sqrt: " << sqrt(distance) << "\n";
    return sqrt(distance);
}


double calculate_accuracy(vector< vector<double> > data, vector<int> current_set, int feature_to_add){
    clean_data(data, current_set, feature_to_add);
    vector<double> object_to_classify;
    int label_object_to_classify = 0;
    int label_nearest_neighbor = 0;
    
    double number_correctly_classified = 0;
    
    //double distance = 0;
    //adjust data
    
    //get accuracy
    for(int i = 0; i < data.size(); i++){ //# of total data
        object_to_classify = adjusted_data[i];
        label_object_to_classify = data[i][0];
        
        double nearest_neighbor_distance = 90000;
        double nearest_neighbor_location = 90000;
        double distance = 0;
        //cout << "\tLooping over i, at the " << i+1 << "-th location\n";
        //cout << "\t\tThe " << i << "-th object is in class " << data[i][0] << "\n";
        for(int k = 0; k < data.size(); k++){ //# of total data
            if(!(k == i)){
                //cout << "\tAsk if object " << i+1 << " is nearest neighbor with " << k+1 <<"\n";
                distance = calculate_distance(object_to_classify, adjusted_data[k]);
//                for(int h = 0; h < object_to_classify.size(); h++){
//                    distance = distance + pow((object_to_classify[h]-data[k][h]),2);
//                }
//                distance = sqrt(distance);
                if(distance < nearest_neighbor_distance){
                    nearest_neighbor_distance = distance;
                    nearest_neighbor_location = k;
                    label_nearest_neighbor = data[k][0];
                }
            }
        }
//        cout << "\nObject " << i << " is class " << label_object_to_classify;
//        cout << "\nIts nearest_neighbor is " << nearest_neighbor_location << " which is in class " << label_nearest_neighbor << "\n";
        if(label_object_to_classify == label_nearest_neighbor){
            number_correctly_classified++;
            //cout << "Adding\n";
        }
        
    }
    //cout << "Accuracy is: " << (number_correctly_classified / data.size())*100 << "\n";
    return (number_correctly_classified / data.size())*100;
}

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
    vector<int> whatif_current_set;
    double highest_accuracy_reached = 0;
    vector<int> best_set;
    double accuracy = 0;
    
    for(int i = 1; i < data[0].size(); i++){ //# of features
        cout << "\nOn the " << i << "-th level of the search tree...\n";
        int feature_to_add_at_this_level;
        double best_so_far_accuracy = 0;
        for(int j = 1; j < data[0].size(); j++){
            if(is_in_set(j,current_set)){ //# of features
                //dont assess it
            }
            else{
                //cout << "\tConsidering adding the " << j << "-th feature\n";
                //accuracy = fake_cross_validation(data,current_set,j);
                //whatif_current_set = current_set;
                //whatif_current_set.push_back(j);
                //clean_data(data, whatif_current_set);
                accuracy = calculate_accuracy(data, current_set, j);
                //accuracy = new_accuracy(data, current_set, j);
                cout << "\tUsing features(s) {" << j <<"} accuracy is " << accuracy << "%\n";
                if(accuracy > best_so_far_accuracy){
                    best_so_far_accuracy = accuracy;
                    feature_to_add_at_this_level = j;
                    if(best_so_far_accuracy > highest_accuracy_reached){
                        //best_set = current_set;
                        highest_accuracy_reached = best_so_far_accuracy;
                    }
                }
            }
        }
        current_set.push_back(feature_to_add_at_this_level);
        cout << "Feature set {" << feature_to_add_at_this_level << "} was best at an accuarcy of " << best_so_far_accuracy << "%\n\n";
        cout << "On level " << i << " added the " << feature_to_add_at_this_level << "-th feature to current set!\n";
    }
    cout << "Search completed.\nThe best feature subset is {";
    for(int i = 0; i < best_set.size(); i++){
        cout << best_set[i] << " ";
    }
    cout << "} with an accuracy of " << highest_accuracy_reached << "%!\n";
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

int main(){
    srand (time(NULL)); //to get random number
    
    string filename;
    string f1 = "CS170_small_special_testdata__95.txt";
    string f2 = "CS170_largetestdata__56.txt";
    string f3 = "my_fake_set.txt";
    string f4 = "CS170_small_special_testdata__96.txt";
    string f5 = "CS170_small_special_testdata__97.txt";
    string f6 = "CS170_small_special_testdata__98.txt";
    string f7 = "CS170_small_special_testdata__99.txt";
    int algorithm = 1; //default to forward

    
    //intro
    cout << "===================================================\n";
    cout << "Welcome to the Feature Selection Algorithm Program!\n";
    cout << "===================================================\n";
    
    
    //input file
    cout << "Type in the name of the file to test: ";
    cin >> filename;
    
    
    //FOR TESTING ONLY::: default for testing
    filename = f3;

    
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
    
    
    if(algorithm == 1){
        cout << "\nStarting forward selection...\n";
        fwd_feature_search(data);
        
    }
    else if(algorithm == 2){
        cout << "\nStarting backward elimination...\n";
        be_feature_search(data);
    }
    
    cout << endl;
    return 0;
}
