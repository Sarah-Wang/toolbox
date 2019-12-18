#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <getopt.h>
#include <string.h>
#include <termios.h>
#include <signal.h>

static char g_file[256] = {0};
static int g_speed;
static int g_fctrl;
static char g_parity;
static int g_quit;

static void handle_signal(int sig)
{
	if (SIGINT == sig || SIGTERM == sig)
		g_quit = 1;
}

static struct sigaction sigact = {
	.sa_handler = handle_signal,
};

static struct option longopts[] = {
	{"help", no_argument, NULL, 'h'},
	{"file", required_argument, NULL, 'f'},
	{"baudrate", required_argument, NULL, 'r'},
	{"flow_ctrl", required_argument, NULL, 'c'},
	{"parity", required_argument, NULL, 'j'},
	{0, 0, 0, 0},
};

static void print_usage(void)
{
	fprintf(stdout, "uart - send video stream to uart.\n");
	fprintf(stdout, "usage:\n");
	fprintf(stdout, "\t-f | --file      the uart file name\n");
	fprintf(stdout, "\t-r | --baudrate  the serial baud rate\n");
	fprintf(stdout, "\t-c | --flow_ctrl  the serial flow control:\n");
	fprintf(stdout, "\t                  0-no, 1-hard, 2-soft\n");
	fprintf(stdout, "\t-j | --parity  the serial parity(N/O/E)\n");
	fprintf(stdout, "\t-h | --help      show this help and exit\n");
}


static int parse_arg(int argc, char **argv)
{
	int ret = 0;
	char c = '0';

	while (
		(c = getopt_long(argc, argv, "-:f:r:c:j:h", longopts, NULL))
		!= -1) {
		switch (c) {
		case 'f':
			snprintf(g_file, sizeof(g_file), "%s", optarg);
			break;
		case 'r':
			g_speed = atoi(optarg);
			break;
		case 'c':
			g_fctrl = atoi(optarg);
			break;
		case 'j':
			g_parity = optarg[0];
			break;
		case 'h':
			ret = -1;
			break;
		case ':':
			fprintf(stdout, "%c require argument\n", optopt);
			ret = -1;
			break;
		case '?':
			fprintf(stdout, "%c invalid argument\n", optopt);
			ret = -1;
			break;
		default:
			break;
		}
	}

	return ret;
}


static int uart_open(char *filename)
{
	int fd;

	fd = open(filename, O_RDWR|O_NOCTTY|O_NDELAY);
	if (fd < 0) {
		printf("%s %d: failed to open output file\n",
				__FILE__, __LINE__);
		return -1;
	}

	//nonblock
	if (fcntl(fd, F_SETFL, 0) < 0) {
		printf("fcntl failed\n");
		close(fd);
		return -1;
	}
	return fd;
}

static int uart_set(int fd, int speed, int flow_ctrl,
		int databits, int parity, int stopbits)
{
	struct termios options;
	int ret = 0;
	int i;
	int speed_arr[] = {B1500000, B1152000, B1000000, B921600,
		B576000, B500000, B460800, B230400, B115200, B57600,
		B38400, B19200, B9600, B4800, B2400, B1800, B1200,
		B600, B300, B200, B150, B134, B110, B75, B50, B0};
	int name_arr[] = {1500000, 1152000, 1000000, 921600,
		576000, 500000, 460800, 230400, 115200, 57600,
		38400, 19200, 9600, 4800, 2400, 1800, 1200,
		600, 300, 200, 150, 134, 110, 75, 50, 0};

	if (tcgetattr(fd, &options) != 0) {
		printf("Setup serial fialed!\n");
		return -1;
	}

	printf("speed = %d\n", speed);
	for (i = 0; i < sizeof(speed_arr) / sizeof(int); i++) {
		if (speed == name_arr[i]) {
			ret = cfsetispeed(&options, speed_arr[i]);
			if (ret) {
				perror("cfsetispeed");
				printf("set in speed failed\n");
			}

			ret = cfsetospeed(&options, speed_arr[i]);
			if (ret) {
				perror("cfsetispeed");
				printf("set out speed failed\n");
			}
			break;
		}
	}

	options.c_cflag |= CLOCAL;
	options.c_cflag |= CREAD;

	switch (flow_ctrl) {
	case 0: // no flow control
		options.c_cflag &= ~CRTSCTS;
		break;
	case 1: // hardware flow control
		options.c_cflag |= CRTSCTS;
		break;
	case 2: //software flow control
		options.c_cflag |= IXON|IXOFF|IXANY;
		break;
	default:
		printf("Unsupported flow control\n");
		return -1;
	}

	options.c_cflag &= ~CSIZE;
	switch (databits) {
	case 5:
		options.c_cflag |= CS5;
		break;
	case 6:
		options.c_cflag |= CS6;
		break;
	case 7:
		options.c_cflag |= CS7;
		break;
	case 8:
		options.c_cflag |= CS8;
		break;
	default:
		printf("Unsupported databits!\n");
		return -1;
	}

	switch (parity) {
	case 'n': //no parity
	case 'N':
		options.c_cflag &= ~PARENB;
		options.c_iflag &= ~INPCK;
		break;
	case 'o': //odd parity
	case 'O':
		options.c_cflag |= (PARODD | PARENB);
		options.c_iflag &= INPCK;
		break;
	case 'e': //even parity
	case 'E':
		options.c_cflag |= PARENB;
		options.c_cflag &= ~PARODD;
		options.c_iflag |= INPCK;
		break;
	case 's': //blank
	case 'S':
		options.c_cflag &= ~PARENB;
		options.c_iflag &= ~CSTOPB;
		break;
	default:
		printf("Unsupported parity\n");
		return -1;
	}

	switch (stopbits) {
	case 1:
		options.c_cflag &= ~CSTOPB;
		break;
	case 2:
		options.c_cflag |= CSTOPB;
		break;
	default:
		printf("Unsupported stop bits\n");
		return -1;
	}

	//mode
	options.c_lflag  &= ~(ICANON | ECHO | ECHOE | ISIG);  /*Input*/
	options.c_oflag  &= ~OPOST;   /*Output*/

	//wait_time: 0.1s; min char to read:1
	options.c_cc[VTIME] = 1;
	options.c_cc[VMIN] = 1;

	//if data overflow, receive data, but not read
	tcflush(fd, TCIFLUSH);

	//save configuration
	if (tcsetattr(fd, TCSANOW, &options) != 0) {
		printf("set serial error!\n");
		return -1;
	}
	return 0;
}

int process(int fd)
{
	int inflg = 0;
	char recv[1] = {0};
	char reply[32] = {0};
	int ret = 0;
	int ws;

	while (!inflg && !g_quit) {
		if (read(fd, recv, 1) > 0) {
			printf("received a char: %c\n", recv[0]);
			inflg = 1;
		}
	}

	printf("recv cmd: %s\n", recv);

	snprintf(reply, sizeof(reply), "receive %c OK!\n", recv[0]);
	reply[31] = '\0';

	write(fd, reply, sizeof(reply));
}



int main(int argc, char *argv[])
{
	int ret = 0;
	int fd;

	ret = sigaction(SIGINT, &sigact, NULL);
	ret |= sigaction(SIGTERM, &sigact, NULL);
	if (ret) {
		printf("sigaction failed!\n");
		return ret;
	}

	ret = parse_arg(argc, argv);
	if (ret) {
		print_usage();
		return ret;
	}

	fd = uart_open(g_file);
	if (fd < 0)
		return ret;

	int speed = g_speed;
	int flow_ctrl = g_fctrl;
	int databits = 8;
	int stopbits = 1;
	int parity = g_parity;

	ret = uart_set(fd, speed, flow_ctrl, databits, parity, stopbits);
	if (ret)
		goto out;

	/* write or read */
	do {

	} while (!g_quit);

out:
	close(fd);
	return ret;
}

