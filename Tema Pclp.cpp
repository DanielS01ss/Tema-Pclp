#include <iostream>
#include <vector>
#include <ctime>
#include <time.h>
#include <algorithm>
#include <functional>
#include <string>
#include <iomanip>
#include <sstream>
#include <chrono>


using namespace std;

///clasa transaction este folosita pentru a desemna
/// o tranzactie de la un txSender la un txRecipient
///avand un txQuantity adica practic reprezinta o tranzactie in bitcoini

class Transaction {

public:

    ///aici avem un constructor gol
    Transaction()
    {
    }
    

       Transaction(string txSender, string txRecipient, double txQuantity)
    {
        this->txSender = txSender;
        this->txRecipient = txRecipient;
        this->txQuantity = txQuantity;
    }

    void setTxSender(string sender)
    {
        this->txSender = sender;
    }

    void setTxRecipient(string recipient)
    {
        this->txRecipient = recipient;
    }

    void setTxQuantity(double qt)
    {
        this->txQuantity = qt;
    }

    string getTxSender()
    {
        return this->txSender;
    }

    string getTxRecipient()
    {
        return this->txRecipient;
    }

    double getTxQuantity()
    {
        return this->txQuantity;
    }


private:
    string txSender, txRecipient;
    double txQuantity;

};


///aceasta clasa modeleaza un block din block chain

class CryptoBlock {
public:
    CryptoBlock(int index, string previousBlockHash, string sender, string recipient, double qt)
    {
        this->index = index;
        this->timeStamp = now();
        this->previousBlockHash = previousBlockHash;
        this->blockHash = calculateHash();
        this->tx.setTxQuantity(qt);
        this->tx.setTxRecipient(recipient);
        this->tx.setTxSender(sender);
    }

    int getIndex()
    {
        return this->index;
    }

    string getTimeStamp()
    {
        return this->timeStamp;
    }
    string getPreviousBlockHash()
    {
        return this->previousBlockHash;
    }
    string getBlockHash()
    {
        return this->blockHash;
    }
    Transaction getTx()
    {
        return this->tx;
    }
private:
    int index;
    string timeStamp, previousBlockHash, blockHash;
    Transaction tx;

    string calculateHash()
    {


        hash<std::string> str_hash;
        return to_string(str_hash(timeStamp + previousBlockHash + getTx().getTxSender() + getTx().getTxRecipient() + to_string(getTx().getTxQuantity())));
    }

    string now()
    {
        ///deci chrono este un namespace mai mult
        /// in el are system clock care ce face este ca returneaza
        ///timpul cu precizie , si este mai expert implementat
        /// pentru ca a fost facut in ideea ca exista diferente 
        ///intre masinile de calcul asa ca a fost implementat asta
        ///iar time point este ceva template sau class
        ///si asta system::clock::now() returneaza timpul curent
        chrono::system_clock::time_point p = chrono::system_clock::now();
        ///variabila folosita pentru a stoca timpul
        ///si cred ca to_time_t returneaza timpul
        ///ca sa il poti stoca tin time_t
        ///cum ar veni il converteste
        ///poate este un operator overloaded
        time_t t = chrono::system_clock::to_time_t(p);
        ///aici am declarat un array pentru ca 
        ///asta accepta functia ctime
        ///care ce face este ca cred ca stocheaza
        ///timpul intr-un char array adica practic string dar in c-style
        char str[26];
        
        ctime_s(str, sizeof str, &t);

        ///aici se face conversia explicti
        ///la tipul string 
        ///ca si daca punem string st(str) s-ar face conversie
        ///deci da
        return str;
    }
};


class CryptoBlockChain {

public:

    CryptoBlockChain()
    {
        ///aici doar initializam blocul
        CryptoBlock c(0, "0", "", "", 0);
        chain.push_back(c);
    }

    void addNewBlock(string sender, string recipient, double quantity)
    {

        CryptoBlock c(this->chain.size(), chain[chain.size() - 1].getBlockHash(),
            sender, recipient, quantity);
        chain.push_back(c);

    }
    vector<CryptoBlock> getChain()
    {
        return chain;
    }

private:
    ///constructor care creeaza blocul 0 numit si „genesis block”
   ///functia de adaugare a unui noi bloc in chain
   ///functie getter pentru data membra

    string getLastHash()
    {

        return this->chain[chain.size() - 1].getBlockHash();
    }
    int getLastIndex()
    {
        return chain[chain.size() - 1].getIndex();
    }
    ///aici practic avem chainul
    vector<CryptoBlock> chain;
};


ostream& operator<<(ostream& o, CryptoBlock& b)
{
    o << "\n{" << endl;
    o << "BlockID:" << b.getIndex() << ",\n";
    o << "Time_Stamp:" << b.getTimeStamp();
    o << "Previous Block Hash:" << b.getPreviousBlockHash() << "\n";
    o << "Block_Hash:" << b.getBlockHash() << ",\n";
    o << "Sender:" << b.getTx().getTxSender() << ",\n";
    o << "Recipient:" << b.getTx().getTxRecipient() << ",\n";
    o << "Quantity:" << b.getTx().getTxQuantity() << ",\n";
    o << "}," << endl;
    return o;
}

ostream& operator<<(ostream& o, CryptoBlockChain& bc)
{
    ///iteram
    vector<CryptoBlock> v = bc.getChain();
    for (int i = 0; i < v.size(); i++)
    {
        o << v[i];
    }

    return o;
}

int main()
{
    CryptoBlockChain myChain;
    myChain.addNewBlock("John", "Bob", 10);
    myChain.addNewBlock("Bob", "Sandra", 15.5);
    cout << myChain;
    return 0;
}
