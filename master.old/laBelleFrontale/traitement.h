#ifndef TRAITEMENT
#define TRAITEMENT

#include <string>
#include <vector>

/*TEST*/
std::string* traitement_look(std::string& affectation);
std::string traitement_exist(std::string& status);
std::string* traitement_lookrec(std::string& datatype, std::string& status);
std::string traitement_pull(std::string& reference, std::vector<std::string>& groupes_client);
/******/

std::string traitement_req_client(std::string numero,
								  std::string action, 
								  std::string statut,
								  std::string affectation, 
								  std::vector<std::string> groupes_client, 
								  std::string typeData, 
								  std::string ref, 
								  std::string user);

std::string traitement_req_bdd(std::string numero,
							   std::string action,
							   std::string statut, 
							   std::string affectation, 
							   std::vector<std::string> groupes_client, 
							   std::string typeData, 
							   std::string ref, 
							   std::string user);

std::string traitement_rep_client(std::string num);
std::string traitement_rep_bdd(std::string num);
std::string hashString(char * to_hash);

#endif