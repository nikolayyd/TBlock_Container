#include "../headers/TBlockContainer.h"


// PROBLEM HERE
TBlockContainer::TBlockContainer() {
   presentTime = time(NULL);
   blockId = 0;
   transBlockSize = 0;
   transBlockCapacity = 16;

   transactionBlocks = new TransactionBlock* [transBlockCapacity];
}

TBlockContainer::~TBlockContainer() {
   eraseTransactionsBlocks();
}

unsigned TBlockContainer::computeHash(const unsigned char *memory, int length) {
   unsigned hash = 0xbeaf;

   for (int c = 0; c < length; c++) {
      hash += memory[c];
      hash += hash << 10;
      hash ^= hash >> 6;
   }

   hash += hash << 3;
   hash ^= hash >> 11;
   hash += hash << 15;
   return hash;
}

void TBlockContainer::copyTransaction(const Transaction& srcTransaction, Transaction& destTransaction) {
   destTransaction.coins = srcTransaction.coins;
   destTransaction.receiver = srcTransaction.receiver;
   destTransaction.sender = srcTransaction.sender;
   destTransaction.time = srcTransaction.time;
}

void TBlockContainer::copyTransactionBlock(const TransactionBlock& srcTransBlock, TransactionBlock* destTransBlock) {
   destTransBlock->id = srcTransBlock.id;
   destTransBlock->prevBlockHash = srcTransBlock.prevBlockHash;
   destTransBlock->prevBlockId = srcTransBlock.prevBlockId;
   destTransBlock->validTransactions = srcTransBlock.validTransactions;
   for (unsigned i = 0; i < srcTransBlock.validTransactions; i++) {
      copyTransaction(srcTransBlock.transactions[i], destTransBlock->transactions[i]);
   }
}

void TBlockContainer::resizeTransactionBlocks() {
   transBlockCapacity *= 2;
   TransactionBlock** copyTransactionBlocks = new TransactionBlock* [transBlockCapacity];
   for (unsigned i = 0; i < transBlockSize; i++)
   {
      copyTransactionBlocks[i] = transactionBlocks[i];
   }

   delete[] transactionBlocks;

   transactionBlocks = copyTransactionBlocks;
}

void TBlockContainer::eraseTransactionsBlocks() {
   for (unsigned i = 0; i < transBlockSize; i++) {
      delete transactionBlocks[i];
   }

   delete[] transactionBlocks;
}

void TBlockContainer::convertFileName(char* fileName, const char* command) const{
   unsigned copyTime = presentTime;

   char presentTimeToCharArr[16];
   char tempSymbol;

   unsigned numOfDigits = 0;

   while (copyTime != 0) 
   {
      copyTime /= 10;
      numOfDigits++;
   }

   copyTime = presentTime;

   for (unsigned i = 0; i < numOfDigits; i++) {
      tempSymbol = (copyTime % 10) + 48;
      presentTimeToCharArr[(numOfDigits - i) - 1] = tempSymbol;
      copyTime /= 10;
   }   
   presentTimeToCharArr[numOfDigits] = '\0';

   strcpy(fileName, command);
   strcat(fileName, presentTimeToCharArr);
   strcat(fileName, ".txt");
}

void TBlockContainer::createFirstBlock() {
   transactionBlocks[transBlockSize] = new TransactionBlock {blockId++, 0};
   transactionBlocks[transBlockSize]->validTransactions = 0;
   transBlockSize++;
}

void TBlockContainer::readBlocksFromBinaryFile(std::ifstream& inFile) {
   inFile.seekg(0, std::ios::end);
   if(inFile.tellg() == 0) {
      // transactionBlocks[transBlockSize] = new TransactionBlock {blockId++, 0};
      // transactionBlocks[transBlockSize]->validTransactions = 0;
      // transBlockSize++;
      return;
   }
    
   TransactionBlock currentBlock;
   inFile.seekg(0, std::ios::beg);
   inFile.read((char*) &transBlockSize, sizeof(unsigned));
   for (unsigned i = 0; i < transBlockSize; i++) {
      inFile.read((char *) &currentBlock, sizeof(TransactionBlock));
      transactionBlocks[i] = new TransactionBlock;
      copyTransactionBlock(currentBlock, transactionBlocks[i]);
      ++blockId;
   }
}

void TBlockContainer::writeBlocksInBinaryFile(std::ofstream& outFile) {
    if (outFile.good()) {
      //Write count of blocks in the file
      outFile.write((char*) &transBlockSize, sizeof(unsigned));
      for (unsigned i = 0; i < transBlockSize; i++)
      {
         outFile.write((char*) &(*transactionBlocks[i]), sizeof(TransactionBlock));
      }
   }
}

double TBlockContainer::findUserCoins(unsigned userId) const {
   double coins = 0;

   for (unsigned i = 0; i < transBlockSize; i++) {
      for (unsigned j = 0;j < transactionBlocks[i]->validTransactions; j++) {
         if (userId == transactionBlocks[i]->transactions[j].receiver) {
            coins += transactionBlocks[i]->transactions[j].coins;
         }
         
         if (userId == transactionBlocks[i]->transactions[j].sender) {
            coins -= transactionBlocks[i]->transactions[j].coins;
         }
      }
   }

   return coins;
}

double TBlockContainer::sumCoinsInTransactionBlock(unsigned blockId) const {
   double sumOfCoins = 0;
   
   for (unsigned i = 0; i < transactionBlocks[blockId]->validTransactions; i++)
   {
      sumOfCoins += transactionBlocks[blockId]->transactions[i].coins;
   }

   return sumOfCoins;
}

void TBlockContainer::doTransaction(unsigned senderId, unsigned receiverId, 
                        double coins) 
{
   unsigned hash;
   unsigned countTransactions;
   
   if (transBlockSize == 0) {
      createFirstBlock();
   }

   if ((findUserCoins(senderId) < coins || senderId == receiverId) && senderId != 0) {
      std::cout << "The transaction can't be done!\n";
      return;
   }

   if (transBlockSize == transBlockCapacity) {
      resizeTransactionBlocks();
   }

   Transaction transaction{senderId, receiverId, coins, presentTime};
   countTransactions = transactionBlocks[transBlockSize-1]->validTransactions;
   
   if (countTransactions == 16) {
      hash = computeHash(reinterpret_cast<const unsigned char*> 
                     (transactionBlocks[transBlockSize-1]), sizeof(TransactionBlock));
      transactionBlocks[transBlockSize++] = new TransactionBlock {blockId, blockId-1, hash, 0};
      blockId++;
      countTransactions = 0;
   }

   copyTransaction(transaction, transactionBlocks[transBlockSize-1]->transactions[countTransactions]);
   transactionBlocks[transBlockSize-1]->validTransactions++;
}

void TBlockContainer::biggestBlocks(unsigned numOfBlocks) {
   if (numOfBlocks > transBlockSize) {
      std::cout << "This amount of blocks can't be displayed\n";
      return;
   }

   char fileName[128];
   char command[] = "biggest-blocks";
   unsigned* arrayOfSortedBlocksId = new (std::nothrow) unsigned [transBlockSize];
   unsigned temp;
   for (unsigned i = 0 ; i < transBlockSize; i++) {
      arrayOfSortedBlocksId[i] = transactionBlocks[i]->id;
   }

   for (unsigned i = 0; i < transBlockSize; i++)
   {
      for (unsigned j =i + 1; j < transBlockSize; j++)
      {
         if (sumCoinsInTransactionBlock(arrayOfSortedBlocksId[i]) 
            < sumCoinsInTransactionBlock(arrayOfSortedBlocksId[j])) {
               temp = arrayOfSortedBlocksId[i];
               arrayOfSortedBlocksId[i] = arrayOfSortedBlocksId[j];
               arrayOfSortedBlocksId[j] = temp;
            }
      }
      
   }
   
   convertFileName(fileName, command);
   std::ofstream out(fileName);



   if (out.good()) {

      for (unsigned i = 0; i < transBlockSize; i++) {
         out << "Number of transaction block: "
             << (arrayOfSortedBlocksId[i] + 1) << " -> ";
         out << "Sum of coins in the block: " 
             << sumCoinsInTransactionBlock(arrayOfSortedBlocksId[i]) << '\n';
      }
   }

   delete[] arrayOfSortedBlocksId;
   out.close();
}

void TBlockContainer::verifyTransactions() {
   bool toCheck;
   bool isCorrect = true;
   unsigned prevHash;

   if (transBlockSize == 0) {
      std::cout << "There are no transactions!\n";
      return;
   }

   if (transactionBlocks[0]->validTransactions <= 0) {
      isCorrect = false;
      std::cout << "Error in block with ID: " << transactionBlocks[0]->id;
   }

   for (unsigned i = 1; i < transBlockSize; i++)
   {
      prevHash = computeHash(reinterpret_cast<const unsigned char*> 
               (transactionBlocks[i-1]), sizeof(TransactionBlock));

      toCheck = (transactionBlocks[i-1]->id == transactionBlocks[i]->prevBlockId)
               && transactionBlocks[i]->prevBlockHash == prevHash
               && transactionBlocks[i]->validTransactions > 0;
            
      if (!toCheck) {
         std::cout << "Error in block with ID: " << transactionBlocks[i]->id;
         isCorrect = false;
      }
   }


   
   if (isCorrect) {
      std::cout << "Transactions are correct!\n";
   } 

}