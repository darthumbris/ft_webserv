/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: shoogenb <shoogenb@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/30 11:15:49 by shoogenb      #+#    #+#                 */
/*   Updated: 2022/06/30 16:45:44 by shoogenb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <err.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/event.h>
#include <sys/socket.h>
#include <unistd.h>

#define NUM_CLIENTS 10
#define MAX_EVENTS 32
#define MAX_MSG_SIZE 256

struct s_client_data
{
	int	fd;
} clients[NUM_CLIENTS];

int	get_connection(int fd)
{
	int	i;

	i = -1;
	while (++i < NUM_CLIENTS)
		if (clients[i].fd == fd)
			return (i);
	return (-1);
}

int	add_connection(int client_socket)
{
	int	i;

	if (client_socket < 1)
		return (-1);
	i = get_connection(0);
	if (i == -1)
		return (-1);
	clients[i].fd = client_socket;
	return (0);
}

int	delete_connection(int client_socket)
{
	int	i;

	if (client_socket < 1)
		return (-1);
	i = get_connection(client_socket);
	if (i == -1)
		return (-1);
	clients[i].fd = 0;
	return (close(client_socket));
}

void	receive_msg(int s)
{
	char	buf[MAX_MSG_SIZE];
	int		bytes_read;

	bytes_read = recv(s, buf, sizeof(buf) - 1, 0);
	buf[bytes_read] = 0;
	printf("client #%d: %s", get_connection(s), buf);
	fflush(stdout);
}

void	send_hello_world_msg(int s)
{
	char	*hello;

	hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
	send(s, hello, strlen(hello), 0);
}

void	run_event_loop(int kq, int server_socket)
{
	struct kevent			ev_set;
	struct kevent			ev_list[MAX_EVENTS];
	struct sockaddr_storage	addr;
	socklen_t				socklen;
	int						clnt_sckt;
	int						n_events;
	int						i;

	socklen = sizeof(addr);
	while (1)
	{
		n_events = kevent(kq, NULL, 0, ev_list, MAX_EVENTS, NULL);
		i = -1;
		while (++i < n_events)
		{
			if (ev_list[i].ident == (uintptr_t)server_socket)
			{
				clnt_sckt = accept
					(ev_list[i].ident, (struct sockaddr *)&addr, &socklen);
				if (add_connection(clnt_sckt) == 0)
				{
					EV_SET(&ev_set, clnt_sckt, EVFILT_READ, EV_ADD, 0, 0, NULL);
					kevent(kq, &ev_set, 1, NULL, 0, NULL);
					send_hello_world_msg(clnt_sckt);
				}
				else
				{
					printf("connection refused.\n");
					close(clnt_sckt);
				}
			}
			else if (ev_list[i].flags & EV_EOF)
			{
				clnt_sckt = ev_list[i].ident;
				printf("client #%d disconnected.\n", get_connection(clnt_sckt));
				EV_SET(&ev_set, clnt_sckt, EVFILT_READ, EV_DELETE, 0, 0, NULL);
				kevent(kq, &ev_set, 1, NULL, 0, NULL);
				delete_connection(clnt_sckt);
			}
			else if (ev_list[i].filter == EVFILT_READ)
				receive_msg(ev_list[i].ident);
		}
	}
}


//TODO:address and port need to be parsed from the config
int	create_socket_and_listen(void)
{
	struct sockaddr_in	address;
	int					local_socket;
	const int			port = 8080;

	memset((char *)&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(0x7f000001);
	address.sin_port = htons(port);
	local_socket = socket(AF_INET, SOCK_STREAM, 0);
	bind(local_socket, (struct sockaddr *)&address, sizeof(address));
	listen(local_socket, 5);
	return (local_socket);
}

int	main(void)
{
	int				local_s;
	int				kq;
	struct kevent	ev_set;

	local_s = create_socket_and_listen();
	kq = kqueue();
	EV_SET(&ev_set, local_s, EVFILT_READ, EV_ADD, 0, 0, NULL);
	kevent(kq, &ev_set, 1, NULL, 0, NULL);
	run_event_loop(kq, local_s);
	return (EXIT_SUCCESS);
}
