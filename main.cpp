#include <iostream>
#include <string>
#include "inv_index.h"
#include <time.h>

using std::cout; using std::endl;
using std::string; using std::vector;
using namespace sdsl;


int main(){

  clock_t start, end;
  
  inv_index<string, int_vector<>> index_int;

  // Insertar archivo al indice invertido
  index_int.insert_doc("data/english.100MB", 1000);

  // Implementaciones con enc_vector
  inv_index<string, enc_vector<coder::elias_gamma>> index_enc_gamma(index_int);
  inv_index<string, enc_vector<coder::elias_delta>> index_enc_delta(index_int);


  // Nombres de docs al hacer merge
  vector<int> docs;
  


  cout << "Espacio utilizado (MBytes)\n\nPara int_vec:\n";
  cout << "Sin comprimir: " << index_int.size() << endl;
  index_int.bit_compress();
  cout << "Comprimido: " << index_int.size() << endl;

  cout << "Para enc_vector con elias gamma:\n" << index_enc_gamma.size() << endl;
  cout << "Para enc_vector con elias delta:\n" << index_enc_delta.size() << endl;

  start = clock();
  for(int i = 0; i < 100; i++){
    docs = index_int.merge("my", "name");
  }
  end = clock();

  cout << "\n\nTiempo merge\n\n";
  
  cout << "merge con int_vector: " << (double)(end - start) / CLOCKS_PER_SEC / 100 << endl;

  start = clock();
  for(int i = 0; i < 100; i++){
    index_enc_gamma.merge("my", "name");
  }
  end = clock();

  cout << "merge con enc_vector y elias gamma: " << (double)(end - start) / CLOCKS_PER_SEC / 100 << endl;

  start = clock();
  for(int i = 0; i < 100; i++){
    index_enc_delta.merge("my", "name");
  }
  end = clock();

  cout << "merge con enc_vector y elias delta: " << (double)(end - start) / CLOCKS_PER_SEC / 100 << endl;
 

  /*
  cout << "ndocs obtenidos del merge: " << docs.size() << endl;
  
  // Obtener nombres de los documentos que devuelve el merge
  for(auto e: docs){
    cout << index_int.get_docs()[e - 1] << endl;
  }
  */
  
  
  return 0;
}
