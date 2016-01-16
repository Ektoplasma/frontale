#include "message.h"
#include <QCoreApplication>
#include <openssl/aes.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include <QTextStream>
#include <sstream>
#include "rsacrypt.h"
#include "clientFront.h"

Message::Message(QString msg, char type, char separateur)
{
    this->msg = msg;
    this->type = type;
    this->separateur = separateur;
    this->nbKey=0;
}

Message::Message(QString chiffre, char separateur){
    this->chiffre = chiffre;
    this->separateur = separateur;
}

Message::Message(){
 
}


QString Message::getMsg(){
    return this->msg;
}

QString Message::getChiffre(){
    return this->chiffre;
}

void Message::entete(){
    this->msg = QString("%1%2%3").arg(this->type).arg(this->separateur).arg(this->msg);
}



void Message::challenge(int N){
    srand(time(NULL));
    int chal;
    chal = N*(rand()/(double)RAND_MAX)*100;

    this->msg = QString("%1%2%3").arg(this->msg).arg(this->separateur).arg(chal);
}

void Message::chiffrement(const unsigned char *key){
    unsigned char trame[1024];

    const char *message = msg.toStdString().c_str();

    unsigned char iv[AES_BLOCK_SIZE];
    memset(iv, 0x00, AES_BLOCK_SIZE);

    AES_KEY enc_key;
    AES_set_encrypt_key(key,256,&enc_key);
    AES_cbc_encrypt((const unsigned char *)message,trame,strlen(message), &enc_key,iv,AES_ENCRYPT );

    this->chiffre = QString((const char *)trame);

}

std::string Message::crypt(unsigned char* aes_input, int size_aes_input, int numkey ){
    
    unsigned char trame [1024];
    //unsigned char iv[AES_BLOCK_SIZE];
    //memset(iv, 0x00, AES_BLOCK_SIZE);

  //  const static unsigned char aes_key[]={0xB0,0xA1,0x73,0x37,0xA4,0x5B,0xF6,0x72,0x87,0x92,0xFA,0xEF,0x7C,0x2D,0x3D,0x4D, 0x60,0x3B,0xC5,0xBA,0x4B,0x47,0x81,0x93,0x54,0x09,0xE1,0xCB,0x7B,0x9E,0x17,0x88};


    AES_KEY enc_key;
    AES_set_encrypt_key((const unsigned char *)tabKeyIv[numkey][0].c_str(), sizeof(tabKeyIv[numkey][0].c_str())*8, &enc_key);
    AES_cbc_encrypt(aes_input, trame, size_aes_input, &enc_key,(unsigned char *) tabKeyIv[numkey][1].c_str(), AES_ENCRYPT);
    //print_data("\n Encrypted",enc_out, strlen(enc_out));

    std::string retour = (const char *) trame;
    return retour;

 }

std::string Message::decrypt(unsigned char* dec_in, int size_aes_input){

    unsigned char trame [1024];
    istringstream iss;
    string chaineTest;
    int i= 0;
    int newKey;
    string aesToSend;
   // unsigned char iv[AES_BLOCK_SIZE];
  //  memset(iv, 0x00, AES_BLOCK_SIZE);
//    const static unsigned char aes_key[]={0xB0,0xA1,0x73,0x37,0xA4,0x5B,0xF6,0x72,0x87,0x92,0xFA,0xEF,0x7C,0x2D,0x3D,0x4D, 0x60,0x3B,0xC5,0xBA,0x4B,0x47,0x81,0x93,0x54,0x09,0xE1,0xCB,0x7B,0x9E,0x17,0x88};
    do{

        AES_KEY dec_key;
        AES_set_decrypt_key((const unsigned char *)tabKeyIv[i][0].c_str(), sizeof(tabKeyIv[i][0].c_str())*8, &dec_key);
        AES_cbc_encrypt(dec_in, trame, size_aes_input, &dec_key,(unsigned char *) tabKeyIv[i][1].c_str(), AES_DECRYPT);
        // print_data("\n Decrypted",dec_out, sizeof(dec_out));

        printf("%s\n", trame);

        iss.str(string((const char *)trame));

        getline(iss,chaineTest,'*');
        i++;
    }while(chaineTest != "CHALL" && i<nbKey);


    // echange de cle
    if(nbKey==i){
        iss.str(string((const char *)trame));
        getline(iss,chaineTest,'*');
        if(chaineTest=="init"){
            rsaCrypt rsaClient = rsaCrypt(2048);
            rsaClient.recupKeyPub(iss.str());

            newKey = this->genAESKey();
            aesToSend = tabKeyIv[newKey][0]+";"+tabKeyIv[newKey][1];
            toSend = "5*"+rsaClient.chiffrement(aesToSend);
            eChangeKey=true;
        }else {
            dechiffre = false;
        }


    }else{
        dechiffre = true;
    }

    numClient=i-1;

    return iss.str();
 }

/* fixit : probleme avec le dechiffrement depuis QT 5.5*/
bool Message::dechiffrement(const unsigned char *key){
    unsigned char trame [1024];
    QTextStream(stdout) << this->chiffre ;
    std::string chif = this->chiffre.toStdString();

    unsigned char iv[AES_BLOCK_SIZE];
    memset(iv, 0x00, AES_BLOCK_SIZE);

    std::cout << "ici crypt : " << chif <<  "taillt : " << chif.size() << std::endl;

    AES_KEY dec_key;
    AES_set_decrypt_key(key, sizeof(key)*8,&dec_key);
    AES_cbc_encrypt((const unsigned char *)chif.data(),trame,chif.size(),&dec_key, iv,AES_DECRYPT);
    std::cout << "ici crypt : " << chif <<  "taillt : " << chif.size() << std::endl;

    this->msg = QString((const char *)trame);
    return true;
}


int Message::genAESKey(){
    int i ;
    ostringstream oss;

    // On alloue notre structure
    char *key = (char *) malloc ( AES_KEY_LENGTH*sizeof(char) ) ;
    char *iv =(char *) malloc (AES_KEY_LENGTH*sizeof(char));
    // On initialise notre structure
    memset ( key, '\0', AES_KEY_LENGTH ) ;
    memset ( iv, '\0', AES_KEY_LENGTH ) ;

    // On génère les clés et IV avec des caractères aléatoires
    for ( i = 0; i < AES_KEY_LENGTH - 1; i++ )
    {
        key[i] = rand() % ( 122 - 97 ) + 97 ;
        iv[i] = rand() % ( 122 - 97 ) + 97 ;
    }
    oss << key;

    tabKeyIv[nbKey][0] = oss.str();

    oss << iv;

    tabKeyIv[nbKey][1] = oss.str();

    nbKey++;

    return nbKey-1;

}

string Message::getAESKey(int numKey){
    return tabKeyIv[numKey][0];
}

string Message::getAESIv(int numKey){
    return tabKeyIv[numKey][1];
}

int Message::getNumClient(){
    return numClient;
}

string Message::getToSend(){
    return toSend;
}

bool Message::getDechiffre(){
    return dechiffre;
}

bool Message::getEChangeKey(){
    return eChangeKey;
}

int Message::getNbKey(){
    return nbKey;
}
