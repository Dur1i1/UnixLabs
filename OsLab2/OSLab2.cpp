#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <signal.h>

volatile sig_atomic_t gotSignal = 0;
void SignalHandler(int r)
{
    gotSignal = 1;
}

int main()
{
    int intSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(4000);
    bind(intSocket, (sockaddr*)&address, sizeof(address));
    listen(intSocket, 1);
    std::cout << "Listening on port 4000.\n";

    // Reg
    struct sigaction sa;
    sigaction(SIGHUP, NULL, &sa);
    sa.sa_handler = SignalHandler;
    sa.sa_flags |= SA_RESTART;
    sigaction(SIGHUP, &sa, NULL);

    // Block
    sigset_t blockedMask, origMask;
    sigemptyset(&blockedMask);
    sigaddset(&blockedMask, SIGHUP);
    sigprocmask(SIG_BLOCK, &blockedMask, &origMask);

    int client = -1;
    while (true)
    {
        int maxFd = intSocket;
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(intSocket, &fds);

        if (client != -1)
        {
            FD_SET(client, &fds);
            if (client != -1) maxFd = client;
        }

        if (pselect(maxFd + 1, &fds, NULL, NULL, NULL, &origMask) == -1)
        {
            if (errno == EINTR)
            {
                if (gotSignal == 1)
                {
                    gotSignal = 0;
                    std::cout << "Got signal! \n";
                }
                continue;
            }
            else
            {
                std::cout << "This is an error!\n";
                return 1;
            }
        }
        if (FD_ISSET(intSocket, &fds))
        {
            bool firstClient = client == -1 ? true : false;
            if (!firstClient)
            {
                std::cout << "Closing old client!\n";
                close(client);
            }
            client = accept(intSocket, nullptr, nullptr);
            std::cout << "Client connected!\n";
            continue;
        }
        if (client != -1 && FD_ISSET(client, &fds))
        {
            char buffer[100];
            int msgSize = read(client, buffer, sizeof(buffer));
            if (msgSize > 0)
            {
                std::cout << "Got message of length " << msgSize << "\n";
            }
            else
            {
                close(client);
                std::cout << "Client disconnected!\n";
                client = -1;
            }
        }
    }

    if (client != -1) close(client);
    close(intSocket);
    return 0;

}
