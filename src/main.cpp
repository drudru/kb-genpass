

#include "common.h"

void validate_args(int argc, char * const argv[]);

bool write_str(int fd, const char * msg);

int PWLen = 0;

int main(int argc, char * const argv[])
{
    validate_args(argc, argv);

    // Render to /dev/fb1
    //
    int rngfd = open("/dev/hwrng", O_RDONLY);
    if (rngfd == -1)
    {
        write_str(2, "Could not open /dev/hwrng\n");
        exit(1);
    }

    U8 inbuff[2];
    U8 outbuff[82];

    int i = 0;
    while (i != PWLen)
    {
        if (read(rngfd, inbuff, 1) != 1)
        {
            write_str(2, "Could not read /dev/hwrng\n");
            exit(1);
        }

        U8 byte = inbuff[0];

        if (byte < 26)
        {
            outbuff[i] = 'A' + byte;
            i++;
            continue;
        }
        else
        if ((byte > 25) && (byte < 35))
        {
            outbuff[i] = '0' + (byte - 25);
            i++;
            continue;
        }
    }

    outbuff[i] = '\n';
    i++;
    outbuff[i] = 0;

    if (write(1, outbuff, i) != i)
    {
        write_str(2, "Could not write result.\n");
        exit(1);
    }
}

void arg_help()
{
    write_str(2, "usage: kb-genpass <length>\n");
    write_str(2, "       length must be between 8 and 80\n");
    write_str(2, "\n");
    write_str(2, "   ex: kb-genpass 64\n");
    exit(1);
}

void validate_args(int argc, char * const argv[])
{
    // cmd mesg -- next in chain
    if (argc != 2)
        arg_help();

    PWLen = atoi(argv[1]);

    if ((PWLen < 8) ||(PWLen > 80)) 
        arg_help();
}

bool write_str(int fd, const char * msg)
{
    int len = strlen(msg);
    if (write(fd, msg, len) != len)
        return false;

    return true;
}
