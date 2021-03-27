#define GPIO_DATA 199


#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/types.h>

int init_gpio(int gpio) {
	// export gpio to userspace
	FILE * tmpf = fopen("/sys/class/gpio/export", "w");
	char path[42];
	fprintf(tmpf, "%d\n", gpio);
	fclose(tmpf);

	// set output direction
	sprintf(path, "/sys/class/gpio/gpio%d/direction", gpio);
	tmpf = fopen(path, "w");
	fprintf(tmpf, "%s\n", "in");
	fclose(tmpf);

	sprintf(path, "/sys/class/gpio/gpio%d/edge", gpio);
	tmpf = fopen(path, "w");
	fprintf(tmpf, "%s\n", "falling");
	fclose(tmpf);

	sprintf(path, "/sys/class/gpio/gpio%d/value", gpio);
	int fd = open(path, O_RDWR | O_NONBLOCK);
	if (fd <= 0) {
		fprintf(stderr, "open of gpio %d returned %d: %s\n", gpio, fd, strerror(errno));
	}
	return fd;

}

int main(int argc, char** argv) {
	int n;
	int epfd;
	int fd;
    char buf = 0;

	epfd = epoll_create(1);
	fd = init_gpio(GPIO_DATA);


	if(fd <= 0) {
		fprintf(stderr, "error opening gpio sysfs entries\n");
		return 1;
	}

    struct epoll_event ev_data;
    struct epoll_event events;
    ev_data.events = EPOLLET;

    ev_data.data.fd = fd;

    n = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev_data);
    if (n != 0) {
		fprintf(stderr, "epoll_ctl returned %d: %s\n", n, strerror(errno));
		return 1;
	}

	size_t i;

	unsigned int value = 0;
    while(1) {
		n = epoll_wait(epfd, &events, 1, -1);

		if(n > 0) {
            n = lseek(fd, 0, SEEK_SET);
            n = read(fd, &buf, 1);
            buf -= 0x30;
            printf("GOT %d\n", buf);
            if (buf == 1) {
                system("xdotool key Esc");
            }
        }
    }


  return(0);
}
