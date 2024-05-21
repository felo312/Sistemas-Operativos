#include <iostream>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>


int main(){
    std::vector<std::string> ejecutable = {"clasico", "transpuesta"};
    std::vector<std::string> tamMatriz = {"200", "300", "500", 
        "750", "1000", "1500", "2000", "3000", "4000"};
    std::vector<std::string> numHilos = {"1", "2", "4", "8", "16"};
    std::vector<std::string>::iterator itEjecutable;
    std::ofstream promedioClasicoCSV("salidasCSV/promedios/promedioClasico.csv");
    std::ofstream promedioTranspuestaCSV("salidasCSV/promedios/proedioTranspuesta.csv");
    std::vector<std::string>::iterator it;
    promedioClasicoCSV << " ,";
    promedioTranspuestaCSV << " ,";
    for(it = numHilos.begin(); it != numHilos.end(); it++){
        promedioClasicoCSV << *it;
        promedioTranspuestaCSV << *it;
        if(it + 1 != numHilos.end()){
            promedioClasicoCSV << ",";
            promedioTranspuestaCSV << ",";           
        }
    }
    promedioClasicoCSV << "\n";
    promedioTranspuestaCSV << "\n";
    for(itEjecutable = ejecutable.begin(); itEjecutable != ejecutable.end(); itEjecutable++){
    std::vector<std::string>::iterator itTamMatriz;
        for(itTamMatriz = tamMatriz.begin(); itTamMatriz != tamMatriz.end(); itTamMatriz++){
            if(*itEjecutable == "clasico"){
                promedioClasicoCSV << *itTamMatriz << ",";
            } else if(*itEjecutable == "transpuesta"){
                promedioTranspuestaCSV << *itTamMatriz << ",";
            }
            std::vector<std::string>::iterator itNumHilos;
            for(itNumHilos = numHilos.begin(); itNumHilos != numHilos.end(); itNumHilos++){ 
                std::string rutaLeer ="Resultados/" + *itEjecutable + "-" + *itTamMatriz + "-" + "Hilos-" + *itNumHilos + ".dat";
                std::cout << rutaLeer << "\n";
                std::ifstream archivo(rutaLeer);
                std::string linea;
                unsigned int sumaTiempos = 0;
                unsigned int numLineas = 0;
                std::string rutaDatos ="salidasCSV/" + *itEjecutable + "-" + *itTamMatriz + "-" + "Hilos-" + *itNumHilos + ".csv"; 
                std::ofstream salidaDatos(rutaDatos);
                while(std::getline(archivo, linea)){ // 0
                    std::getline(archivo, linea); // 1
                    std::getline(archivo, linea); // 2
                    std::getline(archivo, linea); // 3
                    std::getline(archivo, linea); // 4
                    std::getline(archivo, linea); // 5
                    std::cout << linea << "\n";
                    std::stringstream ss(linea);
                    std::string aux = "0";
                    ss >> aux;
                    ss >> aux;
                    salidaDatos << aux << "\n";
                    std::cout << aux << "\n";
                    sumaTiempos += std::stoll(aux);
                    std::cout << std::stoll(aux) << " " << sumaTiempos << "\n";
                    std::getline(archivo, linea); // 6
                    std::getline(archivo, linea); // 7
                    numLineas++;
                }

                std::cout << numLineas << "\n";
                unsigned int promedioTiempos = sumaTiempos / numLineas;
                std::cout << sumaTiempos << " " << numLineas << " " <<promedioTiempos << "\n";
                if(*itEjecutable == "clasico"){
                    promedioClasicoCSV << promedioTiempos;
                }
                else if(*itEjecutable == "transpuesta"){
                    promedioTranspuestaCSV << promedioTiempos;
                }
                if(!(itNumHilos + 1 == numHilos.end())){    
                    if(*itEjecutable == "clasico"){
                        promedioClasicoCSV << ",";
                    }
                    else if(*itEjecutable == "transpuesta"){
                        promedioTranspuestaCSV << ",";
                    }
                }
            }
            if(*itEjecutable == "clasico"){
                promedioClasicoCSV << "\n";
            }
            else if(*itEjecutable == "transpuesta"){
                promedioTranspuestaCSV <<"\n";
            }
        }
    }
}