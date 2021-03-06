#ifndef INV_INDEX
#define INV_INDEX


#include <unordered_map>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <sdsl/vectors.hpp>  // int_vector    enc_vector


template<typename T, typename R>
class inv_index{

private:
  // --- Variables --- //
  int ndocs;                        // count docs in the inverted index
  bool compressed;
  int c_size;
  int C;
  std::vector<std::string> docs;    // Se guardan los nombres de los documentos
  std::unordered_map<T, R*> data;   // map
  
  // --- Metodos --- //
  void make_vecs(std::unordered_map<T, std::vector<int>> &map){

    for(auto e: map){
      // Crear vector de tamanio del vector de docs que aparecer la palabra e.first
      data[e.first] = new R(e.second.size());

      for(int i = 0; i < e.second.size(); i++){
	//std::cout << "Pasando doc " << e.second[i] << std::endl;
	(*data[e.first])[i] = e.second[i];
      }
    }
  }
  
 public:
  // --- Metodos --- //
  inv_index(){       // Constructor para int_vector
    this -> ndocs = 0;
    compressed = false;
    c_size = 0;
  }

  // Constructor para enc_vector
  inv_index(inv_index<T, sdsl::int_vector<>> &index){
    this -> ndocs = index.get_ndocs();
    this -> C = index.get_C();
    this -> docs = index.get_docs();

    for(auto e: index.get_map()){
      data[e.first] = new R(*e.second);
    }
    compressed = false;
    c_size = 0;
  }

  // Insertar
  void insert_doc(std::string route, int C){

    this -> C = C;           // Size of every document (in chars)
    int count = 0;           // Contador de caracteres
    std::string name_doc;    // Guardamos el nombre base del documento
    std::string line, word;  // strings para guardar lineas y palabra del doc
    std::istringstream iss;  // Token para tomar palabras de la linea

    // Mapa para indice invertido auxiliar   (palabra (key) , vector de docs (int))
    std::unordered_map<T, std::vector<int>> map;

    
    std::ifstream inFile(route);

    // Si el archivo no existe, error
    if(!inFile.is_open()){
      std::cout << "No se encontro archivo " << route << std::endl;
      return;
    }

    // Si el archivo esta vacio, avisar y no hacer nada
    if(inFile.eof()){
      std::cout << "El archivo " << route << " esta vacio, no se inserta\n";
      return;
    }

    // Si existe...

    // Verificar nombre de la ruta para separar nombre del documento de la ruta
    int pos = -1;
    for(int i = 0; i < route.size(); i++){
      if(route[i] == '/')
	pos = i;
    }

    // Separar el la ruta con el nombre del archivo con el ultimo '/'
    if(pos != -1){
      //std::cout << route.substr(pos + 1, route.size() - pos - 1) << std::endl;
      //docs.push_back(route.substr(pos + 1, route.size() - pos - 1));
      name_doc = route.substr(pos + 1, route.size() - pos -1);
    }
    else
      name_doc = route;

    // Insertar palabras en mapa
    while(!inFile.eof()){
      // Leer linea por linea
      std::getline(inFile, line);
      // Lo separamos en palabras
      if(inFile.good()){
	iss.clear();
	iss.str(line);

	// Cada palabra se pushea al mapa
	while(iss.good()){

	  // Cuando el contador de caracteres se reinicia, pushear otro doc
	  if(count == 0){
	    ndocs++;
	    docs.push_back(name_doc + "_" + std::to_string(ndocs));
	    //std::cout << "Se crea " << docs.back() << std::endl;
	  }

	  iss >> word;
	  count += word.size();

	  // Comprobar si llegamos a los "C" caracteres (1000 en caso de lab)
	  // Si llegamos, reiniciar contador
	  if(count >= C)
	    count = 0;

	  // Si la palabra ya esta insertada en el doc, saltar
	  if(map[word].size() == 0){
	    //std::cout << "Se inserta " << word << " por size 0\n";
	    map[word].push_back(ndocs);
	  }
	  else if(map[word].back() != ndocs){
	    //std::cout << "Nueva palabra " << word << std::endl;
	    map[word].push_back(ndocs);
	  }
	}
      }
    }

    make_vecs(map);
    
  }// END INSERT_DOC

  std::vector<int> merge(std::string key1, std::string key2){
    // Iteradores
    int i = 0;
    int j = 0;

    std::vector<int> merge;

    R *vec_1 = data[key1];
    R *vec_2 = data[key2];

    //std::cout << "Size of vec_1: " << vec_1.size() << std::endl;
    //std::cout << "Size of vec_2: " << vec_2.size() << std::endl;
    
    // Si alguna de las palabras no est??, se retorna vector vacio
    if(data[key1] -> size() == 0 || data[key2] -> size() == 0)
      return merge;

    // Buscamos en ambas listas
    while(true){

      int data_1 = (*vec_1)[i];
      int data_2 = (*vec_2)[j];

      //std::cout << "i: " << i << " j: " << j << std::endl;

      // Si coinciden, se pushea uno y avanzan ambos
      if(data_1 == data_2){
	merge.push_back(data_1);
	i++;
	j++;
      }
      // Si no coinciden, si data_1 es menor, aumenta i
      else if(data_1 < data_2){
	i++;
      }
      // Si no, aumenta j
      else{
	j++;
      }

      // Verificar si termina de recorrer alguna de las listas
      if(i >= vec_1 -> size() || j >= vec_2 -> size())
	return merge;
    }

  }
      

  
  void bit_compress(){
    for(auto e: data){
      sdsl::util::bit_compress(*e.second);
      c_size += size_in_bytes(*e.second);
    }
    compressed = true;
  }


  // In megabytes
  double size(){

    if(!compressed){
      int count = 0;
      for(auto e: data){
	count += size_in_bytes(*e.second);
      }
      return count / (double)1048576;
    }

    return c_size / (double)1048576;
  }

  // Getters

  // Obtener map para iterar sobre el
  const std::unordered_map<T, R*> &get_map(){
    return data;
  }

  int get_ndocs(){
    return ndocs;
  }

  std::vector<std::string> get_docs(){
    return docs;
  }

  int get_C(){
    return C;
  }
  
  // --- Operadores --- //
  R &operator[](T key){
    return data[key];
  } 

};


#endif
