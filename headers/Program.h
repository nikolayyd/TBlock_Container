#ifndef PROGRAM_H
#define PROGRAM_H

#include "UserContainer.h"
#include "TBlockContainer.h"

class Program {
private:
    UserContainer users;
    TBlockContainer transactionBlocks;
public:
    Program() = default;
    ~Program() = default;
    void validateCommand();
    void launchProgram();   
};

#endif //PROGRAM_H