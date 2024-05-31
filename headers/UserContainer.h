#ifndef USER_CONTAINER_H
#define USER_CONTAINER_H

#include "TBlockContainer.h"

class UserContainer {
      
private:
    struct User {
        unsigned id;
        char name[128];
    };

    unsigned userId;
    unsigned userCapacity;
    unsigned userSize;
    User** users;

private:
    void copyUser(const User& srcUser, User* destUser);
    void resizeUsers();
    void eraseUsers();

public:
    UserContainer();
    ~UserContainer();
    
    bool checkExistenceOfUser(unsigned id);
    unsigned searchUserId(const char* name) const;
    unsigned getUserSize() const;

    void createSysUser();
    
    void readUsersFromBinaryFile(std::ifstream& inFile);
    void writeUsersInBinaryFile(std::ofstream& outFile);
    
    void wealthiestUser(unsigned numOfUsers, const TBlockContainer& blocks) const;
    void listUsers();
    
    void createUser(const char* name);
    void removeUser(const char* name);

};

#endif //USER_CONTAINER_H