# User and Transaction Management System

Тази C++ апликация е система за управление на потребители и транзакции, която поддържа създаване, премахване и изкарване на списък на потребители, както и изпълнение и проверка на транзакции.


## Пускане на програмата

За да компилирате проекта, изпълнете следната команда в терминала:

```sh
g++ -I headers -o start main.cpp source/Program.cpp source/UserContainer.cpp source/TBlockContainer.cpp
./start
```
# Команди

- create-user: Създава нов потребител.
- remove-user: Премахва съществуващ потребител.
- list-users: Листва всички потребители.
- wealthiest-users: Показва най-богатите потребители.
- biggest-blocks: Показва блокове с най-много транзакции.
- send-coins: Изпраща монети между потребители.
- verify-transactions: Проверява валидността на транзакциите.
- exit: Излиза от програмата.


# Структура на кода
## UserContainer
Клас за управление на потребители. Съдържа методи за създаване, премахване, листване и търсене на потребители.

### Методи
- UserContainer(): Конструктор, инициализира контейнера за потребители.
- ~UserContainer(): Деструктор, изтрива всички потребители.
- createUser(const char* name): Създава нов потребител.
- removeUser(const char* name): Премахва потребител.
- listUsers(): Листва всички потребители.
- checkExistenceOfUser(unsigned id): Проверява дали съществува потребител с даденото ID.
- searchUserId(const char* name): Търси ID на потребител по име.
- getUserSize(): Връща броя на потребителите.
- createSysUser(): Създава системен потребител.
- readUsersFromBinaryFile(std::ifstream& inFile): Чете потребители от бинарен файл.
- writeUsersInBinaryFile(std::ofstream& outFile): Записва потребители в бинарен файл.
- wealthiestUser(unsigned numOfUsers, const TBlockContainer& transactionBlocks): Показва най-богатите потребители.

## TBlockContainer
Клас за управление на блокове с транзакции. Съдържа методи за създаване на блокове, запис и четене от файлове, както и проверка на транзакции.

### Методи
- TBlockContainer(): Конструктор, инициализира контейнера за блокове с транзакции.
- ~TBlockContainer(): Деструктор, изтрива всички блокове.
- createFirstBlock(): Създава първия блок.
- readBlocksFromBinaryFile(std::ifstream& inFile): Чете блокове от бинарен файл.
- writeBlocksInBinaryFile(std::ofstream& outFile): Записва блокове в бинарен файл.
- findUserCoins(unsigned userId): Намира монетите на потребител.
- doTransaction(unsigned senderId, unsigned receiverId, double coins): Изпълнява транзакция между потребители.
- biggestBlocks(unsigned numOfBlocks): Показва блокове с най-много транзакции.
- verifyTransactions(): Проверява валидността на транзакциите.

## Program
Главен клас на програмата, който стартира и управлява целия процес.

### Методи
- launchProgram(): Стартира програмата, чете данни от файлове и валидира команди.
- validateCommand(): Валидира и изпълнява команди въведени от потребителя.
