#ifndef TBLOCK_CONTAINER_H
#define TBLOCK_CONTAINER_H

#include <ctime>
#include <iostream>
#include <fstream>
#include <cstring>


class TBlockContainer
{
private:

    struct Transaction {
        unsigned sender;
        unsigned receiver;
        double coins;
        long long time;
    };

    struct TransactionBlock {
        unsigned id;
        unsigned prevBlockId;
        unsigned prevBlockHash;
        int validTransactions;
        Transaction transactions[16];
    };

    // Time in seconds from 1st January 1970 until now
    time_t presentTime;

    unsigned blockId;
    unsigned transBlockCapacity;
    unsigned transBlockSize;
    TransactionBlock** transactionBlocks;

private:
    unsigned computeHash(const unsigned char *memory, int length);
    
    void copyTransaction(const Transaction& srcTransaction, Transaction& destTransaction);
    void copyTransactionBlock(const TransactionBlock& srcTransBlock, TransactionBlock* destTransBlock);
    void resizeTransactionBlocks();
    void eraseTransactionsBlocks();
    
public:
    TBlockContainer();
    ~TBlockContainer();

    void convertFileName(char* fileName, const char* command) const;
    void createFirstBlock();
    
    void readBlocksFromBinaryFile(std::ifstream& inFile);
    void writeBlocksInBinaryFile(std::ofstream& outFile);

    double findUserCoins(unsigned userId) const;
    double sumCoinsInTransactionBlock(unsigned blockId) const;
    
    void doTransaction(unsigned senderId, unsigned receiverId, double coins);

    void biggestBlocks(unsigned numOfBlocks);

    void verifyTransactions();
};


#endif //TBLOCK_CONTAINER_H