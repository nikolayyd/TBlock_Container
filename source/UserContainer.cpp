#include "../headers/UserContainer.h"

UserContainer::UserContainer() {
   userId = 0;
   userSize = 0;
   userCapacity = 16;
   users = new User* [userCapacity];
}

UserContainer::~UserContainer() {
    eraseUsers();
}

void UserContainer::copyUser(const User& srcUser, User* destUser) {
   destUser->id = srcUser.id;
   strcpy(destUser->name, srcUser.name);

   // because some users can be removed
   userId = destUser->id+1;
}

void UserContainer::resizeUsers() {
   userCapacity *= 2;
   User** copyUsers = new User* [userCapacity];
   for (unsigned i = 0; i < userSize; i++)
   {
      copyUsers[i] = users[i];
   }

   delete[] users;

   users = copyUsers;
}

void UserContainer::eraseUsers() {
   for (unsigned i = 0; i < userSize; i++)
   {
      delete users[i];
   }

   delete[] users;
}

bool UserContainer::checkExistenceOfUser(unsigned id) {
   for (unsigned i = 0; i < userSize; i++)
   {
      if (id == users[i]->id) {
         return true;
      }
   }
   
   return false; 
}

unsigned UserContainer::searchUserId(const char* name) const {
   unsigned saveUserId;
   for (unsigned i = 0; i < userSize; i++)
   {
      if (!strcmp(users[i]->name, name)) {
         saveUserId = users[i]->id;
      }
   }

   return saveUserId;
}

unsigned UserContainer::getUserSize() const {
   return userSize;
}

void UserContainer::createSysUser() {
   users[userSize++] = new User{userId++, "System user"};
}

void UserContainer::readUsersFromBinaryFile(std::ifstream& inFile) {
    //    Check for empty file
   inFile.seekg(0, std::ios::end);
   if(inFile.tellg() == 0) {
      users[userSize++] = new User {userId++, "System user"};
      return;
   }

   User currentUser;
   inFile.seekg(0, std::ios::beg);
   inFile.read((char*) &userSize, sizeof(unsigned));

   for(unsigned i = 0 ; i < userSize; i++) {
      inFile.read((char*) &currentUser, sizeof(User));
      users[i] = new User;
      copyUser(currentUser, users[i]);
   }
}

void UserContainer::writeUsersInBinaryFile(std::ofstream& outFile) {
   if (outFile.good()) {

      // Write number of users in the file
      outFile.write((char*) &userSize, sizeof(unsigned));

      for (unsigned i = 0; i < userSize; i++)
      {
         outFile.write((char*) &(*users[i]), sizeof(User));
      }
   }
}

void UserContainer::wealthiestUser(unsigned numOfUsers, const TBlockContainer& transactionBlocks) const { 
   if (numOfUsers > (userSize - 1)) {
      std::cout << "This amount of users can't be displayed\n";
      return;
   }

   char fileName[128];
   char command[] = "wealthiest-users";
   transactionBlocks.convertFileName(fileName, command);
   std::ofstream out(fileName);

   User** sortedUsers = new User* [userCapacity];
   User* temp;

   for (unsigned i = 0; i < userSize; i++) {
      sortedUsers[i] = users[i];
   }

   for (unsigned i = 0; i < userSize; i++) {
      for (unsigned j = i + 1; j < userSize; j++) {
         if (transactionBlocks.findUserCoins(sortedUsers[j]->id) 
            > transactionBlocks.findUserCoins(sortedUsers[i]->id)) {
            
            temp = sortedUsers[i];
            sortedUsers[i] = sortedUsers[j];
            sortedUsers[j] = temp;
         }
      }
   }

   if (out.good()) {
      for (unsigned i = 0; i < numOfUsers; i++) {
         // because of the system user
         if (sortedUsers[i]->id == 0) {
            continue;
         }

         out << "Username: "<< sortedUsers[i]->name << " -> " 
             << "coins: " 
             << transactionBlocks.findUserCoins(sortedUsers[i]->id) << '\n';
      }
   }

   delete[] sortedUsers;
   out.close();
}

void UserContainer::listUsers() {
      for (unsigned i = 0; i < userSize; i++)
      {
         std::cout << users[i]->id << " : ";
         std::cout << users[i]->name << '\n';
      }
      
}

void UserContainer::createUser(const char* name) {
   if (userSize == userCapacity) {
      resizeUsers();
   }

   for (unsigned i = 0; i < userSize; i++) {
      if (!strcmp(name, users[i]->name)) {
         std::cout << "The user cant't be created, there is user with this name!\n";
         return;
      }
   }

   users[userSize] = new User{userId++};
   strcpy(users[userSize]->name, name);
   ++userSize;
}

void UserContainer::removeUser(const char* name) {

   for (unsigned i = 0; i < userSize; i++)
   {
      if (!strcmp(name, users[i]->name)) {

         if (users[i]->id == 0) {
            std::cout << "System user can't be removed!\n";
         }

         delete users[i];
         for (unsigned j = i; j < (userSize - 1); j++)
         {
            users[j] = users[j+1];
         }
         --userSize;
         return;
      }
   }

   std::cout << "There is no such a user with this name!";
}