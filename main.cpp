#include <iostream>
#include <string>
#include "inv_index.h"
#include <dirent.h>

using std::cout; using std::endl;
using std::string; using std::vector;
using namespace sdsl;


int main(){
  
  inv_index<string, int_vector<>> index_int;
  index_int.insert_doc("data/english.100MB", 1000);
  inv_index<string, enc_vector<coder::elias_gamma>> index_enc_gamma(index_int);
  inv_index<string, enc_vector<coder::elias_delta>> index_enc_delta(index_int);


  cout << "Para int_vec:\n";
  cout << "Sin comprimir: " << index_int.size() << endl;
  index_int.bit_compress();
  cout << "Comprimido: " << index_int.size() << endl;

  cout << "Para enc_vector con elias gamma:\n" << index_enc_gamma.size() << endl;
  cout << "Para enc_vector con elias delta:\n" << index_enc_delta.size() << endl;
  

 
  
  
  return 0;
}
