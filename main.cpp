#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <ios>
#include <valarray>
#include <thread>
#include <chrono>
#include <iostream>
#include <random>
#include <memory>
#include <thread>
//so all the threads will wait before adding to my incoming and outgoing;
//made the mistake of making this a local variable before
std::mutex fileMutex;
int tracker = 0;
class User {
    std::vector<std::string> names = {
        "Alice", "John", "Caleb", "Noah", "Eve",
        "Blake", "Grace", "Harlot", "Ivy", "Jared",
    };

private:
    std::string name;
    static inline int i;
    int id;
    int cash;

public:
    mutable std::mutex cashMutex;
    User() {
        //Not meant to go above 10;

        this->name = names.at(i++);
        this->id = i;
        this->cash = rand() % 10000;
        //hardCode Id for blake changed for testing purposes because his id was 0 messes with cmd f when searching data;
        //i is 6 instead of 5 because it is incremented on line 35 may look off but DO NOT CHANGE;
        //if (i == 6) this->id = 36;
    }
    std::string getName() {
        return this->name;
    }
    int getId() {
        return this->id;
    }
    //Returns total cash of a user
    int getCash() {
        return this->cash;
    }
    //Function to withdraw amt of cash from users account
    int withdraw(int cash) {
        if (this->cash < cash) return 0;
        this->cash -= cash;
        return cash;
    }
    //Function to receive cash from another user
    int receive(int cash) {
        //debugging purposes
       // std::cout << "Cash before Transaction: " << this->cash << " dollars" << std::endl;
        this->cash += cash;
        //debugging purposes
       // std::cout << "Cash receieved:" << cash << " dollars" << std::endl;
        //std::cout << "Cash after transaction:" << this->cash << " dollars" << std::endl;
        return cash;
    }




};
int recalculateSender(int sender,std::uniform_int_distribution<int> random) {
    std::random_device rd;
    int newSender = random(rd);
    //checks if old sender is equal to new assignment num;
    while (sender == newSender  ) {
        newSender = random(rd);

    }
    return newSender;

}

void transactions(std::vector<std::unique_ptr<User>>& users) {
    std::fstream incoming;
    std::fstream outgoing;
    std::mutex mtx;
    incoming.open("incoming.csv", std::ios::out | std::ios::app);
    outgoing.open("outgoing.csv", std::ios::out | std::ios::app);

    // for space formatting work on later if time allows std::string spaces(8, ' ');
    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
    std::chrono::seconds end(10);
    std::random_device rd;
    std::uniform_int_distribution random(0,9);
    std::uniform_int_distribution<int> randomTwo(1,10);

    while (std::chrono::system_clock::now() <= start + end) {
        //picks a random sender from the ten names same for receiver


        int sender = random(rd);
        int amt = randomTwo(rd);
        int receiver = random(rd);
        //we don't want to send money to ourselves so we will start from the next iteration if that happens;
        if (sender == receiver ) {

            std::cout << "sender equals reciever" << std::endl;
            continue;
        }
        //choose a new sender if sender has no money
        //If we don't use continue then it will deadlock if the user runs out of cash my solution was just to start the loop over;
        if (users.at(sender)->getCash() == 0) {
            continue;
        }
        auto transaction_start = std::chrono::system_clock::now();

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        if (std::chrono::system_clock::now() - transaction_start > std::chrono::milliseconds(500)) {
            //if it takes longer then 500 milliseconds then continue;
            continue;
        }


        std::scoped_lock lock(users[sender]->cashMutex, users[receiver]->cashMutex);
        std::lock_guard lock_guard(fileMutex);
        outgoing <<"\n" << users.at(sender)->getName() << " | " << users.at(sender)->getCash() << " | " << users.at(sender)->withdraw(amt)
                  << " | "  << users.at(sender)->getCash() << " | "  << users.at(sender)->getId() << " | " << users.at(receiver)->getId();

        incoming << "\n" << users.at(receiver)->getName() << " | " << users.at(receiver)->getCash() << " | " << users.at(receiver)->receive(amt)
        << " | " << users.at(receiver)->getCash() << " | " << users.at(receiver)->getId() << " | "<< users.at(sender)->getId();
        //buffering can possible cause inconsistent data logging;
        incoming.flush();
        outgoing.flush();

    }

    std::cout << "Function is done\n";

}


void clear() {
    std::ofstream ofs;
    std::ofstream ofsTwo;
    ofs.open("incoming.csv", std::ofstream::out | std::ofstream::trunc);
    ofsTwo.open("outgoing.csv", std::ofstream::out | std::ofstream::trunc);
    ofs.close();
    ofsTwo.close();
}
void printHeader() {

    std::fstream incoming;
    std::fstream outgoing;
    incoming.open("incoming.csv", std::ios::out | std::ios::app);
    outgoing.open("outgoing.csv", std::ios::out | std::ios::app);
    outgoing << "Name | Prev Total | Money Sent | New Total | AccountID | OtherAccountID" << std::endl;
    incoming << "Name | Prev Total | Money Received | New Total | AccountID | OtherAccountID"  << std::endl;
}

int main() {
int choice = 0;
    std::vector<std::thread> threads;
    std::thread t1;
    std::vector<std::unique_ptr<User>> users;

     for (int i = 0; i<10;i++) {
        users.push_back(std::make_unique<User>());
    };


    std::cout << "1)Generate Transaction List\n2)Reset"<<std::endl;
    while (choice != 2) {
        std::cin >> choice;
        switch (choice) {

            case 1:
                printHeader();
                std::cout << "How many threads would you like to initialize?" << std::endl;
                int threadCount;
                std::cin >> threadCount;
                for (int i=0;i<threadCount;i++) {
                    threads.push_back(std::thread(transactions, std::ref(users)));
                }
                for (auto& thread : threads) {
                thread.join();
            }
            break;
            case 2: clear(); break;


        }
    }


    return 0;
}


