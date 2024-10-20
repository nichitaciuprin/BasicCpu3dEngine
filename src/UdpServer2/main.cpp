#include <Std.h>
#include <StdExt.h>

#include <arpa/inet.h>
#include <sys/socket.h>

int main(void)
{
    int bufferSize = 512;
    int port = 8888;

    struct sockaddr_in si_me = {};
    struct sockaddr_in si_other = {};

    socklen_t socklen = sizeof(si_other);

    char buffer[bufferSize];

    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == -1)
        abort();

    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(port);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);

    // bind socket to port
    auto bindResult = bind(sock, (struct sockaddr*)&si_me, sizeof(si_me));
    if (bindResult == -1)
        abort();

    while (true)
    {
        // fflush(stdout);

        // try to receive some data, this is a blocking call
        int recv_len = recvfrom(sock, buffer, bufferSize, 0, (struct sockaddr*)&si_other, &socklen);
        if (recv_len == -1) abort();
        printf("recv: %s\n", buffer);

        // printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));

        int sendToResult = sendto(sock, buffer, recv_len, 0, (struct sockaddr*) &si_other, socklen);
        if (sendToResult == -1) abort();
        printf("sendto: %s\n", buffer);
    }

    // close(s);

    return 0;
}