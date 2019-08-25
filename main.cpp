#include <json.hpp>
#include <iostream>
using json=nlohmann::json;
using namespace std;



int main(){

json j = json({"nuria","culo"});
cout<<j.dump(4)<<endl;
cout << endl;



return 0;
}