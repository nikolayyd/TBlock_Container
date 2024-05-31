#include "../headers/Program.h"

void Program::validateCommand() {
   char command[128];
   
   do {   
      std::cout << "Enter command: ";
      std::cin >> command;

      if(!strcmp(command, "create-user")) {
         char name[128];
         double money = 0;
         unsigned currUserId;

         std::cout << "Enter name: "; 
         std::cin >> name;
         std::cout << "Enter money: ";
         std::cin >> money;

         if (strlen(name) + 1 > 128 || money <= 0) {
            std::cout << "User can't be created!\n";
            continue;
         }

         users.createUser(name);
         currUserId = users.searchUserId(name);
         transactionBlocks.doTransaction(0, currUserId, (money * 420));

      } 
      else if (!strcmp(command, "remove-user")) {
         char name[128];
         std::cout << "Enter name: ";
         std::cin >> name;

         unsigned currentUserId = users.searchUserId(name);
         unsigned prevUserSize = users.getUserSize();
         unsigned userCoins;
         users.removeUser(name);
         // Check if user is removed and then do transaction to system user
         if (users.getUserSize() < prevUserSize) {
            userCoins = transactionBlocks.findUserCoins(currentUserId);
            transactionBlocks.doTransaction(currentUserId, 0, userCoins);
         }
      }

      else if (!strcmp(command, "list-users")) {
         users.listUsers();
      }

      else if (!strcmp(command, "wealthiest-users")) {
         unsigned numOfUsers;
         std::cout << "Enter number of users, which should be displayed: ";
         std::cin >> numOfUsers;
         users.wealthiestUser(numOfUsers, transactionBlocks);
      }

      else if (!strcmp(command, "biggest-blocks")) {
         unsigned numOfTransBlocks;
         std::cout << "Enter number of blocks, which should be displayed: ";
         std::cin >> numOfTransBlocks;
         transactionBlocks.biggestBlocks(numOfTransBlocks);
      }

      else if (!strcmp(command, "send-coins")) {
         unsigned senderId;
         unsigned receiverId;
         double coins;
         bool isTransactionValid;

         std::cout << "Enter sender id: ";
         std::cin >> senderId;
         std::cout << "Enter receiver id: ";
         std::cin >> receiverId;
         std::cout << "Enter coins: ";
         std::cin >> coins;

         isTransactionValid = coins > 0 && senderId != receiverId 
            && users.checkExistenceOfUser(senderId) 
            && users.checkExistenceOfUser(receiverId)
            && senderId != 0 && receiverId != 0;

         if (isTransactionValid) {
            transactionBlocks.doTransaction(senderId, receiverId, coins);
         } else {
            std::cout << "Transaction can't be done!\n";
         }
      }

      else if(!strcmp(command, "verify-transactions")) {
         transactionBlocks.verifyTransactions();
      }
      
      else if (!strcmp(command, "exit")) {              
         break;
      }
      else {
         std::cout << "No such a command!\n";
      }
   } while (true);
} 

void Program::launchProgram() {
   std::ifstream usersFileIn("users.dat", std::ios::binary);
   std::ifstream blocksFileIn("blocks.dat", std::ios::binary);
 
   if (usersFileIn.is_open()) {
      users.readUsersFromBinaryFile(usersFileIn);
      usersFileIn.close();
   } else {
      users.createSysUser();
   }
   
   if (blocksFileIn.is_open()) {
      transactionBlocks.readBlocksFromBinaryFile(blocksFileIn);
      blocksFileIn.close();
   } 

   validateCommand();
   
   std::ofstream usersFileOut("users.dat", std::ios::binary);
   std::ofstream blocksFileOut("blocks.dat", std::ios::binary);
   
   users.writeUsersInBinaryFile(usersFileOut);
   transactionBlocks.writeBlocksInBinaryFile(blocksFileOut);

   usersFileOut.close(); 
   blocksFileOut.close();
}