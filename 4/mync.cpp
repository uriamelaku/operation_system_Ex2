#include <string>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <sys/un.h>
#include <sys/wait.h>
#include <netdb.h>
#include <fcntl.h>
using namespace std;

// global variables
int out_fd = STDOUT_FILENO;
int in_fd = STDIN_FILENO;
int port;
string path;
string ip;
string outstream;
string instream;
string command;
vector<string> command_args;
int timeout = 0;

void resolve_host()
{
    cout << "Resolving host " << ip << endl;
    struct hostent *host = gethostbyname(ip.c_str());
    if (host == NULL)
    {
        cerr << "Failed to resolve host" << endl;
        exit(1);
    }
    ip = inet_ntoa(*((struct in_addr *)host->h_addr_list[0]));
}
int TCPC()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        cerr << "Failed to create socket" << endl;
        exit(1);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip.c_str());

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        cerr << "Failed to connect to server" << endl;
        exit(1);
    }

    return sockfd;
}

int TCPS()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        cerr << "Failed to create socket" << endl;
        exit(1);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        cerr << "Failed to bind socket" << endl;
        exit(1);
    }

    if (listen(sockfd, 5) < 0)
    {
        cerr << "Failed to listen on socket" << endl;
        exit(1);
    }

    int newsockfd = accept(sockfd, NULL, NULL);
    if (newsockfd < 0)
    {
        cerr << "Failed to accept connection" << endl;
        exit(1);
    }

    return newsockfd;
}

int UDPC()
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        cerr << "Failed to create socket" << endl;
        exit(1);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip.c_str());

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        cerr << "Failed to connect to server" << endl;
        exit(1);
    }

    return sockfd;
}

int UDPS()
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        cerr << "Failed to create socket" << endl;
        exit(1);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        cerr << "Failed to bind socket" << endl;
        exit(1);
    }

    return sockfd;
}

void process_output()
{
    if (out_fd != STDOUT_FILENO)
    {
        cerr << "Output stream already set" << endl;
        exit(1);
    }

    string outstream_type = outstream.substr(0, 4);
    // check format is "TCPS[Port] , TCPC[IP],[Port] , UDPC[IP],[Port]"

    if (outstream_type != "TCPS" && outstream_type != "TCPC" && outstream_type != "UDPC")
    {
        cerr << "Invalid output stream type" << endl;
        exit(1);
    }

    if (outstream_type == "TCPS")
    {
        port = stoi(outstream.substr(4));
        out_fd = TCPS();
    }

    if (outstream_type == "TCPC" || outstream_type == "UDPC")
    {
        ip = outstream.substr(4, outstream.find(",") - 4);
        resolve_host();
        port = stoi(outstream.substr(outstream.find(",") + 1));
        if (outstream_type == "TCPC")
        {
            out_fd = TCPC();
        }
        else
        {
            out_fd = UDPC();
        }
    }
}

void process_input()
{
    if (in_fd != STDIN_FILENO)
    {
        cerr << "Input stream already set" << endl;
        exit(1);
    }
    string instream_type = instream.substr(0, 4); // get type
    // check format is "TCPS[Port] , TCPC[IP],[Port] , UDPC[IP],[Port]"

    if (instream_type != "TCPS" && instream_type != "TCPC" && instream_type != "UDPS")
    {
        cerr << "Invalid input stream type" << endl;
        exit(1);
    }

    if (instream_type == "TCPS" || instream_type == "UDPS")
    {
        port = stoi(instream.substr(4)); // get port
        if (instream_type == "TCPS")
        {
            in_fd = TCPS();
        }
        else
        {
            in_fd = UDPS();
        }
    }

    if (instream_type == "TCPC")
    {
        ip = instream.substr(4, instream.find(",") - 4); // get ip address
        resolve_host();
        port = stoi(instream.substr(instream.find(",") + 1)); // get port
        in_fd = TCPC();
    }
}

void process_command()
{
    string arg;
    for (size_t i = 0; i < command.length(); i++)
    {
        if (command[i] == ' ')
        {
            command_args.push_back(arg);
            arg = "";
        }
        else
        {
            arg += command[i];
        }
    }

    if (arg != "")
    {
        command_args.push_back(arg);
    }

    if (command_args.size() == 0)
    {
        cerr << "Invalid command" << endl;
        exit(1);
    }

    return;
}

void get_arguments(int argc, char const *argv[])
{
    int opt;
    while ((opt = getopt(argc, (char *const *)argv, "o:i:e:b:t:")) != -1)
    {
        switch (opt)
        {
        case 'o':
            outstream = optarg;
            break;
        case 'i':
            instream = optarg;
            break;
        case 'e':
            command = optarg;
            process_command();
            break;
        case 'b':
            outstream = optarg;
            instream = optarg;
            break;
        case 't':
            cout << optarg << endl;
            timeout = stoi(optarg);
            break;
        default:
            break;
        }
    }

    if (outstream != "" && outstream == instream)
    {
        process_input();
        out_fd = in_fd;
    }

    else
    {
        if (outstream != "")
        {
            process_output();
        }

        if (instream != "")
        {
            process_input();
        }
    }
}

void exec_command()
{
    char *args[command_args.size() + 1];
    for (size_t i = 0; i < command_args.size(); i++)
    {
        args[i] = (char *)command_args[i].c_str();
    }

    args[command_args.size()] = NULL;

    int pid = fork();
    if (pid == 0)
    {
        if (out_fd != STDOUT_FILENO)
        {
            dup2(out_fd, STDOUT_FILENO);
        }

        if (in_fd != STDIN_FILENO)
        {
            dup2(in_fd, STDIN_FILENO);
        }

        if (out_fd != STDOUT_FILENO)
        {
            close(out_fd);
        }

        if (in_fd != STDIN_FILENO)
        {
            close(in_fd);
        }

        execv(args[0], args);
        cerr << "Failed to execute command" << endl;
        exit(1);
    }
    else
    {
        waitpid(pid, NULL, 0);
    }
}

int main(int argc, char const *argv[])
{
    get_arguments(argc, argv);

    if (timeout > 0)
    {
        // handle signal
        signal(SIGALRM, [](int signum)
               {
            cerr << "Timeout" << endl;
            if (out_fd != STDOUT_FILENO)
            {
                close(out_fd);
            }
            if (in_fd != STDIN_FILENO)
            {
                close(in_fd);
            }
            exit(1); });

        alarm(timeout);
    }

    if (command_args.size() > 0)
    {
        cout << "Executing command " << command_args[0] << " " << command_args[1] << endl;
        exec_command();
    }

    else
    {
        // set file descriptors to non-blocking
        int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

        flags = fcntl(in_fd, F_GETFL, 0);
        fcntl(in_fd, F_SETFL, flags | O_NONBLOCK);

        flags = fcntl(STDOUT_FILENO, F_GETFL, 0);
        fcntl(STDOUT_FILENO, F_SETFL, flags | O_NONBLOCK);

        flags = fcntl(out_fd, F_GETFL, 0);
        fcntl(out_fd, F_SETFL, flags | O_NONBLOCK);

        while (1)
        {
            char buffer[1024];
            if (in_fd != STDIN_FILENO)
            {
                int n = read(in_fd, buffer, 1024);

                if (n < 0)
                {
                    if (errno != EWOULDBLOCK)
                    {
                        cerr << "Failed to read from input stream" << endl;
                        close(in_fd);
                        close(out_fd);
                        exit(1);
                    }
                }

                else if (n == 0)
                {
                    close(in_fd);
                    close(out_fd);
                    break;
                }

                else
                {
                    write(STDOUT_FILENO, buffer, n);
                    fflush(stdout);
                }
            }

            if (out_fd != STDOUT_FILENO)
            {
                int n = read(STDIN_FILENO, buffer, 1024);

                if (n < 0)
                {
                    if (errno != EWOULDBLOCK)
                    {
                        cerr << "Failed to read from input stream" << endl;
                        close(in_fd);
                        close(out_fd);
                        exit(1);
                    }
                }

                else if (n == 0)
                {
                    close(in_fd);
                    close(out_fd);
                    break;
                }

                else
                {
                    write(out_fd, buffer, n);
                    fflush(stdout);
                }
            }
        }
    }
    return 0;
}
