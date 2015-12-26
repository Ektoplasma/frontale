#include "traitement.h"
#include "fusionneur.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <openssl/sha.h>

using namespace std;
Fusionneur* Fusionneur::_instance = nullptr;

Fusionneur::Fusionneur()
{
	t0=time(NULL);
	srand(t0);
}

string Fusionneur::HexFormate(const unsigned char *hash, size_t length)
{
	//init OSS
	ostringstream os;
	os.fill('0');
	os << hex;
	//parcours de la chaine et copie de 2 caractères hexa
	for(const unsigned char * ptr = hash ; ptr < hash + length ; ptr++)
		os << setw(2) << (unsigned int) *ptr;

	return os.str();
}

Fusionneur* Fusionneur::getInstance()
{
	if (Fusionneur::_instance == nullptr){
		_instance=new Fusionneur;
	}
	return _instance;
}

std::string Fusionneur::GenToken(QHostAddress addr, quint16 port)
{
	ostringstream tokenContainer;
	tokenContainer << time(NULL)-t0<< rand()%100;

	SHA_CTX ctx;
	SHA1_Init(&ctx);
	char hashChar[SHA_DIGEST_LENGTH];
	SHA1_Update(&ctx,tokenContainer.str().c_str(),tokenContainer.str().length());
	SHA1_Final((unsigned char*)hashChar,&ctx);
	string token = HexFormate((const unsigned char*) hashChar, SHA_DIGEST_LENGTH);

	cout << "TOKEN GENERE : " << token << endl;
	if (tokenToTimer.find(token) == tokenToTimer.cend()){
		tokenToTimer.emplace(token, new TimerToken(token));
		tokenToTimer[token]->setInterval(2000);
		tokenToTimer[token]->setSingleShot(true);
		cout << "Resultat du connect" << connect(tokenToTimer[token], SIGNAL(timeOutMyToken(std::string)), this, SLOT(timeoutCallback(std::string))) << endl;
	}
	else {
		cerr << "Erreur : tokenToTimer n'est pas censé avoir un TimerToken à cet emplacement avant d'avoir recu ce token" << endl;
		return "";
	}
	if (tokenToMsgList.find(token) == tokenToMsgList.cend()){
		tokenToMsgList.emplace(token, new list<string>());
	}
	else {
		// Ne devrait jamais s'afficher
		cerr << "Erreur : tokenToMsgList n'est pas censé avoir une liste à cet emplacement avant d'avoir recu ce token" << endl;
		return "";
	}
	if (tokenToClient.find(token) == tokenToClient.cend()){
		IpPort p;
		p.hostAddress=addr;
		p.port=port;
		tokenToClient.emplace(token, p);
	}
	else {
		cerr << "Erreur : tokenToClient n'est pas censé avoir un TimerToken à cet emplacement avant d'avoir recu ce token" << endl;
		return "";
	}
	return token;
}

void Fusionneur::startTimer(string token)
{
	if (tokenToTimer.find(token) != tokenToTimer.cend()){
		tokenToTimer[token]->start();
		cout << "Timer lancé" << std::endl;
	}
	else {
		cout << "Timer pas lancéé" << std::endl;
	}
}

void Fusionneur::addMessageToList(string token, string msg)
{
	if (tokenToMsgList.find(token) != tokenToMsgList.cend()){
		tokenToMsgList[token]->push_back(msg);
	}
	else {
		cerr << "Erreur : on tente de rajoutter des messages alors que le token n'existe pas. Timeout dépassé ou tentative d'attaque." << endl;
	}
}

void Fusionneur::timeoutCallback(string token)
{
	cout << __FUNCTION__ << " appelée avec le token:" << token << " en argument" << std::endl;
	std::list<string>* listeReponse = tokenToMsgList[token];
	tokenToMsgList.erase(token);

	traitement_rep_client(listeReponse);
	delete listeReponse;

	IpPort ipPortDuClient = tokenToClient[token];
	tokenToClient.erase(token);

	/* TODO */
	/*Faire l'envoi au client*/

	delete tokenToTimer[token];
	tokenToTimer.erase(token);
	cout << "Le slot à été appelé correctement avec le token" << token << endl;
}