#include <Std.h>
#include <StdExt.h>

#include <arpa/inet.h>
#include <sys/socket.h>

int main()
{
    int bufferSize = 512;
    int port = 8888;

    char buffer[bufferSize];

    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == -1)
        abort();

    struct sockaddr_in si_other = {};
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(port);
    inet_aton("127.0.0.1", &si_other.sin_addr);

    socklen_t socklen = sizeof(si_other);

    while (true)
    {
        const char* message2 = "sendtome\0";
        int messageLength = strlen(message2);
        strcpy(buffer, message2);

        // printf(buffer);
        // printf("\n");

        cout << "send" << endl;

        auto sendtoResult = sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr*)&si_other, socklen);
        if (sendtoResult == -1)
            abort();

        memset(buffer,'\0', bufferSize);

        cout << "recv" << endl;

        // blocks
        auto recvfromResult = recvfrom(sock, buffer, bufferSize, 0, (struct sockaddr*)&si_other, &socklen);
        if (recvfromResult == -1)
            abort();

        puts(buffer);
    }

    // close(s);

    return 0;
}
