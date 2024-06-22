#ifndef SERVER_H
#define SERVER_H

#include <enet/enet.h>

class Server {
public:
    bool init();
    void run();
    ~Server();

private:
    ENetHost* server = nullptr;
    
};

#endif // SERVER_H
