#include <json.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <set>
using json=nlohmann::json;
using namespace std;



int main(){

  std::ifstream i("lex.json");
  json jlex;
  i >> jlex;
  //C++17 structured bindings
  for ( const auto &[k,v] : jlex.items())
  {
    cout << k << " : " << v.dump(4) << endl;
  }

  const auto & match  = jlex.find("tags_to_map");

  set<string> sv;
  if(match != jlex.end()){
    //hay cosas
    sv = match.value().get<set<string>>();
  }

  sv.find()


  std::ifstream ifs ("html", std::ifstream::in);

  // char c = ifs.get();

  // while (ifs.good()) {
  //   //std::cout << c;
  //   c = ifs.get();

  // }

  ifs.close();


return 0;
}